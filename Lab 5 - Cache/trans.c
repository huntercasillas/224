/* 
 * Hunter Casillas
 * NetID: casillas
 *
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
    int i;
    int j;
    int k;
    int l;
    int one;
    int two;
    int three;
    int four;
    int five;
    int six;
    int seven;
    int eight;

    if (M == 32 && N == 32) {
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                for (k = i; k < i + 8; k++) {
                    for (l = j; l < j + 8; l += 8) {
                        one = A[k][l];
                        two = A[k][l + 1];
                        three = A[k][l + 2];
                        four = A[k][l + 3];
                        five = A[k][l + 4];
                        six = A[k][l + 5];
                        seven = A[k][l + 6];
                        eight = A[k][l + 7];

                        B[l][k] = one;
                        B[l + 1][k] = two;
                        B[l + 2][k] = three;
                        B[l + 3][k] = four;
                        B[l + 4][k] = five;
                        B[l + 5][k] = six;
                        B[l + 6][k] = seven;
                        B[l + 7][k] = eight;
                    }
                }
            }
        }
    }
    else if (M == 64 && N == 64) {
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                for (k = i; k < i + 4; k++) {
                    one = A[k][j];
                    two = A[k][j + 1];
                    three = A[k][j + 2];
                    four = A[k][j + 3];
                    five = A[k][j + 4];
                    six = A[k][j + 5];
                    seven = A[k][j + 6];
                    eight = A[k][j + 7];

                    B[j][k] = one;
                    B[j + 1][k] = two;
                    B[j + 2][k] = three;
                    B[j + 3][k] = four;
                    B[j][k + 4] = five;
                    B[j + 1][k + 4] = six;
                    B[j + 2][k + 4] = seven;
                    B[j + 3][k + 4] = eight;
                }
                for (l = j + 4; l < j + 8; l++) {
                    five = A[i + 4][l - 4];
                    six = A[i + 5][l - 4];
                    seven = A[i + 6][l - 4];
                    eight = A[i + 7][l - 4];
                    one = B[l - 4][i + 4];
                    two = B[l - 4][i + 5];
                    three = B[l - 4][i + 6];
                    four = B[l - 4][i + 7];

                    B[l - 4][i + 4] = five;
                    B[l - 4][i + 5] = six;
                    B[l - 4][i + 6] = seven;
                    B[l - 4][i + 7] = eight;
                    B[l][i] = one;
                    B[l][i + 1] = two;
                    B[l][i + 2] = three;
                    B[l][i + 3] = four;

                    B[l][i + 4] = A[i + 4][l];
                    B[l][i + 5] = A[i + 5][l];
                    B[l][i + 6] = A[i + 6][l];
                    B[l][i + 7] = A[i + 7][l];
                }
            }
        }
    }
    else if (M == 61 && N == 67) {
        one = 23;
        for (i = 0; i < N; i += one) {
            for (j = 0;  j < M; j += one) {
                for (k = i; k < i + one && k < N; k++) {
                    for (l = j; l < j + one && l < M; l++) {
                        two = A[k][l];
                        B[l][k] = two;
                    }
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

