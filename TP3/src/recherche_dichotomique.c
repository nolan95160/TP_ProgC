#include <stdio.h>

static void afficher_tableau(const int tableau[], int taille)
{
    for (int i = 0; i < taille; i++)
    {
        printf("%d ", tableau[i]);
    }
    printf("\n");
}

static int recherche_dichotomique(const int tableau[], int taille, int recherche)
{
    int gauche = 0;
    int droit = taille - 1;

    while (gauche <= droit)
    {
        int milieu = gauche + (droit - gauche) / 2;

        if (tableau[milieu] == recherche)
        {
            return 1;
        }
        if (tableau[milieu] < recherche)
        {
            gauche = milieu + 1;
        }
        else
        {
            droit = milieu - 1;
        }
    }

    return 0;
}

int main(void)
{
    int tableau[100];
    int valeur;

    for (int i = 0; i < 100; i++)
    {
        tableau[i] = i * 2 - 100;
    }

    printf("Tableau trié :\n");
    afficher_tableau(tableau, 100);

    printf("Entrez l'entier que vous souhaitez chercher : ");
    scanf("%d", &valeur);

    if (recherche_dichotomique(tableau, 100, valeur))
    {
        printf("Résultat : entier présent\n");
    }
    else
    {
        printf("Résultat : entier absent\n");
    }

    return 0;
}
