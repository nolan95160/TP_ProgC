#include <stdio.h>
#include <string.h>

struct Etudiant {
	char nom[30];
	char prenom[30];
	char adresse[100];
	float noteProgrammation;
	float noteSysteme;
};

int main(void) {
	struct Etudiant etudiants[5];

	strcpy(etudiants[0].nom, "Dupont");
	strcpy(etudiants[0].prenom, "Marie");
	strcpy(etudiants[0].adresse, "20 Boulevard Niels Bohr, Lyon");
	etudiants[0].noteProgrammation = 16.5f;
	etudiants[0].noteSysteme = 12.1f;

	strcpy(etudiants[1].nom, "Martin");
	strcpy(etudiants[1].prenom, "Pierre");
	strcpy(etudiants[1].adresse, "22 Boulevard Niels Bohr, Lyon");
	etudiants[1].noteProgrammation = 14.0f;
	etudiants[1].noteSysteme = 14.1f;

	strcpy(etudiants[2].nom, "Bernard");
	strcpy(etudiants[2].prenom, "Sophie");
	strcpy(etudiants[2].adresse, "5 rue des Lilas, Paris");
	etudiants[2].noteProgrammation = 12.5f;
	etudiants[2].noteSysteme = 13.5f;

	strcpy(etudiants[3].nom, "Petit");
	strcpy(etudiants[3].prenom, "Lucas");
	strcpy(etudiants[3].adresse, "10 avenue Victor Hugo, Lille");
	etudiants[3].noteProgrammation = 15.0f;
	etudiants[3].noteSysteme = 16.0f;

	strcpy(etudiants[4].nom, "Robert");
	strcpy(etudiants[4].prenom, "Emma");
	strcpy(etudiants[4].adresse, "8 rue de la Paix, Nantes");
	etudiants[4].noteProgrammation = 17.0f;
	etudiants[4].noteSysteme = 15.5f;

	for (int i = 0; i < 5; i++) {
		printf("Etudiant %d :\n", i + 1);
		printf("Nom : %s\n", etudiants[i].nom);
		printf("Prenom : %s\n", etudiants[i].prenom);
		printf("Adresse : %s\n", etudiants[i].adresse);
		printf("Note programmation : %.1f\n", etudiants[i].noteProgrammation);
		printf("Note systeme : %.1f\n\n", etudiants[i].noteSysteme);
	}

	return 0;
}
