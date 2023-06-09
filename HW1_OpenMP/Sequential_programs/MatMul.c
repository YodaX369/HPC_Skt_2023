
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void zero_init_matrix(double **matrix, size_t N) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = 0.0;
    }
  }
}

void rand_init_matrix(double **matrix, size_t N) {
  srand(time(NULL));

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = rand() / RAND_MAX;
    }
  }
}

double **malloc_matrix(size_t N) {
  double **matrix = (double **)malloc(N * sizeof(double *));

  for (int i = 0; i < N; ++i) {
    matrix[i] = (double *)malloc(N * sizeof(double));
  }

  return matrix;
}

void free_matrix(double **matrix, size_t N) {

  for (int i = 0; i < N; ++i) {
    free(matrix[i]);
  }

  free(matrix);
}

void MatMul_IJN(double **A, double **B, double **C, size_t N) {
#pragma omp for nowait
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int n = 0; n < N; n++) {
        C[i][j] += A[i][n] * B[n][j];
      }
    }
  }

  return;
}

void MatMul_JIN(double **A, double **B, double **C, size_t N) {
#pragma omp for nowait
  for (int j = 0; j < N; j++) {
    for (int i = 0; i < N; i++) {
      for (int n = 0; n < N; n++) {
        C[i][j] += A[i][n] * B[n][j];
      }
    }
  }

  return;
}

void MatMul_NIJ(double **A, double **B, double **C, size_t N) {
#pragma omp for nowait
  for (int n = 0; n < N; n++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        C[i][j] += A[i][n] * B[n][j];
      }
    }
  }

  return;
}

int main() {
  const size_t N = 1000; // size of an array

  double start, end;

  double **A, **B, **C; // matrices

  printf("Starting:\n");

  A = malloc_matrix(N);
  B = malloc_matrix(N);
  C = malloc_matrix(N);

  rand_init_matrix(A, N);
  rand_init_matrix(B, N);
  

  
  /////////////////////// /////////////////////// ijn
    zero_init_matrix(C, N);
    start = clock();
#pragma omp parallel shared(A, B, C)
    { 
        MatMul_IJN(A, B, C, N); 
    }
    end = clock();

    printf("Time elapsed (ijn): %f seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);



    ////////////////////////////////////////////// jin
    
    zero_init_matrix(C, N);
    start = clock();
#pragma omp parallel shared(A, B, C)
    {
        MatMul_JIN(A, B, C, N); 
    }
    end = clock();

    printf("Time elapsed (jin): %f seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);


    //////////////////////////////////////////////  nij
    
    zero_init_matrix(C, N);
    start = clock();
#pragma omp parallel shared(A, B, C)
    { 
        MatMul_NIJ(A, B, C, N); 
    }
    end = clock();

    printf("Time elapsed (nij): %f seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);

    free_matrix(A, N);
    free_matrix(B, N);
    free_matrix(C, N);

    return 0;
}
