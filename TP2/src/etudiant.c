#include <stdio.h>

int main(void) {
	const char *noms[5] = {"Dupont", "Martin", "Bernard", "Petit", "Robert"};
	const char *prenoms[5] = {"Marie", "Pierre", "Sophie", "Lucas", "Emma"};
	const char *adresses[5] = {
		"20 Boulevard Niels Bohr, Lyon",
		"22 Boulevard Niels Bohr, Lyon",
		"5 rue des Lilas, Paris",
		"10 avenue Victor Hugo, Lille",
		"8 rue de la Paix, Nantes"
	};
	float notesProgrammation[5] = {16.5f, 14.0f, 12.5f, 15.0f, 17.0f};
	float notesSysteme[5] = {12.1f, 14.1f, 13.5f, 16.0f, 15.5f};

	for (int i = 0; i < 5; i++) {
		printf("Etudiant %d :\n", i + 1);
		printf("Nom : %s\n", noms[i]);
		printf("Prenom : %s\n", prenoms[i]);
		printf("Adresse : %s\n", adresses[i]);
		printf("Note programmation : %.1f\n", notesProgrammation[i]);
		printf("Note systeme : %.1f\n\n", notesSysteme[i]);
	}

	return 0;
}
