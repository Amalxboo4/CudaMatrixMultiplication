%%cu
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

#define TILE_WIDTH 32  // Assuming we're using 32x32 threads per block

__global__ void MatrixMulKernel(float* M, float* N, float* P, int M_rows, int M_cols_N_rows, int N_cols) {
    # // Calculate the row index of the P element and M
    int Row = blockIdx.y*TILE_WIDTH+threadIdx.y;
    # // Calculate the column index of P and N
    int Col = blockIdx.x*TILE_WIDTH+threadIdx.x;
    if ((Row < M_rows) && (Col < N_cols)) {
        float Pvalue = 0;
        # // each thread computes one element of the block sub-matrix
        for (int k = 0; k < M_cols_N_rows; ++k) {
            Pvalue += M[Row*M_cols_N_rows+k]*N[k*N_cols+Col];
        }
        P[Row*N_cols+Col] = Pvalue;
    }
}

int main() {
    int M_rows = 1000;
    int M_cols_N_rows = 1000;
    int N_cols = 1000;

    size_t size_M = M_rows * M_cols_N_rows * sizeof(float);
    size_t size_N = M_cols_N_rows * N_cols * sizeof(float);
    size_t size_P = M_rows * N_cols * sizeof(float);

    # // allocate host memory
    float* h_M = (float*)malloc(size_M);
    float* h_N = (float*)malloc(size_N);
    float* h_P = (float*)malloc(size_P);

    # // initialize host matrices with random values
    for(int i = 0; i < M_rows * M_cols_N_rows; i++){
        h_M[i] = rand() / (float)RAND_MAX;
    }
    for(int i = 0; i < M_cols_N_rows * N_cols; i++){
        h_N[i] = rand() / (float)RAND_MAX;
    }

    # // allocate device memory
    float* d_M;
    float* d_N;
    float* d_P;
    cudaMalloc(&d_M, size_M);
    cudaMalloc(&d_N, size_N);
    cudaMalloc(&d_P, size_P);

    # // copy inputs to device
    cudaMemcpy(d_M, h_M, size_M, cudaMemcpyHostToDevice);
    cudaMemcpy(d_N, h_N, size_N, cudaMemcpyHostToDevice);

    dim3 dimBlock(TILE_WIDTH, TILE_WIDTH);
    dim3 dimGrid((N_cols + dimBlock.x - 1) / dimBlock.x, (M_rows + dimBlock.y - 1) / dimBlock.y);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, NULL);

    MatrixMulKernel<<<dimGrid, dimBlock>>>(d_M, d_N, d_P, M_rows, M_cols_N_rows, N_cols);

    cudaEventRecord(stop, NULL);
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    printf("The elapsed time in GPU was: %f ms\n", milliseconds);

    # // Copy result back to host
    cudaMemcpy(h_P, d_P, size_P, cudaMemcpyDeviceToHost);

    # // print some of the results
    printf("Printing some elements of the result matrix:\n");
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%f ", h_P[i*N_cols+j]);
        }
        printf("\n");
    }

    # // Cleanup
    free(h_M);
    free(h_N);
    free(h_P);
    cudaFree(d_M);
    cudaFree(d_N);
    cudaFree(d_P);

    return 0;
}

