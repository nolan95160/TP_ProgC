#include <stdio.h>
#include "operator.h"

int main(void)
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

    return 0;
}

