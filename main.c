#include <stdlib.h>
#include <math.h>
#include "a1.h"
//#include "mpi.h"
#include "readwriteppm.c"

//extern RGB * readPPM(char* file, int* width, int* height, int* max);
//extern void writePPM(char* file, int width, int height, int max, const RGB *image);
//int argc, char** argv
int main(){
  RGB *image;
  RGB *pixel;

  int width, height, max;
  int N, F;		
  int         my_rank;       /* rank of process      */
  int         p;             /* number of processes  */
  int         source;        /* rank of sender       */
  int         dest;          /* rank of receiver     */
  int         tag = 0;       /* tag for messages     */
  int i;
  int curPix= 4;

  MPI_Init(&argc, &argv);	

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  MPI_Comm_size(MPI_COMM_WORLD, &p);

  N = argv[3]; //N is the block size 
  F = argv[4]; //F is type of filtering
  image = readPPM("test.ppm", &width, &height, &max); //Read the image, i don't know if we need this in the MPI section




  //char        *buf = malloc(5*sizeof(int)+sizeof(RGB));  /*passing N, F, image height,segment top, segment bottom ,partial image for compression*/
  //if the top/bottom is (N/2)-0.5 away from 0 or image height, then the grid of the NxN must be made smaller when compressing.

  if (N != 3){ //Check N
   f("N must be 3.");
   return;
  }
  // Find how many rows per process.
  int *rows;
  rows = (int*)malloc(sizeof(int)*p);
  for (i=0; i < p; i++){
    rows[i] = height/p;}
  for (i=0; i < height % p; i++){
    rows[i]++;}

  int tag = 0
 
  if (my_rank == 0){
  
   int offset = 0;
	 
   for(i=1;i<p;i++){
	 offset += rows[i-1];
	MPI_Send( image+offset*width-1, rows[i]*width +1 , MPI_CHAR, i, tag, MPI_COMM_WORLD);
	//MPI_Recv(buf, 1, MPI_CHAR, i, i, MPI_COMM_WORLD);  

  }else{
    MPI_Recv(image, rows[my_rank]*width, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);

  //do work on the image segment
  }

  if (my_rank !=0)
    MPI_Send(pixel, rows[my_rank], MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
  else
    {
      offset = 0;
      for (source = 1; source < p; source++)
	{
	  offset += rows[source-1];
	  MPI_Recv(pixel+offset, rows[source], MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
	}
    }
   writePPM("test1.ppm", width, height, max, pixel);

   free(image);
   free(buf);
   MPI_Finalize();
   return(0);

}

/*
  
  image = readPPM(argv[1], &width, &height, &max); //Read the image, i don't know if we need this in the MPI section


  int local_section = floor(height/p); //local segament of processing
  int unaccounted = height % p; //OVer the line
  RGB* sub_image;
  RGB* total_image_section;
  int mean_sum_r;
  int mean_sum_g;
  int mean_sum_b;
  int mean;
  int mean_counter;

  int local_calc = my_rank*local_section;
  int local_calc_termination = (my_rank + 1)*local_section;

  for (i = 0; i < local_calc_termination; i +=3){ 	//Mean Calculation



}

/*
  int subN = (height / N) / p;  //how any NxN blocks per process
  int remN = (height / N) % p;	 //remainder to add to the first few processes
  int curLine ;		//current line
  int lastLine ;	//previous line in loop
  RGB subImage ;       //image segment for the process

  int local_midRow = 3*my_rank*subN;
  */

/*
  }else{ //side proccess for dealing with NxN
   MPI_Recv()
   if(F== A){

   }else if(F==M){

   }
   MPI_Send()
*/
