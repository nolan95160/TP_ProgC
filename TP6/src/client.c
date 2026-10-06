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
#include "json.h"

static int envoyer_json(int socketfd, const char *message)
{
  char frame[1024];
  int length = snprintf(frame, sizeof(frame), "%s\n", message);
  if (length < 0 || (size_t)length >= sizeof(frame))
  {
    return -1;
  }

  size_t sent = 0;
  while (sent < (size_t)length)
  {
    ssize_t count = write(socketfd, frame + sent, (size_t)length - sent);
    if (count <= 0)
    {
      perror("erreur ecriture");
      return -1;
    }
    sent += (size_t)count;
  }
  return 0;
}

static int recevoir_json(int socketfd, char *message, size_t message_size)
{
  size_t used = 0;
  while (used + 1 < message_size)
  {
    char value;
    ssize_t count = read(socketfd, &value, 1);
    if (count <= 0)
    {
      return -1;
    }
    if (value == '\n')
    {
      message[used] = '\0';
      return 0;
    }
    message[used++] = value;
  }
  return -1;
}

/*
 * Fonction d'envoi et de réception de messages
 * Il faut un argument : l'identifiant de la socket
 */

int envoie_recois_message(int socketfd)
{
  char message[JSON_VALUE_SIZE];
  char data[1024];
  const char *values[1];
  printf("Votre message (max 1000 caracteres): ");
  if (fgets(message, sizeof(message), stdin) == NULL)
  {
    return -1;
  }
  message[strcspn(message, "\r\n")] = '\0';
  values[0] = message;
  if (json_encode_message(data, sizeof(data), "message", values, 1) != 0 ||
      envoyer_json(socketfd, data) != 0)
  {
    fprintf(stderr, "Erreur: creation ou envoi du message JSON.\n");
    return -1;
  }

  if (recevoir_json(socketfd, data, sizeof(data)) != 0)
  {
    perror("erreur lecture");
    return -1;
  }
  json_message response;
  if (json_decode_message(data, &response) != 0 || strcmp(response.code, "message") != 0 ||
      response.value_count != 1)
  {
    fprintf(stderr, "Reponse JSON invalide.\n");
    return -1;
  }
  printf("Message recu: %s\n", response.values[0]);
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
  char color_values[JSON_MAX_VALUES][8];
  const char *values[JSON_MAX_VALUES];

  for (int index = 0; index < count; index++)
  {
    int color_index = cc->size - index - 1;
    if (cc->compte_bit == BITS32)
    {
      couleur32 pixel = cc->cc.cc32[color_index].c;
      snprintf(color_values[index], sizeof(color_values[index]), "#%02x%02x%02x", pixel.rouge, pixel.vert, pixel.bleu);
    }
    else
    {
      couleur24 pixel = cc->cc.cc24[color_index].c;
      snprintf(color_values[index], sizeof(color_values[index]), "#%02x%02x%02x", pixel.rouge, pixel.vert, pixel.bleu);
    }
    values[index] = color_values[index];
  }

  liberer_couleurs(cc);
  return json_encode_message(data, data_size, "couleurs", values, (size_t)count);
}

int envoie_couleurs(int socketfd, char *pathname, int color_count)
{
  char data[1024];
  memset(data, 0, sizeof(data));
  if (analyse(pathname, data, sizeof(data), color_count) != 0)
  {
    return -1;
  }

  if (envoyer_json(socketfd, data) != 0)
  {
    return -1;
  }

  if (recevoir_json(socketfd, data, sizeof(data)) != 0)
  {
    perror("erreur lecture");
    return -1;
  }
  json_message response;
  if (json_decode_message(data, &response) != 0 || strcmp(response.code, "resultat") != 0 ||
      response.value_count != 1)
  {
    fprintf(stderr, "Reponse JSON invalide du serveur.\n");
    return -1;
  }
  printf("Serveur: %s\n", response.values[0]);
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
  int message_mode = argc == 2 && strcmp(argv[1], "--message") == 0;

  if (argc != 2)
  {
    printf("usage: ./client chemin_bmp_image | --message\n");
    return (EXIT_FAILURE);
  }

  if (!message_mode)
  {
    color_count = lire_nombre_couleurs();
    if (color_count < 1)
    {
      return EXIT_FAILURE;
    }
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
  int result = message_mode ? envoie_recois_message(socketfd)
                            : envoie_couleurs(socketfd, argv[1], color_count);
  if (result != 0)
  {
    close(socketfd);
    return EXIT_FAILURE;
  }

  close(socketfd);
  return EXIT_SUCCESS;
}
