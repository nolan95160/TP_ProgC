#include <stdio.h>
#include <stdlib.h>

#include "fichier.h"

int lire_fichier(const char *nom_de_fichier)
{
    FILE *fichier = fopen(nom_de_fichier, "r");
    char ligne[256];

    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    printf("Contenu du fichier %s :\n", nom_de_fichier);

    while (fgets(ligne, sizeof(ligne), fichier) != NULL)
    {
        printf("%s", ligne);
    }

    fclose(fichier);
    return 0;
}

int ecrire_dans_fichier(const char *nom_de_fichier, const char *message)
{
    FILE *fichier = fopen(nom_de_fichier, "w");

    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    fprintf(fichier, "%s\n", message);
    fclose(fichier);

    printf("Le message a ete ecrit dans le fichier %s.\n", nom_de_fichier);
    return 0;
}
