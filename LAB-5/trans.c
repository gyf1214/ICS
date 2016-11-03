#include <stdio.h>
#include "cachelab.h"

char transpose_submit_desc[] = "Transpose submission";

char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

#define BLOCK 8

void transBlock(int M, int N, int A[N][M], int B[M][N], int bi, int bj, int block, int offset) {
    int i, j;
    for (i = bi; i < bi + block; ++i) {
        for (j = bj; j < bj + block; ++j) {
            B[j][i] = A[(i + offset) % N][(j + offset) % M];
        }
    }
}

char trans1_desc[] = "Ver 1.0";
void trans1(int M, int N, int A[N][M], int B[M][N]) {
    int i, j;
    for (i = 0; i < N; i += BLOCK) {
        for (j = 0; j < M; j += BLOCK) {
            if (i != j) {
                transBlock(M, N, A, B, i, j, BLOCK, 0);
            } else {
                transBlock(M, N, A, B, i, j, BLOCK, 0);
            }
        }
    }
}

void registerFunctions()
{
    registerTransFunction(trans, trans_desc);
    registerTransFunction(trans1, trans1_desc);
}
