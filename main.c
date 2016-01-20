#include <stdlib.h>
#include "a1.h"
#include "mpi.h"

int main(int argc, char** argv)
{
  RGB *image;
  int width, height, max;
  int N, F;		
  int         my_rank;       /* rank of process      */
  int         p;             /* number of processes  */
  int         source;        /* rank of sender       */
  int         dest;          /* rank of receiver     */
  int         tag = 0;       /* tag for messages     */
  char        *buf = malloc(5*sizeof(int)+sizeof(RGB));  /*passing N, F, image height,segment top, segment bottom ,partial image for compression*/
  //if the top/bottom is (N/2)-0.5 away from 0 or image height, then the grid of the NxN must be made smaller when compressing.

  N = argv[3];
  F = argv[4];
  
  MPI_Init(&argc, &argv);	/*init mpi*/
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (my_rank == 0){

   if (N%2 == 0){
    printf("N must be odd");
    return;
   }

   image = readPPM(argv[1], &width, &height, &max);
   
   //processImage(width, height, image);
   
   int subN = (height / N) / p;  //how any NxN blocks per process
   int remN = (height / N) % p;	 //remainder to add to the first few processes
   int curLine ;	
/*current line in loop of assigning lines to processes. Since 0 has to process its own lines,
so I think we should start at process 1 and skip sub, so we skip lines between 0 and subN*N
   int lastLine ;	//previous line in loop
   RGB subImage ;       //image segment for the process
   for(i=1;i<p;i++){

  	  int *N  = (int*)buf;
  	  int *F  = N+1;
	  int *height = F+1;
          int *lastLine = height+1;
	  int *curLine = lastLine+1;
          RGB   *image  = (int*)(b+1);
	MPI_Send( buf, 1, MPI_CHAR, i, i, MPI_COMM_WORLD);
	//MPI_Recv(buf, 1, MPI_CHAR, i, i, MPI_COMM_WORLD);
   }
   writePPM(argv[2], width, height, max, image);
  

  }else{ //side proccess for dealing with NxN
   MPI_Recv()
   if(F== A){

   }else if(F==M){

   }
   MPI_Send()

}
   free(image);
   free(buf);
   MPI_Finalize();
   return(0);

}

int 
