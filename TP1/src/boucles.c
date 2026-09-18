#include <stdio.h>

int main() {
    int compteur = 5;

    if (compteur >= 10) {
        printf("Erreur : le compteur doit etre strictement inferieur a 10.\n");
        return 1; 
    }

    printf("--- Version avec boucles FOR ---\n");
    
    for (int ligne = 1; ligne <= compteur; ligne++) {
        for (int colonne = 1; colonne <= ligne; colonne++) {
            if (colonne == 1 || colonne == ligne || ligne == compteur) {
                printf("* ");
            } else {
                printf("# ");
            }
        }
        printf("\n");
    }

    printf("\n--- Version avec boucles WHILE ---\n");
    
    int i = 1;
    while (i <= compteur) {
        int j = 1;
        
        while (1) { 
            if (j > i) {
                break; 
            }
            
            if (j == 1 || j == i || i == compteur) {
                printf("* ");
            } else {
                printf("# ");
            }
            j++;
        }
        printf("\n");
        i++;
    }

    return 0;
}