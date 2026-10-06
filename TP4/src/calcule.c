#include <stdio.h>
#include <stdlib.h>

#include "operator.h"

int main(int argc, char *argv[])
{
    int num1;
    int num2 = 0;

    if (argc < 3)
    {
        printf("Usage : ./calcule <operateur> <nombre1> [nombre2]\n");
        return 1;
    }

    num1 = atoi(argv[2]);

    if (argv[1][0] != '~')
    {
        if (argc < 4)
        {
            printf("Usage : ./calcule <operateur> <nombre1> <nombre2>\n");
            return 1;
        }
        num2 = atoi(argv[3]);
    }

    switch (argv[1][0])
    {
        case '+':
            printf("Résultat : %d\n", somme(num1, num2));
            break;
        case '-':
            printf("Résultat : %d\n", difference(num1, num2));
            break;
        case '*':
            printf("Résultat : %d\n", produit(num1, num2));
            break;
        case '/':
            printf("Résultat : %d\n", quotient(num1, num2));
            break;
        case '%':
            printf("Résultat : %d\n", modulo(num1, num2));
            break;
        case '&':
            printf("Résultat : %d\n", et_binaire(num1, num2));
            break;
        case '|':
            printf("Résultat : %d\n", ou_binaire(num1, num2));
            break;
        case '~':
            printf("Résultat : %d\n", negation(num1));
            break;
        default:
            printf("Opérateur invalide.\n");
            return 1;
    }

    return 0;
}
