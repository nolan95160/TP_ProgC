#include <stdio.h>

int main() {

    char c = 'A';
    signed char sc = -100;
    unsigned char uc = 200;
    printf("char               : %c\n", c);
    printf("signed char        : %d\n", sc);
    printf("unsigned char      : %u\n", uc);

    short s = -30000;
    unsigned short us = 60000;
    printf("short              : %hd\n", s);
    printf("unsigned short     : %hu\n", us);

    int i = -100000;
    unsigned int ui = 3000000000U;
    printf("int                : %d\n", i);
    printf("unsigned int       : %u\n", ui);

    long int li = 1234567890L;
    unsigned long int uli = 4000000000UL;
    printf("long int           : %ld\n", li);
    printf("unsigned long int  : %lu\n", uli);

    long long int lli = 9000000000000LL;
    unsigned long long int ulli = 18000000000000000ULL;
    printf("long long int      : %lld\n", lli);
    printf("unsigned long long : %llu\n", ulli);

    float f = 3.14f;
    double d = 3.141592653589;
    long double ld = 3.14159265358979L;
    printf("float              : %f\n", f);
    printf("double             : %lf\n", d);
    printf("long double        : %Lf\n", ld);

    return 0;
}