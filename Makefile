CC = mpicc

all: matvec_mpi

matvec_mpi: matvec_mpi.o
	$(CC) matvec_mpi.o -o matvec_mpi

matvec_mpi.o: matvec_mpi.c
	$(CC) -Wall -c matvec_mpi.c -o matvec_mpi.o 

clean:
	rm -f matvec_mpi.o matvec_mpi
