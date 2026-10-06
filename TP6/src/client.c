/*
 * SPDX-FileCopyrightText: 2021 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "client.h"
#include "bmp.h"

/*
 * Fonction d'envoi et de réception de messages
 * Il faut un argument : l'identifiant de la socket
 */

int envoie_recois_message(int socketfd)
{

  char data[1024];
  // la réinitialisation de l'ensemble des données
  memset(data, 0, sizeof(data));

  // Demandez à l'utilisateur d'entrer un message
  char message[1024];
  printf("Votre message (max 1000 caracteres): ");
  fgets(message, sizeof(message), stdin);
  strcpy(data, "message: ");
  strcat(data, message);

  int write_status = write(socketfd, data, strlen(data));
  if (write_status < 0)
  {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
  }

  // la réinitialisation de l'ensemble des données
  memset(data, 0, sizeof(data));

  // lire les données de la socket
  int read_status = read(socketfd, data, sizeof(data));
  if (read_status < 0)
  {
    perror("erreur lecture");
    return -1;
  }

  printf("Message recu: %s\n", data);

  return 0;
}

static void liberer_couleurs(couleur_compteur *cc)
{
  if (cc != NULL)
  {
    if (cc->compte_bit == BITS24)
    {
      free(cc->cc.cc24);
    }
    else
    {
      free(cc->cc.cc32);
    }
    free(cc);
  }
}

static int analyse(char *pathname, char *data, size_t data_size, int requested_count)
{
  couleur_compteur *cc = analyse_bmp_image(pathname);
  if (cc == NULL || cc->size <= 0)
  {
    liberer_couleurs(cc);
    fprintf(stderr, "Aucune couleur exploitable dans l'image.\n");
    return -1;
  }

  int count = cc->size < requested_count ? cc->size : requested_count;
  size_t used = (size_t)snprintf(data, data_size, "couleurs:%d,", count);

  for (int index = 0; index < count; index++)
  {
    char color[8];
    int color_index = cc->size - index - 1;
    if (cc->compte_bit == BITS32)
    {
      couleur32 pixel = cc->cc.cc32[color_index].c;
      snprintf(color, sizeof(color), "#%02x%02x%02x", pixel.rouge, pixel.vert, pixel.bleu);
    }
    else
    {
      couleur24 pixel = cc->cc.cc24[color_index].c;
      snprintf(color, sizeof(color), "#%02x%02x%02x", pixel.rouge, pixel.vert, pixel.bleu);
    }

    int written = snprintf(data + used, data_size - used, "%s%s", index == 0 ? "" : ",", color);
    if (written < 0 || (size_t)written >= data_size - used)
    {
      liberer_couleurs(cc);
      return -1;
    }
    used += (size_t)written;
  }

  liberer_couleurs(cc);
  return 0;
}

int envoie_couleurs(int socketfd, char *pathname, int color_count)
{
  char data[1024];
  memset(data, 0, sizeof(data));
  if (analyse(pathname, data, sizeof(data), color_count) != 0)
  {
    return -1;
  }

  int write_status = write(socketfd, data, strlen(data));
  if (write_status < 0)
  {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
  }

  return 0;
}

static int lire_nombre_couleurs(void)
{
  char input[32];
  printf("Nombre de couleurs a traiter (1-30) : ");
  if (fgets(input, sizeof(input), stdin) == NULL)
  {
    return -1;
  }

  errno = 0;
  char *end = NULL;
  long count = strtol(input, &end, 10);
  while (end != NULL && isspace((unsigned char)*end))
  {
    end++;
  }
  if (errno != 0 || end == input || *end != '\0' || count < 1 || count > 30)
  {
    fprintf(stderr, "Le nombre de couleurs doit etre compris entre 1 et 30.\n");
    return -1;
  }
  return (int)count;
}

int main(int argc, char **argv)
{
  int socketfd;
  struct sockaddr_in server_addr;
  int color_count;

  if (argc != 2)
  {
    printf("usage: ./client chemin_bmp_image\n");
    return (EXIT_FAILURE);
  }

  color_count = lire_nombre_couleurs();
  if (color_count < 1)
  {
    return EXIT_FAILURE;
  }

  /*
   * Creation d'une socket
   */
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // détails du serveur (adresse et port)
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  // demande de connection au serveur
  int connect_status = connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (connect_status < 0)
  {
    perror("connection serveur");
    exit(EXIT_FAILURE);
  }
  if (envoie_couleurs(socketfd, argv[1], color_count) != 0)
  {
    close(socketfd);
    return EXIT_FAILURE;
  }

  close(socketfd);
  return EXIT_SUCCESS;
}
