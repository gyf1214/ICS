#include <stdio.h>

int A(int x) {
    return !!x;
}

int B(int x) {
    return !!(~x);
}

int C(int x) {
    return !!(x & 0xFF);
}

int D(int x) {
    return !!((~x) & 0xFF000000);
}

int main() {
    int x;
    scanf("%x", &x);
    printf("%d\n%d\n%d\n%d\n", A(x), B(x), C(x), D(x));
    return 0;
}
