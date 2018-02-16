#include <stdio.h>
#include <stdlib.h>

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


int main() {
  int mat_col_1, mat_row_1, mat_row_2, mat_col_2;

  printf("Digite el numero de columnas matriz 1: \n");
  scanf("%d", &mat_col_1);

  printf("Digite el numero de filas matriz 1: \n");
  scanf("%d", &mat_row_1);

  printf("Digite el numero de columnas matriz 2: \n");
  scanf("%d", &mat_col_2);

  printf("Digite el numero de filas matriz 2: \n");
  scanf("%d", &mat_row_2);

  float *mat_1 = (float*)malloc(mat_row_1 * mat_col_1 * sizeof(float));
  float *mat_2 = (float*)malloc(mat_row_2 * mat_col_2 * sizeof(float));
  fill_matrix(mat_1, mat_row_1, mat_col_1);
  fill_matrix(mat_2, mat_row_2, mat_col_2);

  print(mat_1, mat_row_1, mat_col_1);
  print(mat_2, mat_row_2, mat_col_2);


  return 0;
}
