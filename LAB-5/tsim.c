#include <stdio.h>
#include "cachelab.h"

#define SIZE 31
#define rep(i) for (i = 0; i < SIZE; ++i)

void trans61(int M, int N, int A[N][M], int B[M][N]);

int A[SIZE][SIZE], B[SIZE][SIZE];

void registerTransFunction(void (*trans)(int M,int N,int[N][M],int[M][N]),
                           char* desc) {

                           }

void print(int A[][SIZE]) {
    int i, j;
    rep(i) rep(j) {
        printf("%d%c", A[i][j], j == SIZE - 1 ? '\n' : '\t');
    }
}

int main() {
    int i, j;
    rep(i) rep(j) {
        A[i][j] = (i * SIZE + j) % 1000;
    }
    print(A);
    trans61(SIZE, SIZE, A, B);
    print(B);

    return 0;
}
