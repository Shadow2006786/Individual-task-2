#include <windows.h>
#include <stdio.h>

#define MATRIX_SIZE 4

// Matrices
int A[MATRIX_SIZE][MATRIX_SIZE] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
};
int B[MATRIX_SIZE][MATRIX_SIZE] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};
int C[MATRIX_SIZE][MATRIX_SIZE] = {0};

// Thread function
DWORD WINAPI MultiplyRow(LPVOID param) {
    int row = *(int*)param;
    for (int col = 0; col < MATRIX_SIZE; ++col) {
        for (int k = 0; k < MATRIX_SIZE; ++k) {
            C[row][col] += A[row][k] * B[k][col];
        }
    }
    return 0;
}

int main() {
    HANDLE threads[MATRIX_SIZE];
    int threadParams[MATRIX_SIZE];

    // Create threads for each row
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        threadParams[i] = i;
        threads[i] = CreateThread(
            NULL,               // Default security attributes
            0,                  // Default stack size
            MultiplyRow,        // Thread function
            &threadParams[i],   // Parameter to thread function
            0,                  // Default creation flags
            NULL                // Ignore thread ID
        );

        if (threads[i] == NULL) {
            printf("Error creating thread %d\n", i);
            return 1;
        }
    }

    // Wait for all threads to complete
    WaitForMultipleObjects(MATRIX_SIZE, threads, TRUE, INFINITE);

    // Close thread handles
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        CloseHandle(threads[i]);
    }

    // Print result
    printf("Resultant Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
