#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "a1.h"
#include "mpi.h"
#include "readwriteppm.c"
#include "Jeremyqzt.c" //Jeremyqzt.c contains processing of image

int main(int argc, char* argv[]){
  int m, n;

  int width, height, max;
  int my_rank, p;
  int i, dest, source;
  int offset;
  int tag = 0;
  MPI_Status status;
  int counter = 0;  //I put counters outside P0 so the other Ps can use them!
  int width_counter = width;
  char *input = argv[1]; //Input File Name
  char *output = argv[2]; //Output File Name
  int A = atoi(argv[3]); //N - The Filtering Size
  char *F = argv[4]; //F - Type of Filter
  RGB *image;
  
  //MPI STUFF HERE//
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  RGB *pixel; //Pixel is local buffer to host the original image segament
  RGB *newPixel; //new Pixel is the new buffer

  int sent;
  
  if (my_rank ==0){	//read the file and distribute the work

      if (A != 3){ //Check N
   printf("N must be 3.");
   return;
  }

    int *rows;
  	rows = (int*)malloc(sizeof(int)*p);	//size of rows based on processor, Based off of tutorial code
  	image = readPPM("4K-Ultra-HD.ppm", &width, &height, &max); //Read the entire image 
  	
  	for (i=0; i < p; i++)
    	rows[i] = width/p;	//principle calculations
  	for (i=0; i < width % p; i++)
    	rows[i]++;	//remaining calculations
    	
  	pixel = (double*)malloc(width*rows[0]*sizeof(double)); //process 0 size
  	newPixel = (double*)malloc(width*rows[0]*sizeof(double)); //process 0 new segament size

        int rowCount = 0; 
    for (i=1; i <p; i++){
    	sent = rows[i];
        rowCount = rowCount + sent*width;
			MPI_Send(&sent,1, MPI_INT, i, tag, MPI_COMM_WORLD); //send sent is # of rows each process is receiving
	if((i+1) == p){ //if last process, dont send extra row at end
		MPI_Send(image+rowCount-width,(sent*width)+(1*width), MPI_DOUBLE, i, tag, MPI_COMM_WORLD); 
	}else{

		MPI_Send(image+rowCount-width,(sent*width)+(2*width), MPI_DOUBLE, i, tag, MPI_COMM_WORLD); //send pointers to other processes
	}
    }
		

//start of P0's compression work----------------------------

		//printf("old pixel 0: %d %d %d\n", image[rows[0]*width].r, image[rows[0]*width].g, image[rows[0]*width].b);
		top(newPixel, image, 0, width);//NOTE: first run through

		while(counter < rows[0]*width){// cycle through all of rank 0's pixels
  		if(counter - width_counter > 0){
  			width_counter +=3*width; //go down 2 rows for next condition
  			counter = width_counter;//adjust new next pixel
  			top(newPixel, image, counter, width);//NOTE: for process 0: it has access to whole image, so boarder cases are not covered.
  		}else{
  			counter += 3; //next
  			top(newPixel, image, counter, width);
  		}
  	}
  	//printf("new pixel 0: %d %d %d\n", newPixel[rows[0]*width].r, newPixel[rows[0]*width].g, newPixel[rows[0]*width].b);



//start of compression work for P>0----------------------------
  }else{ 
    MPI_Recv(&sent,1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
  	pixel = (double*)malloc(width*sent*sizeof(double));

  	newPixel = (double*)malloc(width*sent*sizeof(double));
    MPI_Recv(pixel,width*(sent+1), MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status); //1 more row received
    
    //printf("myrank: %d my first red color: %d\n", my_rank, pixel[0].r);

        counter=width; //since top row is extra
	width_counter=2*	width;

	if (my_rank==p){ // if last row, need to treat it differently
		sent=sent-1;
	}

	while(counter < (1+sent)*width){// cycle through all of rank x's pixels. since row 1 is extra, we add a row(sent). 
  		if(counter - width_counter > 0){
  			width_counter +=3*width; //go down 2 rows for next condition
  			counter = width_counter;//adjust new next pixel
  			middle(newPixel, pixel, counter, width, width_counter/width);//NOTE: for process 0: it has access to whole image, so boarder cases are not covered.
  		}else{
  			middle(newPixel, pixel, counter, width, width_counter/width);
  			counter += 3; //next
  		}
  	}
	
	if(my_rank==p){ //last row
		width_counter+=3*width;
		counter = width_counter;
		while(counter < (2+sent)*width){//2+sent for last row. Will skip last row if its already processed by second last row
 
  			bottom(newPixel, pixel, counter, width,width_counter/width);
  			counter += 3; //next
  		}
	}
  	//printf("new pixel 0: %d %d %d\n", newPixel[rows[0]*width].r, newPixel[rows[0]*width].g, newPixel[rows[0]*width].b);

  }
  
   
  free(pixel);
  free(newPixel);
  MPI_Finalize();
  return 0;


/*

  
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
 	
  if (my_rank == 0){

  
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

   //writePPM("test1.ppm", width, height, max, pixel);

   free(image);
   free(buf);
   MPI_Finalize();
   return(0);

}

/*


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

