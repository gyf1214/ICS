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
#define repBLOCK(bi, bj) for (i = (bi); i < (bi) + BLOCK; ++i) for (j = (bj); j < (bj) + BLOCK; ++j)
#define iteBLOCK(i, bi, N) for (i = (bi) * BLOCK; i < (N); i += BLOCK)
#define repHALF(bi, bj) for (i = (bi); i < (bi) + HALF; ++i) for (j = (bj); j < (bj) + HALF; ++j)
#define transQUAD(b1, b2, t1, t2) repHALF(0, 0) B[j + (t1)][i + (t2)] = A[i + (b1)][j + (b2)]
#define copyQUAD(b1, b2, t1, t2) repHALF(0, 0) B[j + (t1)][i + (t2)] = B[j + (b1)][i + (b2)]

void transBlock(int M, int N, int A[N][M], int B[M][N], int bi, int bj) {
    int i, j;
    repBLOCK(bi, bj) B[j][i] = A[i][j];
}

char desc32[] = "For 32x32";
void trans32(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, k;
    iteBLOCK(k, 1, N) {
        repBLOCK(0, 0) B[j][i] = A[i + k][j + k];
        repBLOCK(0, 0) B[i + k][j + k] = B[i][j];
    }
    repBLOCK(0, 0) B[j][i] = A[i][j];
    iteBLOCK(i, 0, N) iteBLOCK(j, 0, M) if (i != j) {
        transBlock(M, N, A, B, i, j);
    }
}

void transHalf(int M, int N, int A[N][M], int B[M][N], int bi, int bj, int t) {
    int i, j;
    transQUAD(bi, bj, bj, bi);
    transQUAD(bi, bj + HALF, t, t);
    transQUAD(bi + HALF, bj, bj, bi + HALF);
    copyQUAD(t, t, bj + HALF, bi);
    transQUAD(bi + HALF, bj + HALF, bj + HALF, bi + HALF);
}

void transFullHalf(int M, int N, int A[N][M], int B[M][N], int b, int t1, int t2, int t) {
    int i, j;
    repHALF(0, 0) B[t1 + j][t2 + i] = A[i + b][j + b];
    repHALF(0, 0) B[t1 + j][t2 + i + HALF] = A[i + b][j + b + HALF];
}

char desc64[] = "For 64x64";
void trans64(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, k;
    iteBLOCK(i, 1, N) iteBLOCK(j, 1, M) if (i != j) transHalf(M, N, A, B, i, j, 0);
    iteBLOCK(i, 2, N) transHalf(M, N, A, B, i, 0, BLOCK);
    iteBLOCK(j, 2, M) transHalf(M, N, A, B, 0, j, BLOCK);
    transHalf(M, N, A, B, BLOCK, 0, 2 * BLOCK);
    transHalf(M, N, A, B, 0, BLOCK, 2 * BLOCK);
    iteBLOCK(k, 0, N) {
        repBLOCK(k, k) B[j][i] = A[i][j];
    }
}

void registerFunctions()
{
    registerTransFunction(trans64, desc64);
    registerTransFunction(trans, trans_desc);
    registerTransFunction(trans32, desc32);
}
