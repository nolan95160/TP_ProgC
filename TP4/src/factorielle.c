#include <stdio.h>

static int factorielle(int n)
{
    if (n == 0 || n == 1)
    {
        return 1;
    }

    return n * factorielle(n - 1);
}

int main(void)
{
    int valeurs[] = {0, 1, 5, 7, 10};
    int nombre = sizeof(valeurs) / sizeof(valeurs[0]);

    for (int i = 0; i < nombre; i++)
    {
        printf("fact(%d) = %d\n", valeurs[i], factorielle(valeurs[i]));
    }

    return 0;
}
