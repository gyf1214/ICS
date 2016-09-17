#include <stdio.h>

int odd_ones(unsigned x) {
    x = x ^ (x >> 1);
    x = x ^ (x >> 2);
    x = x ^ (x >> 4);
    x = x ^ (x >> 8);
    x = x ^ (x >> 16);
    return x & 1;
}

int main() {
    int x;
    scanf("%x", &x);
    printf("%d\n", odd_ones(x));
    return 0;
}
