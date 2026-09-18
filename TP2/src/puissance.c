#include <stdio.h>

int main() {
    int a = 2;
    int b = 3;
    int r = 1;
    for (int i = 0; i < b; i++) {
        r *= a;
    }
    printf("%d\n", r);
    return 0;
}
