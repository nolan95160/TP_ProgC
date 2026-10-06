#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void afficher_tableau(const int tableau[], int taille)
{
    for (int i = 0; i < taille; i++)
    {
        printf("%d ", tableau[i]);
    }
    printf("\n");
}

int main(void)
{
    int tableau[100];
    int valeur;
    int present = 0;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < 100; i++)
    {
        tableau[i] = rand() % 200 - 100;
    }

    printf("Tableau :\n");
    afficher_tableau(tableau, 100);

    printf("Entrez l'entier que vous souhaitez chercher : ");
    scanf("%d", &valeur);

    for (int i = 0; i < 100; i++)
    {
        if (tableau[i] == valeur)
        {
            present = 1;
            break;
        }
    }

    if (present)
    {
        printf("Résultat : entier présent\n");
    }
    else
    {
        printf("Résultat : entier absent\n");
    }

    return 0;
}
