#include <iostream>
#include <cusparse.h>
#include <vector>
#include <cuda_runtime.h>
#include <cstdlib>

#include "Graph.hpp"

int adjacencyMatrix[N][N] = {0};
float X[N][8] = {0.0f};

void Create_adj_Table(int N) {
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                if (std::rand() % 100 < 30) {
                    adjacencyMatrix[i][j] = 1;
                }
            }
        }
    }
}

void Create_X(int dim) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < dim; j++) {
            X[i][j] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }
    }
}

int main() {
    Create_adj_Table(10);
    Create_X(8);
    Graph g(adjacencyMatrix, N);

    int n = g.get_v_num();
    int nnz = g.get_nzz();

    // 创建cuSPARSE句柄
    cusparseHandle_t handle;
    cusparseCreate(&handle);

    // 描述稀疏矩阵A
    cusparseSpMatDescr_t matA;
    cusparseCreateCsr(&matA, n, n, nnz, 
                      g.get_csr_row_offset().data(), 
                      g.get_csr_col_indices().data(), 
                      g.get_values().data(), 
                      CUSPARSE_INDEX_32I, CUSPARSE_INDEX_32I, 
                      CUSPARSE_INDEX_BASE_ZERO, CUDA_R_32F);

    // 描述输入矩阵X (列主序)
    cusparseDnMatDescr_t matX;
    cusparseCreateDnMat(&matX, n, 8, n, X, CUDA_R_32F, CUSPARSE_ORDER_COL);

    // 重新存储Y为列主序格式
    float Y_col_major[N * 8] = {0.0f};

    // 描述输出矩阵Y (也设置为列主序以匹配matX的顺序)
    cusparseDnMatDescr_t matY;
    cusparseCreateDnMat(&matY, n, 8, n, Y_col_major, CUDA_R_32F, CUSPARSE_ORDER_COL);

    float alpha = 1.0f;
    float beta = 0.0f;
    // 获取缓冲区大小
    size_t bufferSize = 0;
    cusparseSpMM_bufferSize(handle, 
                            CUSPARSE_OPERATION_NON_TRANSPOSE, 
                            CUSPARSE_OPERATION_NON_TRANSPOSE, 
                            &alpha, 
                            matA, 
                            matX, 
                            &beta, 
                            matY, 
                            CUDA_R_32F, 
                            CUSPARSE_SPMM_CSR_ALG1, 
                            &bufferSize);

    // 分配缓冲区
    void* dBuffer = nullptr;
    cudaMalloc(&dBuffer, bufferSize);

    // 执行稀疏矩阵与密集矩阵的乘法
    cusparseSpMM(handle, CUSPARSE_OPERATION_NON_TRANSPOSE, 
                 CUSPARSE_OPERATION_NON_TRANSPOSE, 
                 &alpha, matA, matX, &beta, matY, 
                 CUDA_R_32F, CUSPARSE_SPMM_CSR_ALG1, dBuffer);

    // 清理资源
    cudaFree(dBuffer);
    cusparseDestroyDnMat(matX);
    cusparseDestroyDnMat(matY);
    cusparseDestroySpMat(matA);
    cusparseDestroy(handle);


    return 0;
}
