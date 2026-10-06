#include <stdio.h>

#define NB_PHRASES 10

static int comparer_chaines(const char *a, const char *b)
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
        {
            return 0;
        }
        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

static int rechercher_phrase(const char *phrases[NB_PHRASES], const char *recherche)
{
    for (int i = 0; i < NB_PHRASES; i++)
    {
        if (comparer_chaines(phrases[i], recherche))
        {
            return 1;
        }
    }

    return 0;
}

int main(void)
{
    const char *phrases[NB_PHRASES] = {
        "Bonjour, comment ca va ?",
        "Le temps est magnifique aujourd'hui.",
        "C'est une belle journée.",
        "La programmation en C est amusante.",
        "Les tableaux en C sont puissants.",
        "Les pointeurs en C peuvent être déroutants.",
        "Il fait beau dehors.",
        "La recherche dans un tableau est interessante.",
        "Les structures de donnees sont importantes.",
        "Programmer en C, c'est genial."
    };

    char recherche[256];

    printf("Entrez une phrase à rechercher : ");
    fgets(recherche, sizeof(recherche), stdin);

    size_t longueur = 0;
    while (recherche[longueur] != '\0' && recherche[longueur] != '\n')
    {
        longueur++;
    }
    recherche[longueur] = '\0';

    if (rechercher_phrase(phrases, recherche))
    {
        printf("Phrase trouvée\n");
    }
    else
    {
        printf("Phrase non trouvée\n");
    }

    return 0;
}
