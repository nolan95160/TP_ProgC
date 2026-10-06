#include <stdio.h>
#include <string.h>

static int compter_occurrences(const char *ligne, const char *phrase)
{
    int compteur = 0;
    const char *position = ligne;
    size_t longueur_phrase = strlen(phrase);

    while ((position = strstr(position, phrase)) != NULL)
    {
        compteur++;
        position += longueur_phrase;
    }

    return compteur;
}

int main(void)
{
    char nom_fichier[128];
    char phrase[256];
    char ligne[512];
    FILE *fichier;
    int numero_ligne = 0;

    printf("Entrez le nom du fichier : ");
    if (fgets(nom_fichier, sizeof(nom_fichier), stdin) == NULL)
    {
        return 1;
    }
    nom_fichier[strcspn(nom_fichier, "\r\n")] = '\0';

    printf("Entrez la phrase que vous souhaitez rechercher : ");
    if (fgets(phrase, sizeof(phrase), stdin) == NULL)
    {
        return 1;
    }
    phrase[strcspn(phrase, "\r\n")] = '\0';

    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    printf("\nResultats de la recherche :\n");
    while (fgets(ligne, sizeof(ligne), fichier) != NULL)
    {
        numero_ligne++;
        int occurrences = compter_occurrences(ligne, phrase);
        if (occurrences > 0)
        {
            printf("Ligne %d, %d fois\n", numero_ligne, occurrences);
        }
    }

    fclose(fichier);
    return 0;
}
