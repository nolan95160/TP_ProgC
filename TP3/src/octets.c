#include <stdio.h>

static void afficher_octets(const void *adresse, size_t taille)
{
    const unsigned char *octets = (const unsigned char *)adresse;

    for (size_t i = 0; i < taille; i++)
    {
        printf("%02x ", octets[i]);
    }
    printf("\n");
}

int main(void)
{
    short s = 0x0302;
    int i = 0x04030201;
    long int li = 0x0807060504030201L;
    float f = 3.5f;
    double d = 1.5;
    long double ld = 1.5L;

    printf("Octets de short :\n");
    afficher_octets(&s, sizeof(s));

    printf("Octets de int :\n");
    afficher_octets(&i, sizeof(i));

    printf("Octets de long int :\n");
    afficher_octets(&li, sizeof(li));

    printf("Octets de float :\n");
    afficher_octets(&f, sizeof(f));

    printf("Octets de double :\n");
    afficher_octets(&d, sizeof(d));

    printf("Octets de long double :\n");
    afficher_octets(&ld, sizeof(ld));

    return 0;
}
