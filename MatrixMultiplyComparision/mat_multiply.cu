#include <stdlib.h>
#include <stdio.h>
#include <cuda.h>

#define TILE_WIDTH 32

__host__
void fill_matrix(float* M , int row , int col){
	for (int i = 0; i < row; ++i){
		for (int j = 0; j < col; ++j)
		{
			M[i*col + j] = 2.0;
		}
	}
}

__host__
void read(float *M, FILE *source, int rows, int cols){
	for (int i = 0; i < rows; ++i){
		for (int j = 0; j < cols; ++j){
			fscanf(source, "%f,", &M[i * cols + j]);
		}
	}
	fclose(source);
	return;
}

__host__
void print(float *M, int rows, int cols){
  printf("\n");
  printf("----------------------------------------\n");
  for(int i = 0; i < rows; i++) {
  		for(int j = 0; j < cols; j++) {
     		printf("%.2f ", M[i * cols + j]);
    	}
		printf("\n");
  }
  printf("----------------------------------------\n");
  printf("\n");
  return;
}

__global__
void MatrixMultiplySMKernel(float *d_A, float *d_B, float *d_R, int colsA, int rowsA, int colsB, int rowsB){

	__shared__ float Ads[TILE_WIDTH][TILE_WIDTH];
	__shared__ float Bds[TILE_WIDTH][TILE_WIDTH];

	int bx = blockIdx.x; int by = blockIdx.y;
	int tx = threadIdx.x; int ty = threadIdx.y;


	int col = bx * TILE_WIDTH + tx;
	int row = by * TILE_WIDTH + ty;

	float Pvalue = 0;
	for (int m = 0; m < (TILE_WIDTH + colsA - 1)/TILE_WIDTH; ++m){

		if(m * TILE_WIDTH + tx < colsA && row < rowsA){
			Ads[ty][tx] = d_A[row * colsA + m * TILE_WIDTH + tx];
		}
		else{
			Ads[ty][tx] = 0.0;
		}

		if(m * TILE_WIDTH + ty < rowsB && col < colsB){
			Bds[ty][tx] = d_B[(m * TILE_WIDTH + ty) * colsB + col];
		}
		else{
			Bds[ty][tx] = 0.0;
		}

		__syncthreads();

		for (int k = 0; k < TILE_WIDTH; ++k){
			Pvalue += Ads[ty][k] * Bds[k][tx];
		}

		__syncthreads();
	}

	if(row < rowsA && col < colsB){
		d_R[((by * blockDim.y + ty) * colsB) + (bx * blockDim.x) + tx] = Pvalue;
	}
	return;
}


int main(int argc, char** argv)
{

	/*if (argc != 3){
		printf("Debe añadir los nombres de los archivos\n");
		return 1;
	}*/

	float *h_A, *h_B, *h_R;
	int rowsA, rowsB, colsA, colsB = 100;



	cudaError_t error = cudaSuccess;

	//FILE *file_1, *file_2;
	//file_1 = fopen(argv[1], "r");
	//file_2 = fopen(argv[2], "r");

	//fscanf(file_1, "%d", &rowsA);
	//fscanf(file_1, "%d", &colsA);
	//fscanf(file_2, "%d", &rowsB);
	//fscanf(file_2, "%d", &colsB);

	if (colsA != rowsB){
		printf("Es imposible multiplicar las matrices\n");
		return 1;
	}

	float sizeA = rowsA * colsA * sizeof(float);
	float sizeB = rowsB * colsB * sizeof(float);
	float sizeR = rowsA * colsB * sizeof(float);


	h_A = (float*)malloc(sizeA);
	h_B = (float*)malloc(sizeB);
	h_R = (float*)malloc(sizeR);

	//read(h_A, file_1, rowsA, colsA);
	//read(h_B, file_2, rowsB, colsB);

	fill_matrix(*h_A, rowsA, colsA);
	fill_matrix(*h_B, rowsB, colsB);

	float *d_A, *d_B, *d_R;

	error = cudaMalloc((void**)&d_A, sizeA);
	if (error != cudaSuccess){
		printf("Error solicitando memoria para d_A \n");
		return 1;
	}

	error = cudaMalloc((void**)&d_B, sizeB);
	if (error != cudaSuccess){
		printf("Error solicitando memoria para d_B \n");
		return 1;
	}

	error = cudaMalloc((void**)&d_R, sizeR);
	if (error != cudaSuccess){
		printf("Error solicitando memoria para d_R \n");
		return 1;
	}

	cudaMemcpy(d_A, h_A, sizeA, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, h_B, sizeB, cudaMemcpyHostToDevice);

	int blockSize = 32;
	dim3 dimGrid(32, 32, 1);
	dim3 dimBlock(blockSize, blockSize, 1);

	MatrixMultiplySMKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_R, colsA, rowsA, colsB, rowsB);
	cudaMemcpy(h_R, d_R, sizeR, cudaMemcpyDeviceToHost);

	print(h_A, rowsA, colsA);
	print(h_B, rowsB, colsB);
	print(h_R, rowsA, colsB);


	free(h_A);
	free(h_B);
	free(h_R);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_R);


	/* code */
	return 0;
}
