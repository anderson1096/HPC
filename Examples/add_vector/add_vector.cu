#include <stdlib.h>
#include <cuda.h>
#include <stdio.h>
#include <malloc.h>



__host__
void fill_vector(float *V, int len){
  float aux = 5.0;
  for (int i = 0; i < len; i++) {
    V[i] = ((float)rand() / (float)(RAND_MAX)) * aux ;
  }
}

__host__
void print(float *V, int len){
  for (int i = 0; i < len; i++) {
    printf("%.2f ", V[i]);
  }
  printf("\n");
}

__global__
void AddVector(float* d_A, float* d_B, float* d_R, int n){
  //calculate row index of element
  int i = threadIdx.x + blockDim.x * blockIdx.x;

  if (i < n) d_R[i] = d_A[i] + d_B[i];
  return;
}


int main(){
  int n = 100;
  float size = n * sizeof(float);

  //Manejo de errores en cuda
  cudaError_t error = cudaSuccess;

  //CPU
  float *h_A, *h_B, *h_R;
  h_A = (float*)malloc(size);
  h_B = (float*)malloc(size);
  h_R = (float*)malloc(size);


  //GPU
  float *d_A, *d_B, *d_R;

  error = cudaMalloc((void**)&d_A, size);
  if (error != cudaSuccess){
    printf("Error solicitando memoria en la GPU para d_A\n");
    exit(-1);
  }

  error = cudaMalloc((void**)&d_B, size);
  if (error != cudaSuccess){
    printf("Error solicitando memoria en la GPU para d_B\n");
    exit(-1);
  }

  error = cudaMalloc((void**)&d_R, size);
  if (error != cudaSuccess){
    printf("Error solicitando memoria en la GPU para d_R\n");
    exit(-1);
  }

  //Fill Matrix
  fill_vector(h_A, n);
  fill_vector(h_B, n);
  print(h_A, n);
  printf("---------------------------------\n");
  print(h_B, n);
  printf("---------------------------------\n");
  //Copy from CPU to GPU
  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

  //Dimension kernel
  dim3 dimGrid(ceil(n/10.0), 1, 1);
  dim3 dimBlock(10,1,1);
  AddVector<<<dimGrid, dimBlock>>>(d_A, d_B, d_R, n);
  cudaDeviceSynchronize();


  cudaMemcpy(h_R, d_R, size, cudaMemcpyDeviceToHost);
  print(h_R, n);

  free(h_A); free(h_B); free(h_R);
  cudaFree(d_A); cudaFree(d_B); cudaFree(d_R);

  return 0;
}
