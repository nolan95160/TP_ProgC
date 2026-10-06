#include <dirent.h>
#include <stdio.h>
#include <string.h>

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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Utilisation : %s <nom_du_repertoire>\n", argv[0]);
        return 1;
    }

    lire_dossier(argv[1]);
    return 0;
}
