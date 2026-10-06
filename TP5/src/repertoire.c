#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "repertoire.h"

void lire_dossier(const char *nom_repertoire)
{
    DIR *repertoire = opendir(nom_repertoire);
    struct dirent *entree;

    if (repertoire == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entree = readdir(repertoire)) != NULL)
    {
        if (strcmp(entree->d_name, ".") == 0 || strcmp(entree->d_name, "..") == 0)
        {
            continue;
        }

        printf("%s\n", entree->d_name);
    }

    closedir(repertoire);
}

static void lire_dossier_recursif_interne(const char *chemin, int niveau)
{
    DIR *repertoire = opendir(chemin);
    struct dirent *entree;

    if (repertoire == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entree = readdir(repertoire)) != NULL)
    {
        if (strcmp(entree->d_name, ".") == 0 || strcmp(entree->d_name, "..") == 0)
        {
            continue;
        }

        char chemin_complet[4096];
        snprintf(chemin_complet, sizeof(chemin_complet), "%s/%s", chemin, entree->d_name);

        printf("%*s%s\n", niveau * 2, "", entree->d_name);

        struct stat info;
        if (stat(chemin_complet, &info) == 0 && S_ISDIR(info.st_mode))
        {
            lire_dossier_recursif_interne(chemin_complet, niveau + 1);
        }
    }

    closedir(repertoire);
}

void lire_dossier_recursif(const char *nom_repertoire)
{
    lire_dossier_recursif_interne(nom_repertoire, 0);
}

void lire_dossier_iteratif(const char *nom_repertoire)
{
    char files[256][4096];
    int debut = 0;
    int fin = 1;

    snprintf(files[0], sizeof(files[0]), "%s", nom_repertoire);

    while (debut < fin)
    {
        DIR *repertoire = opendir(files[debut]);
        struct dirent *entree;

        if (repertoire == NULL)
        {
            perror("opendir");
            debut++;
            continue;
        }

        printf("[%s]\n", files[debut]);

        while ((entree = readdir(repertoire)) != NULL)
        {
            if (strcmp(entree->d_name, ".") == 0 || strcmp(entree->d_name, "..") == 0)
            {
                continue;
            }

            char chemin_complet[4096];
            snprintf(chemin_complet, sizeof(chemin_complet), "%s/%s", files[debut], entree->d_name);
            printf("%s\n", entree->d_name);

            struct stat info;
            if (stat(chemin_complet, &info) == 0 && S_ISDIR(info.st_mode))
            {
                if (fin < 256)
                {
                    snprintf(files[fin], sizeof(files[fin]), "%s", chemin_complet);
                    fin++;
                }
            }
        }

        closedir(repertoire);
        debut++;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Utilisation : %s <nom_du_repertoire> [recursif|iteratif]\n", argv[0]);
        return 1;
    }

    if (argc >= 3 && strcmp(argv[2], "recursif") == 0)
    {
        lire_dossier_recursif(argv[1]);
    }
    else if (argc >= 3 && strcmp(argv[2], "iteratif") == 0)
    {
        lire_dossier_iteratif(argv[1]);
    }
    else
    {
        lire_dossier(argv[1]);
    }

    return 0;
}
