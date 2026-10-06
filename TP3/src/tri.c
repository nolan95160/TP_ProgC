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

    srand((unsigned int)time(NULL));

    for (int i = 0; i < 100; i++)
    {
        tableau[i] = rand() % 201 - 100;
    }

    printf("Tableau non trié :\n");
    afficher_tableau(tableau, 100);

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 99 - i; j++)
        {
            if (tableau[j] > tableau[j + 1])
            {
                int temp = tableau[j];
                tableau[j] = tableau[j + 1];
                tableau[j + 1] = temp;
            }
        }
    }

    printf("Tableau trié par ordre croissant :\n");
    afficher_tableau(tableau, 100);

    return 0;
}
