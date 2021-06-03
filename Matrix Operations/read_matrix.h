/*
    A header file that contains functions to read and print matrix information from a txt file. 
*/
#ifndef _READ_MATRIX_H_
#define _READ_MATRIX_H_

#include <stdio.h>      // printf, fprintf, fscanf
#include <stdlib.h>     // malloc, free

void printMatrix(int** mat, int r, int c)
/*
 A function to help print out our 2-dimensional matrix.
*/
{
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

int ** readMatrix(const char * filename, int* r, int* c) 
/*
 Read in the matrix information from the filename.
*/
{
    FILE* fp = fopen(filename, "r");
    if(!fp) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }
    int tempR, tempC;
    if(fscanf(fp, "%d %d", &tempR, &tempC) != 2) {
        printf("Could not read rows and columns\n");
        exit(1);
    }
    printf("row: %d\tcol: %d\n", tempR, tempC);
    *r = tempR;
    *c = tempC;

    // Create the 2D matrix
    int ** mat = NULL;
    mat = (int**)malloc(tempR * sizeof(int*));
    for(int i = 0; i < tempR; i++) {
        mat[i] = (int*)malloc(tempC * sizeof(int));
    }

    for(int i = 0; i < tempR; i++) {
        for(int j = 0; j < tempC; j++) {
            fscanf(fp, "%d", &mat[i][j]);
        }
    }
    return mat;
}
#endif