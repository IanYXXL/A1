#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "as1.h"

int main(int argc, char* argv[]) 
{
  double *A, *b, *y;
  int m, n;
  
  int my_rank, p;
  int i, dest, source;
  int offset;
  
  MPI_Status status;
  
  m = atoi(argv[1]);
  n = atoi(argv[2]);
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  

  // Find how many rows per process.
  int *rows;
  rows = (int*)malloc(sizeof(int)*p);
  for (i=0; i < p; i++)
    rows[i] = m/p;
  for (i=0; i < m % p; i++)
    rows[i]++;
  
  // Allocate memory.
  b = malloc( n*sizeof(double));
  if (my_rank == 0) 
    {
      A = (double*)malloc(m * n * sizeof(double));
      y = (double*)malloc(m * sizeof(double));
    } 
  else 
    {
      A = (double*)malloc(rows[my_rank] * n * sizeof(double));
      y = (double*)malloc(rows[my_rank] * sizeof(double));
    }	  

  // Generate matrix and vector
  if (my_rank == 0) 
    { 
      genMatrix(m, n, A);
      genVector(n, b);
    }

  
  // PUT THIS INTO SEPARATE FUNCTION parallelMatrixVectorProduct and in a file parallmatvec.c


  int tag = 0;
  if (my_rank ==0)
    {
      int offset = 0;
      for (dest = 1; dest < p; dest++) 
	{
	  offset += rows[dest-1];
	  MPI_Send(A + offset*n, rows[dest]*n, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
	}
    }
  else
    MPI_Recv(A, rows[my_rank]*n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
  
  MPI_Bcast(b, n, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
  
  compDotProduct(rows[my_rank], n, A, b, y);
  
  // Get the data
  if (my_rank !=0)
    MPI_Send(y, rows[my_rank], MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
  else
    {
      offset = 0;
      for (source = 1; source < p; source++)
	{
	  offset += rows[source-1];
	  MPI_Recv(y+offset, rows[source], MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
	}
    }
  // END OF parallMatrixVectorProduct
  

  if (my_rank == 0)
    {
      getResult(m, n, A, b, y);
    }
    
  free(A);
  free(y);
  free(b);
  free(rows);
  
  
  
  MPI_Finalize();
  return 0;
  
  
}  




