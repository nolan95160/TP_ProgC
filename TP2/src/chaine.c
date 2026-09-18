#include <stdio.h>

int longueur(const char *chaine) {
	int longueur = 0;

	while (chaine[longueur] != '\0') {
		longueur++;
	}
	return longueur;
}

void copier(char *destination, const char *source) {
	int i = 0;

	while (source[i] != '\0') {
		destination[i] = source[i];
		i++;
	}
	destination[i] = '\0';
}

void concatener(char *destination, const char *source) {
	int position = longueur(destination);
	int i = 0;

	while (source[i] != '\0') {
		destination[position + i] = source[i];
		i++;
	}
	destination[position + i] = '\0';
}

int main(void) {
	char premiere[200];
	char seconde[200];
	char copie[200];
	char resultat[400];

	printf("Premiere chaine : ");
	if (fgets(premiere, sizeof premiere, stdin) == NULL) {
		return 1;
	}
	printf("Deuxieme chaine : ");
	if (fgets(seconde, sizeof seconde, stdin) == NULL) {
		return 1;
	}

	if (premiere[longueur(premiere) - 1] == '\n') {
		premiere[longueur(premiere) - 1] = '\0';
	}
	if (seconde[longueur(seconde) - 1] == '\n') {
		seconde[longueur(seconde) - 1] = '\0';
	}

	copier(copie, premiere);
	copier(resultat, premiere);
	concatener(resultat, seconde);

	printf("Longueur de la premiere chaine : %d\n", longueur(premiere));
	printf("Copie : %s\n", copie);
	printf("Concatenation : %s\n", resultat);
	return 0;
}
