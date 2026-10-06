/*
 * SPDX-FileCopyrightText: 2021 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <math.h>
#include <ctype.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serveur.h"
#include "json.h"
int socketfd;

#define MAX_COLORS 30

static int couleur_svg_valide(const char *color)
{
  if (color == NULL || strlen(color) != 7 || color[0] != '#')
  {
    return 0;
  }
  for (int index = 1; index < 7; index++)
  {
    if (!isxdigit((unsigned char)color[index]))
    {
      return 0;
    }
  }
  return 1;
}

int visualize_plot()
{
  if (getenv("DISPLAY") == NULL)
  {
    printf("SVG genere: %s\n", svg_file_path);
    return 0;
  }

  const char *browser = "firefox";

  char command[256];
  snprintf(command, sizeof(command), "%s %s", browser, svg_file_path);

  int result = system(command);

  if (result == 0)
  {
    printf("SVG file opened in %s.\n", browser);
  }
  else
  {
    printf("Failed to open the SVG file.\n");
  }

  return 0;
}

double degreesToRadians(double degrees)
{
  return degrees * M_PI / 180.0;
}

int plot(const json_message *message)
{
  if (message == NULL || strcmp(message->code, "couleurs") != 0 ||
      message->value_count == 0 || message->value_count > MAX_COLORS)
  {
    fprintf(stderr, "Requete de couleurs invalide.\n");
    return 1;
  }

  for (size_t index = 0; index < message->value_count; index++)
  {
    if (!couleur_svg_valide(message->values[index]))
    {
      fprintf(stderr, "Couleur SVG invalide: %s\n", message->values[index]);
      return 1;
    }
  }

  size_t color_count = message->value_count;

  FILE *svg_file = fopen(svg_file_path, "w");
  if (svg_file == NULL)
  {
    perror("Error opening file");
    return 1;
  }

  fprintf(svg_file, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
  fprintf(svg_file, "<svg width=\"400\" height=\"400\" xmlns=\"http://www.w3.org/2000/svg\">\n");
  fprintf(svg_file, "  <rect width=\"100%%\" height=\"100%%\" fill=\"#ffffff\" />\n");

  double center_x = 200.0;
  double center_y = 200.0;
  double radius = 150.0;

  double start_angle = -90.0;

  for (int i = 0; i < color_count; i++)
  {
    double slice = 360.0 / color_count;
    double end_angle = start_angle + slice;

    if (color_count == 1)
    {
      fprintf(svg_file, "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" fill=\"%s\" />\n",
              center_x, center_y, radius, message->values[i]);
    }
    else
    {
      double start_angle_rad = degreesToRadians(start_angle);
      double end_angle_rad = degreesToRadians(end_angle);
      double x1 = center_x + radius * cos(start_angle_rad);
      double y1 = center_y + radius * sin(start_angle_rad);
      double x2 = center_x + radius * cos(end_angle_rad);
      double y2 = center_y + radius * sin(end_angle_rad);
      int large_arc = slice > 180.0 ? 1 : 0;

      fprintf(svg_file, "  <path d=\"M%.2f,%.2f A%.2f,%.2f 0 %d,1 %.2f,%.2f L%.2f,%.2f Z\" fill=\"%s\" />\n",
              x1, y1, radius, radius, large_arc, x2, y2, center_x, center_y, message->values[i]);
    }
    start_angle = end_angle;
  }

  fprintf(svg_file, "</svg>\n");

  fclose(svg_file);

  visualize_plot();
  return 0;
}

/* renvoyer un message (*data) au client (client_socket_fd)
 */
int renvoie_message(int client_socket_fd, char *data)
{
  int data_size = write(client_socket_fd, (void *)data, strlen(data));

  if (data_size < 0)
  {
    perror("erreur ecriture");
    return (EXIT_FAILURE);
  }
  return (EXIT_SUCCESS);
}

static int envoyer_reponse_json(int client_socket_fd, const char *code,
                                const char *value)
{
  char response[1024];
  const char *values[] = {value};
  if (json_encode_message(response, sizeof(response), code, values, 1) != 0)
  {
    return EXIT_FAILURE;
  }

  size_t length = strlen(response);
  response[length++] = '\n';
  size_t sent = 0;
  while (sent < length)
  {
    ssize_t count = write(client_socket_fd, response + sent, length - sent);
    if (count <= 0)
    {
      perror("erreur ecriture JSON");
      return EXIT_FAILURE;
    }
    sent += (size_t)count;
  }
  return EXIT_SUCCESS;
}

/* accepter la nouvelle connection d'un client et lire les données
 * envoyées par le client. En suite, le serveur envoie un message
 * en retour
 */
int recois_envoie_message(int client_socket_fd, char data[1024])
{
  json_message request;
  if (json_decode_message(data, &request) != 0)
  {
    return envoyer_reponse_json(client_socket_fd, "erreur", "JSON invalide");
  }

  if (strcmp(request.code, "message") == 0 && request.value_count == 1)
  {
    printf("Message recu: %s\n", request.values[0]);
    return envoyer_reponse_json(client_socket_fd, "message", request.values[0]);
  }
  if (strcmp(request.code, "couleurs") == 0 && plot(&request) == 0)
  {
    return envoyer_reponse_json(client_socket_fd, "resultat", "SVG genere");
  }
  return envoyer_reponse_json(client_socket_fd, "erreur", "Requete invalide");
}

static int recevoir_ligne_json(int socket_fd, char *buffer, size_t buffer_size)
{
  size_t used = 0;
  while (used + 1 < buffer_size)
  {
    char value;
    ssize_t count = read(socket_fd, &value, 1);
    if (count <= 0)
    {
      return -1;
    }
    if (value == '\n')
    {
      buffer[used] = '\0';
      return 0;
    }
    buffer[used++] = value;
  }
  return -1;
}

// Fonction de gestion du signal Ctrl+C
void gestionnaire_ctrl_c(int signal)
{
  printf("\nSignal Ctrl+C capturé. Sortie du programme.\n");
  // fermer le socket
  close(socketfd);
  exit(0); // Quitter proprement le programme.
}

int main()
{
  int bind_status;

  struct sockaddr_in server_addr;

  /*
   * Creation d'une socket
   */
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0)
  {
    perror("Unable to open a socket");
    return -1;
  }

  int option = 1;
  setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  // détails du serveur (adresse et port)
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Relier l'adresse à la socket
  bind_status = bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (bind_status < 0)
  {
    perror("bind");
    return (EXIT_FAILURE);
  }

  // Enregistrez la fonction de gestion du signal Ctrl+C
  signal(SIGINT, gestionnaire_ctrl_c);

  if (listen(socketfd, 10) < 0)
  {
    perror("listen");
    close(socketfd);
    return EXIT_FAILURE;
  }

  // Écouter les messages envoyés par le client en boucle infinie
  while (1)
  {
    // Lire et répondre au client
    struct sockaddr_in client_addr;
    char data[1024];

    unsigned int client_addr_len = sizeof(client_addr);

    // nouvelle connection de client
    int client_socket_fd = accept(socketfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket_fd < 0)
    {
      perror("accept");
      return (EXIT_FAILURE);
    }

    if (recevoir_ligne_json(client_socket_fd, data, sizeof(data)) != 0)
    {
      perror("erreur lecture JSON");
      close(client_socket_fd);
      continue;
    }

    recois_envoie_message(client_socket_fd, data);
    close(client_socket_fd);
  }

  return 0;
}
