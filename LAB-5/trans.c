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
#define HALF BLOCK / 2
#define repBLOCK(i, j, bi, bj) for (i = bi; i < bi + BLOCK; ++i) for (j = bj; j < bj + BLOCK; ++j)
#define iteBLOCK(i, bi, N) for (i = bi * BLOCK; i < N; i += BLOCK)
#define repHALF(i, j, bi, bj) for (i = bi; i < bi + HALF; ++i) for (j = bj; j < bj + HALF; ++j)

void transBlock(int M, int N, int A[N][M], int B[M][N], int bi, int bj) {
    int i, j;
    repBLOCK(i, j, bi, bj) B[j][i] = A[i][j];
}

char desc32[] = "For 32x32";
void trans32(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, k;
    iteBLOCK(k, 1, N) {
        repBLOCK(i, j, 0, 0) B[j][i] = A[i + k][j + k];
        repBLOCK(i, j, 0, 0) B[i + k][j + k] = B[i][j];
    }
    repBLOCK(i, j, 0, 0) B[j][i] = A[i][j];
    iteBLOCK(i, 0, N) iteBLOCK(j, 0, M) if (i != j) {
        transBlock(M, N, A, B, i, j);
    }
}

void transHalf(int M, int N, int A[N][M], int B[M][N], int bi, int bj, int t) {
    int i, j;
    repHALF(i, j, bi, bj) B[j][i] = A[i][j];
    repHALF(i, j, 0, 0) B[t + j][t + i] = A[i + bi][j + bj + HALF];
    repHALF(i, j, bi + HALF / 2, bj) B[j][i] = A[i][j];
    repHALF(i, j, 0, 0) B[j + bj + HALF][i + bi] = B[t + j][t + i];
    repHALF(i, j, bi + HALF / 2, bj + HALF / 2) B[j][i] = A[i][j];
}

char desc64[] = "For 64x64";
void trans64(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, k;
    iteBLOCK(i, 1, N) iteBLOCK(j, 1, M) if (i != j) transHalf(M, N, A, B, i, j, 0);
    iteBLOCK(i, 2, N) transHalf(M, N, A, B, i, 0, 1);
    iteBLOCK(j, 2, N) transHalf(M, N, A, B, 0, j, 1);
    transHalf(M, N, A, B, 1, 0, 2);
    transHalf(M, N, A, B, 0, 1, 2);
    iteBLOCK(k, 0, N) {
        repBLOCK(i, j, k, k) B[j][i] = A[i][j];
    }
}

void registerFunctions()
{
    registerTransFunction(trans, trans_desc);
    registerTransFunction(trans32, desc32);
    registerTransFunction(trans64, desc64);
}
