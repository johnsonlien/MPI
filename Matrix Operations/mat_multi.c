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
    int pid, world_size, num_slaves;
    int rowA, colA, rowB, colB;

    MPI_Status status;
    MPI_Request request; 

    //----- Initialize MPI environment -----
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);    // get process ID
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);   // Get the world size

    num_slaves = world_size - 1;        // We will not be including process 0 to do the matrix calculations
    if(world_size < 3) {
        printf("Must have at least 3 processes. Exiting\n");
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
        if(num_slaves > rowA) {
            // An additional check to because later when we divide the matrix into subarrays,
            // if the number of slaves is greater than the number of rows we have, it will
            // cause 0 rows to be sent to other the processes.
            printf("Please choose a number of processes less than or equal to the (number of rows of A) + 1.\n");
            exit(1);
        }
        if(colA != rowB)
        {
            // Number of columns of matrix A must match the number of rows of matrix B.
            printf("Column of A does not match row of matrix B. Exiting\n");
            exit(1);
        }

        // We will be splitting up matrix A row-wise and matrix B column-wise.
        // We will then send these arrays to the other processes to compute
        // 1D matrix multiplication.

        // But first, we will rotate matrix B so that the columns now become rows. We can do this by transposing matrix B
        int** matBT = transpose(matB, rowB, colB);
        int rowBT = colB;
        int colBT = rowB;
        //printMatrix(matBT, colB, rowB);
        
        int subRows = rowA/num_slaves;        // 5 rows 4 slave, each process will get 1 row with 1 proc getting extra
        //int remainder = rowA % num_slaves;
        int offset = 0;     // Variable to help keep track of which process receives which row
        for(int dest = 1; dest <= num_slaves; dest++, offset += subRows) {
            // Send the offset. Slave 1 will get row 0, Slave 2 will get row 1
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&subRows, 1, MPI_INT, dest, 10, MPI_COMM_WORLD);       // Send the number of rows the process will receive
            MPI_Send(&colA, 1, MPI_INT, dest, 11, MPI_COMM_WORLD);
            
            // TODO: Send the subarrays
        }


        // TODO: implement receiving the calculated subarrays from each process
    }
    else {  // Slave processes that will do the matrix calculations
        int offset, rowA, colA, rowBT, colBT;
        int * subRowA;
        MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status); // receive the offset
        MPI_Recv(&rowA, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);    // receive the number of rows
        MPI_Recv(&colA, 1, MPI_INT, 0, 11, MPI_COMM_WORLD, &status);   // receive the number of columns
        subRowA = (int*)malloc(rowA * colA * sizeof(int));
        
        // TODO: Receive the sub arrays
        
    }

    MPI_Finalize();
    return 0;
}