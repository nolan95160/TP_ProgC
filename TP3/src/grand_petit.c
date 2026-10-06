#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int tableau[100];
    int min, max;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < 100; i++)
    {
        tableau[i] = rand() % 1000 + 1;
    }

    min = tableau[0];
    max = tableau[0];

    for (int i = 1; i < 100; i++)
    {
        if (tableau[i] < min)
        {
            min = tableau[i];
        }
        if (tableau[i] > max)
        {
            max = tableau[i];
        }
    }

    printf("Le nombre le plus grand est : %d\n", max);
    printf("Le nombre le plus petit est : %d\n", min);

    return 0;
}
