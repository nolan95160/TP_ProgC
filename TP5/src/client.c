/*
 * SPDX-FileCopyrightText: 2021 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "client.h"

int envoie_operateur_numeros(int socketfd, const char *operateur, int num1, int num2)
{
  char data[1024];
  snprintf(data, sizeof(data), "calcule : %s %d %d", operateur, num1, num2);

  if (write(socketfd, data, strlen(data)) < 0)
  {
    perror("Erreur d'ecriture");
    return -1;
  }

  memset(data, 0, sizeof(data));
  if (read(socketfd, data, sizeof(data)) < 0)
  {
    perror("Erreur de lecture");
    return -1;
  }

  printf("Résultat du serveur : %s\n", data);
  return 0;
}

int envoie_recois_message(int socketfd)
{
  char data[1024];
  char message[1024];

  memset(data, 0, sizeof(data));

  printf("Votre message (max 1000 caractères): ");
  if (fgets(message, sizeof(message), stdin) == NULL)
  {
    return -1;
  }

  message[strcspn(message, "\r\n")] = '\0';
  snprintf(data, sizeof(data), "message: %s", message);

  if (write(socketfd, data, strlen(data)) < 0)
  {
    perror("Erreur d'ecriture");
    return -1;
  }

  memset(data, 0, sizeof(data));
  if (read(socketfd, data, sizeof(data)) < 0)
  {
    perror("Erreur de lecture");
    return -1;
  }

  printf("Message reçu: %s\n", data);
  return 0;
}

int main(void)
{
  int socketfd;
  struct sockaddr_in server_addr;
  char choix[32];

  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("connection serveur");
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    printf("Choisir un mode (message / calcule) : ");
    if (fgets(choix, sizeof(choix), stdin) == NULL)
    {
      break;
    }

    choix[strcspn(choix, "\r\n")] = '\0';

    if (strcmp(choix, "message") == 0)
    {
      envoie_recois_message(socketfd);
    }
    else if (strcmp(choix, "calcule") == 0)
    {
      char operateur[8];
      int num1, num2;
      printf("Opérateur (+, -, *, /, %%) : ");
      scanf("%7s", operateur);
      printf("Nombre 1 : ");
      scanf("%d", &num1);
      printf("Nombre 2 : ");
      scanf("%d", &num2);
      getchar();
      envoie_operateur_numeros(socketfd, operateur, num1, num2);
    }
    else
    {
      printf("Choix invalide.\n");
    }
  }

  close(socketfd);
  return 0;
}
