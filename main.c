#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "a1.h"
#include "readwriteppm.c"
int main(int argc, char* argv[]){
  int m, n;

  int width, height, max;
  int my_rank, p;
  int i, dest, source;
  int offset;
  int tag = 0;
  MPI_Status status;
  char *input = argv[1]; //Input File Name
  char *output = argv[2]; //Output File Name
  int A = atoi(argv[3]); //N - The Filtering Size
  char *F = argv[4]; //F - Type of Filter
  RGB *image;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  RGB *pixel;

  int sent;
  if (my_rank ==0){	//read the file

    int *rows;
  	rows = (int*)malloc(sizeof(int)*p);	//size of rows based on processor
  	image = readPPM("4K-Ultra-HD.ppm", &width, &height, &max);
  	
  	for (i=0; i < p; i++)
    	rows[i] = width/p;	//principle calculations
  	for (i=0; i < width % p; i++)
    	rows[i]++;	//remaining calculations
    	
  	pixel = (double*)malloc(width*rows[0]*sizeof(double)); //process 0 size

    for (i=1; i <p; i++){
    	sent = rows[i];
			MPI_Send(&sent,1, MPI_INT, i, tag, MPI_COMM_WORLD); //send pointers to other processes
			MPI_Send(image+i*sent,sent*width, MPI_DOUBLE, i, tag, MPI_COMM_WORLD); //send pointers to other processes
		}
  }else{
    MPI_Recv(&sent,1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
  	pixel = (double*)malloc(width*sent*sizeof(double));
    MPI_Recv(pixel,width*sent, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
  }
   
  free(pixel);
  MPI_Finalize();
  return 0;


/*
  rows = (int*)malloc(sizeof(int)*p);
  for (i=0; i < p; i++)
    rows[i] = m/p;
  for (i=0; i < m % p; i++)
    rows[i]++;
  
  // Allocate memory.

  if (my_rank == 0) 
    {
	pixel = malloc(sizeof(double))
	image = malloc(sizeof(double))	
    } 
  else 
    {

    }	  

  // Generate matrix and vector
  if (my_rank == 0) 
    { 

    }

  
  // PUT THIS INTO SEPARATE FUNCTION parallelMatrixVectorProduct and in a file parallmatvec.c


  int tag = 0;
  if (my_rank ==0)
    {
      int offset = 0;
      for (dest = 1; dest < p; dest++) 
	{
	  offset += rows[dest-1];

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

*/
}

