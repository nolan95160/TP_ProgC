#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE 11

void afficher_entiers(const int *tableau) {
	for (const int *curseur = tableau; curseur < tableau + TAILLE; curseur++) {
		printf("%d", *curseur);
		if (curseur < tableau + TAILLE - 1) {
			printf(", ");
		}
	}
	printf("\n");
}

void afficher_flottants(const float *tableau) {
	for (const float *curseur = tableau; curseur < tableau + TAILLE; curseur++) {
		printf("%.2f", *curseur);
		if (curseur < tableau + TAILLE - 1) {
			printf(", ");
		}
	}
	printf("\n");
}

int main(void) {
	int entiers[TAILLE];
	float flottants[TAILLE];
	int indice = 0;

	srand((unsigned int)time(NULL));
	for (int *curseur = entiers; curseur < entiers + TAILLE; curseur++) {
		*curseur = rand() % 100;
	}
	for (float *curseur = flottants; curseur < flottants + TAILLE; curseur++) {
		*curseur = (float)(rand() % 100) / 10.0f;
	}

	printf("Tableau d'entiers avant :\n");
	afficher_entiers(entiers);
	printf("Tableau de flottants avant :\n");
	afficher_flottants(flottants);

	for (int *curseur = entiers; curseur < entiers + TAILLE; curseur++) {
		if (indice % 2 == 0) {
			*curseur *= 3;
		}
		indice++;
	}

	indice = 0;
	for (float *curseur = flottants; curseur < flottants + TAILLE; curseur++) {
		if (indice % 2 == 0) {
			*curseur *= 3;
		}
		indice++;
	}

	printf("Tableau d'entiers apres :\n");
	afficher_entiers(entiers);
	printf("Tableau de flottants apres :\n");
	afficher_flottants(flottants);

	return 0;
}
