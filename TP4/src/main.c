#include <stdio.h>
#include <stdlib.h>

#include "fichier.h"
#include "liste.h"
#include "operator.h"

static void exercice_41(void)
{
    int num1, num2;
    char operateur;

    printf("Entrez num1 : ");
    scanf("%d", &num1);
    printf("Entrez num2 : ");
    scanf("%d", &num2);
    printf("Entrez l'opérateur (+, -, *, /, %%, &, |, ~) : ");
    scanf(" %c", &operateur);

    switch (operateur)
    {
        case '+':
            printf("Resultat : %d\n", somme(num1, num2));
            break;
        case '-':
            printf("Resultat : %d\n", difference(num1, num2));
            break;
        case '*':
            printf("Resultat : %d\n", produit(num1, num2));
            break;
        case '/':
            printf("Resultat : %d\n", quotient(num1, num2));
            break;
        case '%':
            printf("Resultat : %d\n", modulo(num1, num2));
            break;
        case '&':
            printf("Resultat : %d\n", et_binaire(num1, num2));
            break;
        case '|':
            printf("Resultat : %d\n", ou_binaire(num1, num2));
            break;
        case '~':
            printf("Resultat : %d\n", negation(num1));
            break;
        default:
            printf("Operateur invalide.\n");
            break;
    }
}

static void exercice_42(void)
{
    int choix;
    char nom_fichier[128];
    char message[256];

    printf("Que souhaitez-vous faire ?\n");
    printf("1. Lire un fichier\n");
    printf("2. Ecrire dans un fichier\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    if (choix == 1)
    {
        printf("Entrez le nom du fichier a lire : ");
        scanf("%127s", nom_fichier);
        lire_fichier(nom_fichier);
    }
    else if (choix == 2)
    {
        printf("Entrez le nom du fichier dans lequel vous souhaitez ecrire : ");
        scanf("%127s", nom_fichier);
        printf("Entrez le message a ecrire : ");
        scanf(" %255[^\n]", message);
        ecrire_dans_fichier(nom_fichier, message);
    }
    else
    {
        printf("Choix invalide.\n");
    }
}

static void exercice_47(void)
{
    ListeCouleurs liste = {0};
    const Couleur couleurs[10] = {
        {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
        {255, 255, 0}, {255, 0, 255}, {0, 255, 255},
        {128, 0, 128}, {128, 128, 0}, {0, 128, 128}, {192, 192, 192}
    };

    for (int i = 0; i < 10; i++)
    {
        insertion(&liste, couleurs[i]);
    }

    parcours(&liste);
}

int main(void)
{
    int choix;

    printf("Choisissez l'exercice a executer (1, 2, 7) : ");
    scanf("%d", &choix);

    switch (choix)
    {
        case 1:
            exercice_41();
            break;
        case 2:
            exercice_42();
            break;
        case 7:
            exercice_47();
            break;
        default:
            printf("Exercice inconnu.\n");
            return 1;
    }

    return 0;
}

