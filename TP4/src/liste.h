#ifndef LISTE_H
#define LISTE_H

typedef struct
{
    int rouge;
    int vert;
    int bleu;
} Couleur;

typedef struct ElementListe
{
    Couleur couleur;
    struct ElementListe *suivant;
} ElementListe;

typedef struct
{
    ElementListe *tete;
} ListeCouleurs;

void insertion(ListeCouleurs *liste, Couleur couleur);
void parcours(const ListeCouleurs *liste);

#endif
