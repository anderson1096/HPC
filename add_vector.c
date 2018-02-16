#include <stdio.h>
#include <stdlib.h>

void fill_vector(float *V, int len){
  float aux = 2.0;
  for (int i = 0; i < len; i++) {
    V[i] = (float)rand() / (float)(RAND_MAX / aux);
  }
}

void print(float *V, int len){
  for (int i = 0; i < len; i++) {
    printf("%.2f ", V[i]);
  }
  printf("\n");
}

void add(float *vector_a, float *vector_b, float *result, int len){
  for (int i = 0; i < len; i++) {
    result[i] = vector_a[i] + vector_b[i];
  }
}


void save(float *V, int len){
  FILE *f = fopen("result.csv", "r+");

  if (f == NULL){
    printf("File error\n");
    exit(-1);
  }

  for (int i = 0; i < len; i++) {
    if(len - 1 == i){
      fprintf(f, "%.2f", V[i]);
    }
    else{
      fprintf(f, "%.2f, ", V[i]);
    }
  }
  fclose(f);
}


int main(){
  int len;

  printf("Digite el tamano del vector: \n");
  scanf("%d", &len);

  float *vector_a = (float*)malloc(len * sizeof(float));
  float *vector_b = (float*)malloc(len * sizeof(float));
  float *result = (float*)malloc(len * sizeof(float));

  fill_vector(vector_a, len);
  fill_vector(vector_b, len);
  add(vector_a, vector_b, result, len);
  save(vector_a, len);
  save(vector_b, len);
  save(result, len);

  return 0;
}
