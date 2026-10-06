#include <stdio.h>

#define TAILLE_TABLEAU 100

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Couleur;

typedef struct
{
    Couleur couleur;
    int compteur;
} CouleurCompteur;

static int couleurs_egales(const Couleur *a, const Couleur *b)
{
    return a->r == b->r && a->g == b->g && a->b == b->b && a->a == b->a;
}

static int trouver_couleur(const CouleurCompteur *distinctes, int taille, const Couleur *couleur)
{
    for (int i = 0; i < taille; i++)
    {
        if (couleurs_egales(&distinctes[i].couleur, couleur))
        {
            return i;
        }
    }

    return -1;
}

int main(void)
{
    Couleur palette[8] = {
        {255, 35, 35, 255},
        {0, 0, 0, 255},
        {255, 255, 255, 255},
        {35, 120, 255, 255},
        {50, 200, 100, 255},
        {255, 200, 0, 255},
        {120, 80, 50, 255},
        {255, 35, 35, 255}
    };

    Couleur tableau[TAILLE_TABLEAU];
    CouleurCompteur distinctes[TAILLE_TABLEAU];
    int nombre_distinctes = 0;

    for (int i = 0; i < TAILLE_TABLEAU; i++)
    {
        tableau[i] = palette[i % 7];
    }

    for (int i = 0; i < TAILLE_TABLEAU; i++)
    {
        int index = trouver_couleur(distinctes, nombre_distinctes, &tableau[i]);

        if (index == -1)
        {
            distinctes[nombre_distinctes].couleur = tableau[i];
            distinctes[nombre_distinctes].compteur = 1;
            nombre_distinctes++;
        }
        else
        {
            distinctes[index].compteur++;
        }
    }

    for (int i = 0; i < nombre_distinctes; i++)
    {
        printf("%02x %02x %02x %02x : %d\n",
               distinctes[i].couleur.r,
               distinctes[i].couleur.g,
               distinctes[i].couleur.b,
               distinctes[i].couleur.a,
               distinctes[i].compteur);
    }

    return 0;
}
