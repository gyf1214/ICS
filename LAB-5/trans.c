#include <stdio.h>
#include "cachelab.h"

char transpose_submit_desc[] = "Transpose submission";

#define BLOCK (8)
#define HALF (BLOCK / 2)
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
    t *= BLOCK;
    transQUAD(bi, bj, bj, bi);
    transQUAD(bi, bj + HALF, t, t);
    transQUAD(bi + HALF, bj, bj, bi + HALF);
    copyQUAD(t, t, bj + HALF, bi);
    transQUAD(bi + HALF, bj + HALF, bj + HALF, bi + HALF);
}

void transFull(int M, int N, int A[N][M], int B[M][N], int b, int t1, int t2, int t) {
    int i, j;
    t1 *= BLOCK;
    t2 *= BLOCK;
    t *= BLOCK;
    transQUAD(b, b, t1, t2);
    transQUAD(b, b + HALF, t1, t2 + HALF);
    transQUAD(b + HALF, b, t, t);
    transQUAD(b + HALF, b + HALF, t, t + HALF);
    copyQUAD(t1, t2, b, b);
    copyQUAD(t, t, b, b + HALF);
    copyQUAD(t1, t2 + HALF, b + HALF, b);
    copyQUAD(t, t + HALF, b + HALF, b + HALF);
}

char desc64[] = "For 64x64";
void trans64(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, k;
    iteBLOCK(i, 1, N) iteBLOCK(j, 1, M) if (i != j) transHalf(M, N, A, B, i, j, 0);
    iteBLOCK(k, 3, N) transFull(M, N, A, B, k, 0, 0, 1);
    iteBLOCK(i, 2, N) transHalf(M, N, A, B, i, 0, 1);
    iteBLOCK(j, 2, N) transHalf(M, N, A, B, 0, j, 1);
    transFull(M, N, A, B, 0, 1, 1, 2);
    transFull(M, N, A, B, BLOCK, 1, 0, 2);
    transHalf(M, N, A, B, BLOCK, 0, 2);
    transHalf(M, N, A, B, 0, BLOCK, 2);
    repBLOCK(BLOCK * 2, BLOCK * 2) B[j][i] = A[i][j];
}

#define BLOCK0 16
#define safeRead(a, i, j) if ((i) < N && (j) < M) a = A[(i)][(j)]
#define safeWrite(a, i, j) if ((i) < N && (j) < M) B[(j)][(i)] = a

char desc61[] = "For 61x67";
void trans61(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, k, a0, a1, a2, a3, a4, a5, a6, a7;
    for (i = 0; i < N; i += BLOCK0) {
        for (j = 0; j < M; j += HALF) {
            for (k = i; k < i + BLOCK0 && k < N; k += 8 / HALF) {
                safeRead(a0, k, j);
                safeRead(a1, k, j + 1);
                safeRead(a2, k, j + 2);
                safeRead(a3, k, j + 3);
                safeRead(a4, k + 1, j);
                safeRead(a5, k + 1, j + 1);
                safeRead(a6, k + 1, j + 2);
                safeRead(a7, k + 1, j + 3);
                safeWrite(a0, k, j);
                safeWrite(a1, k, j + 1);
                safeWrite(a2, k, j + 2);
                safeWrite(a3, k, j + 3);
                safeWrite(a4, k + 1, j);
                safeWrite(a5, k + 1, j + 1);
                safeWrite(a6, k + 1, j + 2);
                safeWrite(a7, k + 1, j + 3);
            }
        }
    }
}

void transMain(int M, int N, int A[N][M], int B[M][N]) {
    if (M == 32) {
        trans32(M, N, A, B);
    } else if (M == 64) {
        trans64(M, N, A, B);
    } else {
        trans61(M, N, A, B);
    }
}

void registerFunctions()
{
    registerTransFunction(transMain, transpose_submit_desc);
}
