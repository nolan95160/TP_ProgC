/*
 * SPDX-FileCopyrightText: 2021 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#define _GNU_SOURCE // to avoid c99 related warnings
#include "couleur.h"
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>

static void liberer_compteur(couleur_compteur *compteur)
{
  if (compteur == NULL)
  {
    return;
  }
  if (compteur->compte_bit == BITS24)
  {
    free(compteur->cc.cc24);
  }
  else if (compteur->compte_bit == BITS32)
  {
    free(compteur->cc.cc32);
  }
  free(compteur);
}

// compter les couleurs distincts
couleur_compteur *compte_couleur(couleur *c, int csize)
{
  couleur_compteur *compteur;
  COMPTEBIT bc;
  int compteur_size = 0;
  int i = 0;
  if (csize <= 0)
  {
    return NULL;
  }
  char **keys = calloc((size_t)csize, sizeof(char *));
  if (keys == NULL)
  {
    free(keys);
    return NULL;
  }

  if (c->compte_bit == BITS24)
  {
    bc = BITS24;
    compteur = calloc(1, sizeof(couleur_compteur));
    if (compteur == NULL)
    {
      perror("Erreur: allocation dynamique de memoire\n");
      free(keys);
      return NULL;
    }
    compteur->compte_bit = BITS24;
    compteur->cc.cc24 = calloc((size_t)csize, sizeof(couleur24_compteur));
    if (compteur->cc.cc24 == NULL)
    {
      perror("Erreur: allocation dynamique de memoire\n");
      liberer_compteur(compteur);
      free(keys);
      return NULL;
    }
  }
  else if (c->compte_bit == BITS32)
  {
    bc = BITS32;
    compteur = calloc(1, sizeof(couleur_compteur));
    if (compteur == NULL)
    {
      perror("Erreur: allocation dynamique de memoire\n");
      free(keys);
      return NULL;
    }
    compteur->compte_bit = BITS32;
    compteur->cc.cc32 = calloc((size_t)csize, sizeof(couleur32_compteur));
    if (compteur->cc.cc32 == NULL)
    {
      perror("Erreur: allocation dynamique de memoire\n");
      liberer_compteur(compteur);
      free(keys);
      return NULL;
    }
  }
  else
  {
    perror("compte du bits inconnu");
    free(keys);
    return NULL;
  }

  /*
   * créer une table de hachage pour stocker les différentes couleurs et leur nombre
   */
  if (hcreate((size_t)csize) == 0)
  {
    perror("Erreur: creation de la table de couleurs");
    free(keys);
    liberer_compteur(compteur);
    return NULL;
  }

  for (i = 0; i < csize; i++)
  {
    ENTRY e, *es;
    char key[256];

    if (bc == BITS24)
    {
      snprintf(key, sizeof(key), "%u:%u:%u", c->c.c24[i].rouge, c->c.c24[i].vert, c->c.c24[i].bleu);
    }
    else if (bc == BITS32)
    {
      snprintf(key, sizeof(key), "%u:%u:%u:%u", c->c.c32[i].rouge, c->c.c32[i].vert, c->c.c32[i].bleu, c->c.c32[i].alpha);
    }
    e.key = key;

    es = hsearch(e, FIND);
    if (es == NULL)
    {
      compteur_size++;
      if (c->compte_bit == BITS24)
      {
        compteur->cc.cc24[compteur_size - 1].c = c->c.c24[i];
        compteur->cc.cc24[compteur_size - 1].compte = 1;
        e.data = &compteur->cc.cc24[compteur_size - 1];
      }
      else
      {
        compteur->cc.cc32[compteur_size - 1].c = c->c.c32[i];
        compteur->cc.cc32[compteur_size - 1].compte = 1;
        e.data = &compteur->cc.cc32[compteur_size - 1];
      }
      keys[compteur_size - 1] = strdup(key);
      if (keys[compteur_size - 1] == NULL)
      {
        hdestroy();
        for (int key_index = 0; key_index < compteur_size - 1; key_index++)
        {
          free(keys[key_index]);
        }
        free(keys);
        liberer_compteur(compteur);
        return NULL;
      }
      e.key = keys[compteur_size - 1];
      es = hsearch(e, ENTER);
      if (es == NULL)
      {
        perror("Erreur: impossible d'inserer\n");
        hdestroy();
        for (int key_index = 0; key_index < compteur_size; key_index++)
        {
          free(keys[key_index]);
        }
        free(keys);
        liberer_compteur(compteur);
        return NULL;
      }
    }
    else
    {
      if (bc == BITS24)
      {
        couleur24_compteur *cc24 = (couleur24_compteur *)es->data;
        cc24->compte++;
      }
      else if (bc == BITS32)
      {
        couleur32_compteur *cc32 = (couleur32_compteur *)es->data;
        cc32->compte++;
      }
    }
  }

  compteur->size = compteur_size;

  hdestroy();
  for (i = 0; i < compteur_size; i++)
  {
    free(keys[i]);
  }
  free(keys);
  return compteur;
}

// afficher les couleurs
void print_couleur(couleur *c, int csize)
{
  int i = 0;

  for (i = 0; i < csize; i++)
  {
    if (c->compte_bit == BITS24)
    { // 3 octets (RGB)
      printf("%5x %5x %5x\n", c->c.c24[i].rouge, c->c.c24[i].vert, c->c.c24[i].bleu);
    }
    else if (c->compte_bit == BITS32)
    { // 4 octets (RGBA)
      printf("%5x %5x %5x %5x\n", c->c.c32[i].rouge, c->c.c32[i].vert, c->c.c32[i].bleu, c->c.c32[i].alpha);
    }
    else
    {
      return;
    }
  }
}

// afficher le compte de couleurs distincts
void print_couleur_compteur(couleur_compteur *ccompteur)
{
  int i = 0;

  for (i = 0; i < ccompteur->size; i++)
  {
    if (ccompteur->compte_bit == BITS24)
    {
      printf("%5hx %5hx %5hx: %10d\n", ccompteur->cc.cc24[i].c.rouge, ccompteur->cc.cc24[i].c.vert, ccompteur->cc.cc24[i].c.bleu, ccompteur->cc.cc24[i].compte);
    }
    else if (ccompteur->compte_bit == BITS32)
    {
      printf("%5hx %5hx %5hx %5hx: %10d\n", ccompteur->cc.cc32[i].c.rouge, ccompteur->cc.cc32[i].c.vert, ccompteur->cc.cc32[i].c.bleu, ccompteur->cc.cc32[i].c.alpha, ccompteur->cc.cc32[i].compte);
    }
  }
}

/*
 * fonction permettant de comparer deux couleurs
 */
static int compare_compteur(const void *ccp1, const void *ccp2, void *compte_bitp)
{
  COMPTEBIT *bc = (COMPTEBIT *)compte_bitp;
  if (*bc == BITS24)
  {
    couleur24_compteur *cc241 = (couleur24_compteur *)ccp1;
    couleur24_compteur *cc242 = (couleur24_compteur *)ccp2;
    if (cc241->compte == cc242->compte)
    {
      return 0;
    }
    else if (cc241->compte > cc242->compte)
    {
      return 1;
    }
  }
  else if (*bc == BITS32)
  {
    couleur32_compteur *cc321 = (couleur32_compteur *)ccp1;
    couleur32_compteur *cc322 = (couleur32_compteur *)ccp2;
    if (cc321->compte == cc322->compte)
    {
      return 0;
    }
    else if (cc321->compte > cc322->compte)
    {
      return 1;
    }
  }
  return -1;
}

// trier le compte de couleurs distincts
void trier_couleur_compteur(couleur_compteur *ccompteur)
{
  COMPTEBIT bc = ccompteur->compte_bit;
  if (ccompteur->compte_bit == BITS24)
  {
    qsort_r(ccompteur->cc.cc24, ccompteur->size, sizeof(couleur24_compteur),
            compare_compteur, (void *)&bc);
  }
  else if (ccompteur->compte_bit == BITS32)
  {
    qsort_r(ccompteur->cc.cc32, ccompteur->size, sizeof(couleur32_compteur),
            compare_compteur, (void *)&bc);
  }
}
