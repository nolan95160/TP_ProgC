#include <stdio.h>

int main() {
    int d = 0x10001000;
    int b4 = (d >> 28) & 1;
    int b20 = (d >> 12) & 1;
    if (b4 && b20) {
        printf("1\n");
    } else {
        printf("0\n");
    }
    return 0;
}
