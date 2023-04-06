/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

#define BLOCK_SIZE_1 8
#define BLOCK_SIZE_2 4

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int blocksWide;
    int blocksHigh;
    // int temp;
    int k0, k1, k2, k3, k4, k5, k6, k7;

    if (M != N) {
        blocksWide = M / BLOCK_SIZE_1;
        blocksHigh = N / BLOCK_SIZE_1;

        for (int i = 0; i < N; i+= BLOCK_SIZE_1) {
            for (int j = 0; j < M; j+= BLOCK_SIZE_1) {
                for (int jj = j; (jj < j + BLOCK_SIZE_1) && (jj < M); jj++) {
                    for (int ii = i; (ii < i + BLOCK_SIZE_1) && (ii < N); ii++) {
                        B[jj][ii] = A[ii][jj];
                    }
                }
            }
        }
    } else if (M <= 32) {
        blocksWide = M / BLOCK_SIZE_1;
        blocksHigh = N / BLOCK_SIZE_1;

        for (int i = 0; i < blocksHigh; i++) {
            for (int j = 0; j < blocksWide; j++) {
                for (int ii = i * BLOCK_SIZE_1; ii < i * BLOCK_SIZE_1 + BLOCK_SIZE_1; ii++) {
                    k0 = A[ii][j * BLOCK_SIZE_1 + 0];
                    k1 = A[ii][j * BLOCK_SIZE_1 + 1];
                    k2 = A[ii][j * BLOCK_SIZE_1 + 2];
                    k3 = A[ii][j * BLOCK_SIZE_1 + 3];
                    k4 = A[ii][j * BLOCK_SIZE_1 + 4];
                    k5 = A[ii][j * BLOCK_SIZE_1 + 5];
                    k6 = A[ii][j * BLOCK_SIZE_1 + 6];
                    k7 = A[ii][j * BLOCK_SIZE_1 + 7];

                    B[j * BLOCK_SIZE_1 + 0][ii] = k0;
                    B[j * BLOCK_SIZE_1 + 1][ii] = k1;
                    B[j * BLOCK_SIZE_1 + 2][ii] = k2;
                    B[j * BLOCK_SIZE_1 + 3][ii] = k3;
                    B[j * BLOCK_SIZE_1 + 4][ii] = k4;
                    B[j * BLOCK_SIZE_1 + 5][ii] = k5;
                    B[j * BLOCK_SIZE_1 + 6][ii] = k6;
                    B[j * BLOCK_SIZE_1 + 7][ii] = k7;
                }
            }
        }
    } else {
        blocksWide = M / BLOCK_SIZE_2;
        blocksHigh = N / BLOCK_SIZE_2;

        for (int j = 0; j < blocksWide; j++) {
            for (int i = 0; i < blocksHigh; i++) {
                for (int ii = i * BLOCK_SIZE_2; ii < i * BLOCK_SIZE_2 + BLOCK_SIZE_2; ii++) {
                    k0 = A[ii][j * BLOCK_SIZE_2 + 0];
                    k1 = A[ii][j * BLOCK_SIZE_2 + 1];
                    k2 = A[ii][j * BLOCK_SIZE_2 + 2];
                    k3 = A[ii][j * BLOCK_SIZE_2 + 3];

                    B[j * BLOCK_SIZE_2 + 0][ii] = k0;
                    B[j * BLOCK_SIZE_2 + 1][ii] = k1;
                    B[j * BLOCK_SIZE_2 + 2][ii] = k2;
                    B[j * BLOCK_SIZE_2 + 3][ii] = k3;
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

