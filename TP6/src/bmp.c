/*
 * SPDX-FileCopyrightText: 2021 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

/*
 * fonction d'analyse des couleurs dans l'image du format BMP
 * Il faut un argument : le chemin du fichier image
 */
couleur_compteur *analyse_bmp_image(char *nom_de_fichier)
{
  FILE *image = fopen(nom_de_fichier, "rb");
  if (image == NULL)
  {
    perror("Erreur: ouverture BMP");
    return NULL;
  }

  bmp_header bheader;
  bmp_info_header binfo_header;
  couleur_compteur *cc = NULL;
  couleur pixels = {0};
  unsigned char *row = NULL;

  if (fread(&bheader, sizeof(bheader), 1, image) != 1 ||
      fread(&binfo_header, sizeof(binfo_header), 1, image) != 1 ||
      bheader.type != 0x4D42 || binfo_header.info_header_size < sizeof(binfo_header))
  {
    fprintf(stderr, "Erreur: en-tete BMP invalide\n");
    goto cleanup;
  }

  int32_t height = (int32_t)binfo_header.hauteur;
  if (binfo_header.largeur == 0 || height == 0 || height == INT32_MIN ||
      (binfo_header.compte_bit != 24 && binfo_header.compte_bit != 32) ||
      binfo_header.compression != 0)
  {
    fprintf(stderr, "Erreur: BMP non compresse 24/32 bits requis\n");
    goto cleanup;
  }

  uint64_t width = binfo_header.largeur;
  uint64_t rows = height < 0 ? (uint64_t)-height : (uint64_t)height;
  uint64_t pixel_count = width * rows;
  uint64_t row_size = ((width * binfo_header.compte_bit + 31) / 32) * 4;
  if (pixel_count > INT_MAX || row_size > SIZE_MAX)
  {
    fprintf(stderr, "Erreur: dimensions BMP trop grandes\n");
    goto cleanup;
  }

  pixels.compte_bit = binfo_header.compte_bit == 24 ? BITS24 : BITS32;
  if (pixels.compte_bit == BITS24)
  {
    pixels.c.c24 = calloc((size_t)pixel_count, sizeof(couleur24));
    if (pixels.c.c24 == NULL)
    {
      perror("Erreur: allocation des pixels");
      goto cleanup;
    }
  }
  else
  {
    pixels.c.c32 = calloc((size_t)pixel_count, sizeof(couleur32));
    if (pixels.c.c32 == NULL)
    {
      perror("Erreur: allocation des pixels");
      goto cleanup;
    }
  }

  row = malloc((size_t)row_size);
  if (row == NULL || fseek(image, (long)bheader.offset, SEEK_SET) != 0)
  {
    perror("Erreur: acces aux pixels BMP");
    goto cleanup;
  }

  size_t bytes_per_pixel = binfo_header.compte_bit / 8;
  for (uint64_t y = 0; y < rows; y++)
  {
    if (fread(row, 1, (size_t)row_size, image) != (size_t)row_size)
    {
      fprintf(stderr, "Erreur: pixels BMP incomplets\n");
      goto cleanup;
    }

    uint64_t destination_y = height > 0 ? rows - y - 1 : y;
    for (uint64_t x = 0; x < width; x++)
    {
      size_t source = (size_t)(x * bytes_per_pixel);
      size_t destination = (size_t)(destination_y * width + x);
      if (pixels.compte_bit == BITS24)
      {
        pixels.c.c24[destination] = (couleur24){row[source], row[source + 1], row[source + 2]};
      }
      else
      {
        pixels.c.c32[destination] = (couleur32){row[source], row[source + 1], row[source + 2], row[source + 3]};
      }
    }
  }

  cc = compte_couleur(&pixels, (int)pixel_count);
  if (cc != NULL)
  {
    trier_couleur_compteur(cc);
  }

cleanup:
  free(row);
  if (pixels.compte_bit == BITS24)
  {
    free(pixels.c.c24);
  }
  else if (pixels.compte_bit == BITS32)
  {
    free(pixels.c.c32);
  }
  fclose(image);
  return cc;
}
