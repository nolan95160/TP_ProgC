#include <stdio.h>

int main() {
    int nombres[] = {0, 4096, 65536, 65535, 1024};

    for (int i = 0; i < 5; i++) {
        int n = nombres[i];
        printf("%7d : ", n);

        if (n == 0) {
            printf("0\n");
            continue;
        }

        int premier_un_trouve = 0;
        
        for (int j = 31; j >= 0; j--) {
            int bit = (n >> j) & 1;
            
            if (bit == 1) {
                premier_un_trouve = 1;
            }
            
            if (premier_un_trouve == 1) {
                printf("%d", bit);
            }
        }
        printf("\n");
    }

    return 0;
}