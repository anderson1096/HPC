#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void fill_vector(float *V, int len){
  float aux = 5.0;
  for (int i = 0; i < len; i++) {
    V[i] = ((float)rand() / (float)(RAND_MAX)) * aux ;
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
  FILE *f = fopen("result_add_vector.csv", "a");

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
  fprintf(f, "\n");
  fclose(f);
}


int main(){
  int len;
  clock_t start, end;

  printf("Digite la longitud de los vectores: \n");
  scanf("%d", &len);

  start = clock();
  //Memoria para los vectores
  float *vector_a = (float*)malloc(len * sizeof(float));
  float *vector_b = (float*)malloc(len * sizeof(float));
  float *result = (float*)malloc(len * sizeof(float));

  //Generando vectores aleatorios
  fill_vector(vector_a, len);
  fill_vector(vector_b, len);

  //Realizando la suma
  add(vector_a, vector_b, result, len);
  end = clock();

  //Almacenando en el archivo
  save(vector_a, len);
  save(vector_b, len);
  save(result, len);
  printf("Tiempo sin OMP: %.9f\n", (double)(end - start)/CLOCKS_PER_SEC);

  free(vector_a);
  free(vector_b);
  free(result);
  return 0;
}
