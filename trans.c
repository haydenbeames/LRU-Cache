//////////////////////////////////////////
//   Hayden Beames  Net-ID: hbeames7
//   Colby Weber    Net-ID: cweber7
//////////////////////////////////////////

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
    // int temp;
    int t0, t1, t2, t3, t4, t5, t6, t7;
    int i, ii, j, jj;

    // different methods worked better for different dimmentions

    // if dimmentions not even
    if (M != N) {

        // divide first by blocks
        for (i = 0; i < N; i+= BLOCK_SIZE_1) {
            for (j = 0; j < M; j+= BLOCK_SIZE_1) {
                // sub elements in each block
                for (jj = j; (jj < j + BLOCK_SIZE_1) && (jj < M); jj++) {
                    for (ii = i; (ii < i + BLOCK_SIZE_1) && (ii < N); ii++) {
                        B[jj][ii] = A[ii][jj];
                    }
                }
            }
        }
    // if even dimmentions and <= 32
    } else if (M <= 32) {
        for (i = 0; i < (N / BLOCK_SIZE_1); i++) {
            for (j = 0; j < (M / BLOCK_SIZE_1); j++) {
                for (ii = i * BLOCK_SIZE_1; ii < i * BLOCK_SIZE_1 + BLOCK_SIZE_1; ii++) {
                    t0 = A[ii][j * BLOCK_SIZE_1 + 0];
                    t1 = A[ii][j * BLOCK_SIZE_1 + 1];
                    t2 = A[ii][j * BLOCK_SIZE_1 + 2];
                    t3 = A[ii][j * BLOCK_SIZE_1 + 3];
                    t4 = A[ii][j * BLOCK_SIZE_1 + 4];
                    t5 = A[ii][j * BLOCK_SIZE_1 + 5];
                    t6 = A[ii][j * BLOCK_SIZE_1 + 6];
                    t7 = A[ii][j * BLOCK_SIZE_1 + 7];

                    B[j * BLOCK_SIZE_1 + 0][ii] = t0;
                    B[j * BLOCK_SIZE_1 + 1][ii] = t1;
                    B[j * BLOCK_SIZE_1 + 2][ii] = t2;
                    B[j * BLOCK_SIZE_1 + 3][ii] = t3;
                    B[j * BLOCK_SIZE_1 + 4][ii] = t4;
                    B[j * BLOCK_SIZE_1 + 5][ii] = t5;
                    B[j * BLOCK_SIZE_1 + 6][ii] = t6;
                    B[j * BLOCK_SIZE_1 + 7][ii] = t7;
                }
            }
        }
    // if even and larger than 32
    } else {
        for (i = 0; i < (M / BLOCK_SIZE_1); i ++) {
            for (j = 0; j < (N / BLOCK_SIZE_1); j ++) {
                for (ii = 0; ii < 4; ii ++) {
                    B[j * 8 + 0][i * 8 + 0 + ii] = A[i * 8 + ii][j * 8 + 0];
                    B[j * 8 + 1][i * 8 + 0 + ii] = A[i * 8 + ii][j * 8 + 1];
                    B[j * 8 + 2][i * 8 + 0 + ii] = A[i * 8 + ii][j * 8 + 2];
                    B[j * 8 + 3][i * 8 + 0 + ii] = A[i * 8 + ii][j * 8 + 3];
                    B[j * 8 + 0][i * 8 + 4 + ii] = A[i * 8 + ii][j * 8 + 4];
                    B[j * 8 + 1][i * 8 + 4 + ii] = A[i * 8 + ii][j * 8 + 5];
                    B[j * 8 + 2][i * 8 + 4 + ii] = A[i * 8 + ii][j * 8 + 6];
                    B[j * 8 + 3][i * 8 + 4 + ii] = A[i * 8 + ii][j * 8 + 7];
                }

                for (ii = 0; ii < 4; ii ++) {
                    t0 = B[j * 8 + ii][i * 8 + 4];
                    t1 = B[j * 8 + ii][i * 8 + 5];
                    t2 = B[j * 8 + ii][i * 8 + 6];
                    t3 = B[j * 8 + ii][i * 8 + 7];

                    B[j * 8 + ii][i * 8 + 4] = A[i * 8 + 4][j * 8 + ii];
                    B[j * 8 + ii][i * 8 + 5] = A[i * 8 + 5][j * 8 + ii];
                    B[j * 8 + ii][i * 8 + 6] = A[i * 8 + 6][j * 8 + ii];
                    B[j * 8 + ii][i * 8 + 7] = A[i * 8 + 7][j * 8 + ii];

                    B[j * 8 + 4 + ii][i * 8 + 0] = t0;
                    B[j * 8 + 4 + ii][i * 8 + 1] = t1;
                    B[j * 8 + 4 + ii][i * 8 + 2] = t2;
                    B[j * 8 + 4 + ii][i * 8 + 3] = t3;

                    B[j * 8 + 4 + ii][i * 8 + 4] = A[i * 8 + 4][j * 8 + 4 + ii];
                    B[j * 8 + 4 + ii][i * 8 + 5] = A[i * 8 + 5][j * 8 + 4 + ii];
                    B[j * 8 + 4 + ii][i * 8 + 6] = A[i * 8 + 6][j * 8 + 4 + ii];
                    B[j * 8 + 4 + ii][i * 8 + 7] = A[i * 8 + 7][j * 8 + 4 + ii];
                }
            }
        }
    }
    // else {
    //     blocksWide = M / BLOCK_SIZE_2;
    //     blocksHigh = N / BLOCK_SIZE_2;

    //     for (j = 0; j < blocksWide; j++) {
    //         for (i = 0; i < blocksHigh; i++) {
    //             for (ii = i * BLOCK_SIZE_2; ii < i * BLOCK_SIZE_2 + BLOCK_SIZE_2; ii++) {
    //                 t0 = A[ii][j * BLOCK_SIZE_2 + 0];
    //                 t1 = A[ii][j * BLOCK_SIZE_2 + 1];
    //                 t2 = A[ii][j * BLOCK_SIZE_2 + 2];
    //                 t3 = A[ii][j * BLOCK_SIZE_2 + 3];

    //                 B[j * BLOCK_SIZE_2 + 0][ii] = t0;
    //                 B[j * BLOCK_SIZE_2 + 1][ii] = t1;
    //                 B[j * BLOCK_SIZE_2 + 2][ii] = t2;
    //                 B[j * BLOCK_SIZE_2 + 3][ii] = t3;
    //             }
    //         }
    //     }
    // }
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

