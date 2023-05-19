#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_matrix(int rows, int cols, double** matrix)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrix_multiply(int rows_a, int cols_a, double** a,
                     int rows_b, int cols_b, double** b,
                     double** c)
{
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            double sumo = 0.0;
            for (int k = 0; k < cols_a; k++) {
                sumo += a[i][k] * b[k][j];
            }
            c[i][j] = sumo;
        }
    }
}

int main()
{
    // printf("%s", "hiy");
    const int ROWS_A = 1000;
    const int COLS_A = 1000;
    const int ROWS_B = 1000;
    const int COLS_B = 1000;

    // printf("%s", "hy3iy");
    double** a = (double**)malloc(ROWS_A * sizeof(double*));
    double** b = (double**)malloc(ROWS_B * sizeof(double*));
    
    // printf("%s", "hyiy");
    for (int i = 0; i < ROWS_A; i++) {
        a[i] = (double*)malloc(COLS_A * sizeof(double));
        for (int j = 0; j < COLS_A; j++) {
            a[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < ROWS_B; i++) {
        b[i] = (double*)malloc(COLS_B * sizeof(double));
        for (int j = 0; j < COLS_B; j++) {
            b[i][j] = rand() % 10;
        }
    }

    double** c = (double**)malloc(ROWS_A * sizeof(double*));
    for (int i = 0; i < ROWS_A; i++) {
        c[i] = (double*)malloc(COLS_B * sizeof(double));
    }

    // printf("%s", "hi");

    clock_t start = clock();
    matrix_multiply(ROWS_A, COLS_A, a, ROWS_B, COLS_B, b, c);
    clock_t end = clock();

    double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", elapsed_time);

    // Free the allocated memory
    for (int i = 0; i < ROWS_A; i++) {
        free(a[i]);
    }
    free(a);

    for (int i = 0; i < ROWS_B; i++) {
        free(b[i]);
    }
    free(b);

    for (int i = 0; i < ROWS_A; i++) {
        free(c[i]);
    }
    free(c);

    return 0;
}
