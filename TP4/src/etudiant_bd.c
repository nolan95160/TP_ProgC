#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_ETUDIANTS 5

typedef struct
{
    char nom[30];
    char prenom[30];
    char adresse[100];
    float note1;
    float note2;
} Etudiant;

static void lire_ligne(char *buffer, size_t taille)
{
    if (fgets(buffer, (int)taille, stdin) == NULL)
    {
        buffer[0] = '\0';
        return;
    }

    size_t longueur = strlen(buffer);
    if (longueur > 0 && buffer[longueur - 1] == '\n')
    {
        buffer[longueur - 1] = '\0';
    }
}

int main(void)
{
    Etudiant etudiants[NB_ETUDIANTS];
    FILE *fichier = fopen("etudiant.txt", "w");

    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture de etudiant.txt");
        return 1;
    }

    for (int i = 0; i < NB_ETUDIANTS; i++)
    {
        char tampon[128];

        printf("Entrez les details de l'etudiant.e %d :\n", i + 1);
        printf("Nom : ");
        lire_ligne(etudiants[i].nom, sizeof(etudiants[i].nom));

        printf("Prenom : ");
        lire_ligne(etudiants[i].prenom, sizeof(etudiants[i].prenom));

        printf("Adresse : ");
        lire_ligne(etudiants[i].adresse, sizeof(etudiants[i].adresse));

        printf("Note 1 : ");
        lire_ligne(tampon, sizeof(tampon));
        etudiants[i].note1 = strtof(tampon, NULL);

        printf("Note 2 : ");
        lire_ligne(tampon, sizeof(tampon));
        etudiants[i].note2 = strtof(tampon, NULL);

        fprintf(fichier,
                "Etudiant %d\n"
                "Nom : %s\n"
                "Prenom : %s\n"
                "Adresse : %s\n"
                "Note 1 : %.2f\n"
                "Note 2 : %.2f\n\n",
                i + 1,
                etudiants[i].nom,
                etudiants[i].prenom,
                etudiants[i].adresse,
                etudiants[i].note1,
                etudiants[i].note2);
    }

    fclose(fichier);
    printf("Les details des etudiants ont ete enregistres dans le fichier etudiant.txt.\n");

    return 0;
}
