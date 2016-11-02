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

char trans1_desc[] = "Ver 1.0";
void trans1(int M, int N, int A[N][M], int B[M][N]) {
    int bm = M / BLOCK, bn = N / BLOCK;
    int i, j, bi, bj;
    for (bi = 0; bi < bn; ++bi) {
        for (bj = 0; bj < bm; ++bj) {
            for (i = bi * BLOCK; i < bi * BLOCK + BLOCK; ++i) {
                for (j = bj * BLOCK; j < bj * BLOCK + BLOCK; ++j) {
                    B[j][i] = A[i][j];
                }
            }
        }
    }
}

void registerFunctions()
{
    registerTransFunction(trans, trans_desc);
    registerTransFunction(trans1, trans1_desc);
}
