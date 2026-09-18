#include <stdio.h>

int main() {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 0;
    }
    int t1 = 0, t2 = 1, next;
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%d", t1);
        next = t1 + t2;
        t1 = t2;
        t2 = next;
    }
    printf("\n");
    return 0;
}
