/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include "cachelab.h"
#include <stdio.h>

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_32(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, m, n;
  int x[8];
  for (i = 0; i < N; i += 8) {
    for (j = 0; j < M; j += 8) {
      if (i != j) {
        for (m = i; m < i + 8; ++m) {
          for (n = 0; n < 8; ++n) {
            x[n] = A[m][n + j];
          }
          for (n = 0; n < 8; ++n) {
            B[n + j][m] = x[n];
          }
        }
      } else {
        for (m = i; m < i + 8; ++m) {
          for (n = 0; n < 8; ++n) {
            x[n] = A[m][n + j];
          }
          for (n = 0; n < 8; ++n) {
            B[m][n + j] = x[n];
          }
        }
        for (m = 0; m < 8; ++m) {
          x[m] = B[m + i][m + j];
        }
        for (m = 0; m < 8; ++m) {
          for (n = m + 1; n < 8; ++n) {
            B[m + i][m + i] = B[m + i][n + j];
            B[m + i][n + j] = B[n + j][m + i];
            B[n + j][m + i] = B[m + i][m + i];
          }
        }
        for (m = 0; m < 8; ++m) {
          B[m + i][m + j] = x[m];
        }
      }
    }
  }
}
void transpose_64(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, m, n;
  int x[8];
  for (i = 0; i < N; i += 8) {
    for (j = 0; j < M; j += 8) {
      for (n = i; n < i + 4; ++n) {
        for (m = 0; m < 8; ++m) {
          x[m] = A[n][j + m];
        }
        for (m = 0; m < 4; ++m) {
          B[j + m][n] = x[m];
        }
        for (m = 0; m < 4; ++m) {
          B[j + m][n + 4] = x[m + 4];
        }
      }
      for (m = j; m < j + 4; ++m) {
        for (n = 0; n < 4; ++n) {
          x[n] = A[i + 4 + n][m];
        }
        for (n = 0; n < 4; ++n) {
          x[n + 4] = B[m][i + 4 + n];
        }
        for (n = 0; n < 4; ++n) {
          B[m][i + 4 + n] = x[n];
        }
        for (n = 0; n < 4; ++n) {
          B[m + 4][i + n] = x[n + 4];
        }
      }
      for (n = i + 4; n < i + 8; ++n) {
        for (m = 0; m < 4; ++m) {
          x[m] = A[n][j + 4 + m];
        }
        for (m = 0; m < 4; ++m) {
          B[j + 4 + m][n] = x[m];
        }
      }
    }
  }
}
void transpose(int M, int N, int A[N][M], int B[M][N]) {
  int i, j;
  int x[8];
  int n, m;
  for (j = 0; j < 8 * (int)(M / 8); j += 8) {
    for (i = 0; i < 8 * (int)(N / 8); i += 8) {
      for (n = i; n < i + 8; ++n) {
        for (m = 0; m < 8; ++m) {
          x[m] = A[n][j + m];
        }
        for (m = 0; m < 8; ++m) {
          B[j + m][n] = x[m];
        }
      }
    }
  }
  for (i = 8 * (int)(N / 8); i < N; ++i)
    for (j = 8 * (int)(M / 8); j < M; ++j) {
      x[0] = A[i][j];
      B[j][i] = x[0];
    }
  for (i = 0; i < N; ++i)
    for (j = 8 * (int)(M / 8); j < M; ++j) {
      x[0] = A[i][j];
      B[j][i] = x[0];
    }
  for (i = 8 * (int)(N / 8); i < N; ++i)
    for (j = 0; j < M; ++j) {
      x[0] = A[i][j];
      B[j][i] = x[0];
    }
}
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {

  if (N == 32 && M == 32) {
    transpose_32(M, N, A, B);
  } else if (N == 64 && M == 64) {
    transpose_64(M, N, A, B);
  } else {
    transpose(M, N, A, B);
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
void trans(int M, int N, int A[N][M], int B[M][N]) {
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
void registerFunctions() {
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
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
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
