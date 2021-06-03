#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#include "read_matrix.h"        // Header files to read/print a matrix

//#include <errno.h>

/* 
    This program will perform 1-D matrix multiplication.
    It splits the matrix into columns and sends each column 
    into a different process for each process to calculate.
*/

int main(int argc, char ** argv) {
    
    //----- Variable Declarations -----
    int pid, size;
    int rowA, colA, rowB, colB;

    MPI_Status status;
    MPI_Request request; 

    //----- Initialize MPI environment -----
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);    // get process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Get the world size

    if(size < 2) {
        printf("Must have at least 2 processes. Exiting\n");
        exit(1);
    }

    if(pid == 0) 
    {   
        int **matA = NULL, **matB = NULL, **matC = NULL;

        // Read in the matrix 
        matA = readMatrix("matrixA.txt", &rowA, &colA);
        matB = readMatrix("matrixB.txt", &rowB, &colB);
        //printMatrix(matA, rowA, colA);
        //printMatrix(matB, rowB, colB);

        if(colA != rowB)
        {
            // Number of columns of matrix A must match the number of rows of matrix B.
            printf("Column of A does not match row of matrix B. Exiting\n");
            exit(1);
        }

        // We will be splitting up matrix A row-wise and matrix B column-wise.
        // We will then send these arrays to the other processes to compute
        // 1D matrix multiplication.
        int subRows = rowA/size;        // 5 rows 4 procs, each process will get 1 row with 1 proc getting
    }
    else {

    }

    MPI_Finalize();
    return 0;
}