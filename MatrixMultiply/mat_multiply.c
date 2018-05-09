#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4


void fill_matrix(float *M, int row, int col){
  float aux = 5.0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      M[i * col + j] = (float)rand() / (float)(RAND_MAX / aux);
    }
  }
}

void print(float *M, int row, int col){
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%.2f ", M[i * col + j]);
    }
    printf("\n");
  }
  printf("\n");
}

void multiply(float *matrix_a, float *matrix_b, float *result, int col_a, int col_b, int row_a, int row_b){

  int counter;

  if (col_a != row_b){
    printf("Imposible multiplicar estas matrices\n");
    exit(-1);
  }

  for (int i = 0; i < col_a; ++i){
    for (int j = 0; j < row_b; ++j){
      counter = 0;
      for (int k = 0; k < row_b; ++k){
        counter += matrix_a[i * col_a + k] * matrix_b[k * col_b + j];
      }
      result[i * col_b + j] = counter;
    }
  }
}

void save(float *M, int row, int col){
  FILE *f = fopen("result_mat_multiply.csv", "a");

  if (f == NULL){
    printf("File error\n");
    exit(-1);
  }

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; ++j){
      if(col - 1 == j){
        fprintf(f, "%.2f", M[i * col + j]);
      }
      else{
        fprintf(f, "%.2f, ", M[i * col + j]);
      }
    }
     fprintf(f, "\n");
  }

  fprintf(f, "\n");
  fclose(f);

  return;
}

int main() {
  int row_A = N, row_B = N, col_A = N, col_B = N;
  clock_t start, end;

  /*printf("Digite el numero de filas matriz A: \n");
  scanf("%d", &row_A);

  printf("Digite el numero de columnas matriz A: \n");
  scanf("%d", &col_A);

  printf("Digite el numero de filas matriz B: \n");
  scanf("%d", &row_B);

  printf("Digite el numero de columnas matriz B: \n");
  scanf("%d", &col_B);
  */

  float *mat_A = (float*)malloc(row_A * col_A * sizeof(float));
  float *mat_B = (float*)malloc(row_B * col_B * sizeof(float));
  float *result = (float*)malloc(row_A * col_B * sizeof(float));

  fill_matrix(mat_A, row_A, col_A);
  fill_matrix(mat_B, row_B, col_B);

  start = clock();
  multiply(mat_A, mat_B, result, col_A, col_B, row_A, row_B);

  /*
  print(mat_A, row_A, col_A);
  print(mat_B, row_B, col_B);
  print(result, row_A, col_B);
  */
  end = clock();
  printf("Tiempo sin OMP: %.6f\n", (double)(end - start)/CLOCKS_PER_SEC);
  save(mat_A, row_A, col_A);
  save(mat_B, row_B, col_B);
  save(result, row_A, col_B);

  free(mat_A);
  free(mat_B);
  free(result);
  return 0;
}
