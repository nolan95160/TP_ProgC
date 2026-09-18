#include <stdio.h>

void afficher_octets(const void *adresse, size_t taille) {
	const unsigned char *octets = adresse;

	for (size_t i = 0; i < taille; i++) {
		printf("%02x", octets[i]);
	}
}

int main(void) {
	char c = 'A';
	short s = 12;
	int i = 34;
	long int li = 56;
	long long int lli = 78;
	float f = 1.5f;
	double d = 2.5;
	long double ld = 3.5L;

	char *pc = &c;
	short *ps = &s;
	int *pi = &i;
	long int *pli = &li;
	long long int *plli = &lli;
	float *pf = &f;
	double *pd = &d;
	long double *pld = &ld;

	printf("Avant la manipulation :\n");
	printf("Adresse de c : %p, Valeur : ", (void *)pc);
	afficher_octets(pc, sizeof c);
	printf("\nAdresse de s : %p, Valeur : ", (void *)ps);
	afficher_octets(ps, sizeof s);
	printf("\nAdresse de i : %p, Valeur : ", (void *)pi);
	afficher_octets(pi, sizeof i);
	printf("\nAdresse de li : %p, Valeur : ", (void *)pli);
	afficher_octets(pli, sizeof li);
	printf("\nAdresse de lli : %p, Valeur : ", (void *)plli);
	afficher_octets(plli, sizeof lli);
	printf("\nAdresse de f : %p, Valeur : ", (void *)pf);
	afficher_octets(pf, sizeof f);
	printf("\nAdresse de d : %p, Valeur : ", (void *)pd);
	afficher_octets(pd, sizeof d);
	printf("\nAdresse de ld : %p, Valeur : ", (void *)pld);
	afficher_octets(pld, sizeof ld);
	printf("\n\n");

	*pc = 'B';
	*ps = 13;
	*pi = 35;
	*pli = 57;
	*plli = 79;
	*pf = 2.5f;
	*pd = 3.5;
	*pld = 4.5L;

	printf("Apres la manipulation :\n");
	printf("Adresse de c : %p, Valeur : ", (void *)pc);
	afficher_octets(pc, sizeof c);
	printf("\nAdresse de s : %p, Valeur : ", (void *)ps);
	afficher_octets(ps, sizeof s);
	printf("\nAdresse de i : %p, Valeur : ", (void *)pi);
	afficher_octets(pi, sizeof i);
	printf("\nAdresse de li : %p, Valeur : ", (void *)pli);
	afficher_octets(pli, sizeof li);
	printf("\nAdresse de lli : %p, Valeur : ", (void *)plli);
	afficher_octets(plli, sizeof lli);
	printf("\nAdresse de f : %p, Valeur : ", (void *)pf);
	afficher_octets(pf, sizeof f);
	printf("\nAdresse de d : %p, Valeur : ", (void *)pd);
	afficher_octets(pd, sizeof d);
	printf("\nAdresse de ld : %p, Valeur : ", (void *)pld);
	afficher_octets(pld, sizeof ld);
	printf("\n");

	return 0;
}
