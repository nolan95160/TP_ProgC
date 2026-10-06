#include <stdio.h>
#include <stdlib.h>

#include "liste.h"

void insertion(ListeCouleurs *liste, Couleur couleur)
{
    ElementListe *nouveau = malloc(sizeof(*nouveau));
    if (nouveau == NULL)
    {
        perror("Erreur d'allocation memoire");
        return;
    }

    nouveau->couleur = couleur;
    nouveau->suivant = liste->tete;
    liste->tete = nouveau;
}

void parcours(const ListeCouleurs *liste)
{
    ElementListe *actuel = liste->tete;

    printf("Liste des couleurs :\n");
    while (actuel != NULL)
    {
        printf("RGB(%d, %d, %d)\n", actuel->couleur.rouge, actuel->couleur.vert, actuel->couleur.bleu);
        actuel = actuel->suivant;
    }
}
