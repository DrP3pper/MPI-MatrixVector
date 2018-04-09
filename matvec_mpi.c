/*
 *  matvec_mpi.c - MPI Uebung 5
 *
 *  Created by Thomas Kindermann on 25.06.07 with help of MPI
 *  Copyright 2007 Thomas Kindermann. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <stdint.h>
//#include <inttypes.h>
#include <sys/time.h>
#include <math.h>
#include "mpi.h"

#define N 16
#define ROOT 0

/*
 * main-function
 */
int main(int argc, char **argv) {
	char processor_name[MPI_MAX_PROCESSOR_NAME]; 
	int i, j, pos, offset;
	int myrank, numprocs, dest, namelen;
	int M[N][N], X[N], Y[N];
	MPI_Status status;
	MPI_Datatype hori_block, vec_block;

	MPI_Init(&argc, &argv); 
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Get_processor_name(processor_name, &namelen);
	
	offset = N / numprocs;
	pos = myrank * offset;
		
	MPI_Type_vector(offset, N, N, MPI_INT, &hori_block);
	MPI_Type_commit(&hori_block);
	MPI_Type_vector(offset, 1, 1, MPI_INT, &vec_block);
	MPI_Type_commit(&vec_block);

	/* master ******************************************************/
	if(myrank == ROOT) {
		srand(time(NULL));
		
		for(i = 0; i < N; i++) {
			X[i] = rand() % 10;
			for(j = 0; j < N; j++) {
				M[i][j] = rand() % 10;
				printf("%2d ", M[i][j]);	
			}
			printf("\n");
		}
		
		printf("Vektor X:\n");
		for(i = 0; i < N; i++)
			printf("%2d ", X[i]);
		
		printf("\n\n");

		for(dest = 0; dest < numprocs; dest++)
			MPI_Send(&M[dest * offset], 1, hori_block, dest, 0, MPI_COMM_WORLD);
	}
	
	MPI_Recv(&M[pos], 1, hori_block, ROOT, 0, MPI_COMM_WORLD, &status);
	
	printf("process %d (node %s) received this part (starting at pos %d) of the matrix:\n", myrank, processor_name, pos);
	
	for(i = pos; i < pos + offset; i++) {
		for(j = 0; j < N; j++)
			printf("%2d ", M[i][j]);
			
		printf("\n");
	}
/*
	MPI_Bcast(&X, N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
	
	printf("process' %d (node %s) Vektor X:\n", myrank, processor_name);
	for(i = 0; i < N; i++)
		printf("%2d ", X[i]);
		
	printf("\n");

	printf("process' %d (node %s) part of Vektor Y:\n", myrank, processor_name);
	for(i = pos; i < pos + offset; i++) {
		Y[i] = 0;
		for(j = 0; j < N; j++)
			Y[i] += M[i][j] * X[j];

		printf("%2d ", Y[i]);
		printf("\n");
	}

	MPI_Send(&Y[pos], 1, vec_block, 0, 0, MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if(myrank == ROOT) {
		for(dest = 0; dest < numprocs; dest++)
			MPI_Recv(&Y[dest * offset], 1, vec_block, dest, 0, MPI_COMM_WORLD, &status);

		printf("\nVector Y:\n");
		for(i = 0; i < N; i++)
			printf("%2d ", Y[i]);
		
		printf("\n");
	}*/
	
	MPI_Finalize();
	
	return 0;
}

