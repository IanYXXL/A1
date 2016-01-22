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

  //N = argv[3]; //N is the block size 
  //F = argv[4]; //F is type of filtering
  image = readPPM("test.ppm", &width, &height, &max); //Read the image, i don't know if we need this in the MPI section


  //Start MPI here, each section needs an Malloc of their calculation size
  pixel = malloc(sizeof(image));

  if  (curPix == 0){  //Top row, left corner
    int list[4] = {0,1,width, width +1};
    for  (i = 0; i < 4; i++){
	pixel[list[i]].r= floor((image[0].r + image[1].r + image[width].r + image[width+1].r)/4);
	pixel[list[i]].g= floor((image[0].g + image[1].g + image[width].g + image[width+1].g)/4);
	pixel[list[i]].b= floor((image[0].b + image[1].b + image[width].b + image[width+1].b)/4);
    }
  }else if (curPix > 0 &&curPix < (width-1)){ //Top row

    int list[6] = {curPix - 1, curPix , curPix + 1, curPix + width - 1,curPix + width, curPix + width + 1}; 
    for  (i = 0; i < 6; i++){
	pixel[list[i]].r= floor((image[curPix].r + image[curPix-1].r + image[curPix+1].r + image[curPix+width+1].r+image[curPix+width-1].r+image[curPix+width].r)/6);
	pixel[list[i]].g= floor((image[curPix].g + image[curPix-1].g + image[curPix+1].g + image[curPix+width+1].g+image[curPix+width-1].g+image[curPix+width].g)/6);
	pixel[list[i]].b= floor((image[curPix].b + image[curPix-1].b + image[curPix+1].b + image[curPix+width+1].b+image[curPix+width-1].b+image[curPix+width].b)/6);
    }

  }else if ((curPix+1) == width){ //Top row, right corner

    int list[4] = {width-1,width-2,2*width-1, 2*width-2};
    for  (i = 0; i < 4; i++){
	pixel[list[i]].r= floor((image[width-1].r + image[width-2].r + image[2*width-1].r + image[2*width-2].r)/4);
	pixel[list[i]].g= floor((image[width-1].g + image[width-2].g + image[2*width-1].g + image[2*width-2].g)/4);
	pixel[list[i]].b= floor((image[width-1].b + image[width-2].b + image[2*width-1].b + image[2*width-2].b)/4);
    }

  }else if ((((curPix+1) % width) == 0)&&(curPix != (width-1))&&(curPix+1 != width*height)){ //Right Row, Ned's write ppm is bugged
    int list[6] = {curPix - 1, curPix , curPix - width, curPix - width - 1,curPix + width, curPix + width - 1}; 
    for  (i = 0; i < 6; i++){
	pixel[list[i]].r= floor((image[curPix].r + image[curPix-1].r + image[curPix -width].r + image[curPix -width-1].r+image[curPix+width-1].r+image[curPix+width].r)/6);
	pixel[list[i]].g= floor((image[curPix].g + image[curPix-1].g + image[curPix -width].g + image[curPix -width-1].g+image[curPix+width-1].g+image[curPix+width].g)/6);
	pixel[list[i]].b= floor((image[curPix].b + image[curPix-1].b + image[curPix -width].b + image[curPix -width-1].b+image[curPix+width-1].b+image[curPix+width].b)/6);
    	//printf("%d %d %d \n", pixel[list[i]].r,pixel[list[i]].g,pixel[list[i]].b);
	}

  }else if (curPix+1 == width*height){ //Bottom right corner
    int list[4] = {curPix-width-1,curPix-width,curPix-1, curPix};
    for  (i = 0; i < 4; i++){
	pixel[list[i]].r= floor((image[curPix - width-1].r + image[curPix-width].r + image[curPix-1].r + image[curPix].r)/4);
	pixel[list[i]].g= floor((image[curPix - width-1].g + image[curPix-width].g + image[curPix-1].g + image[curPix].g)/4);
	pixel[list[i]].b= floor((image[curPix - width-1].b + image[curPix-width].b + image[curPix-1].b + image[curPix].b)/4);
	}
    }else if ((curPix+width >= height*width)&&(curPix != (height-1)*width)&&(curPix+1 != width*height)){ //bottom row

    int list[6] = {curPix, curPix-1 , curPix +1, curPix - width ,curPix - width +1, curPix - width -1}; 
    for  (i = 0; i < 6; i++){
	pixel[list[i]].r= floor((image[curPix].r + image[curPix-1].r + image[curPix+1].r + image[curPix -width-1].r+image[curPix-width+1].r+image[curPix+width].r)/6);
	pixel[list[i]].g= floor((image[curPix].g + image[curPix-1].g + image[curPix+1].g + image[curPix -width-1].g+image[curPix-width+1].g+image[curPix+width].g)/6);
	pixel[list[i]].b= floor((image[curPix].b + image[curPix-1].b + image[curPix+1].b + image[curPix -width-1].b+image[curPix-width+1].b+image[curPix+width].b)/6);
    	//printf("%d\n", list[i]);
    	//printf("%d %d %d \n", pixel[list[i]].r,pixel[list[i]].g,pixel[list[i]].b);
	}
    }else if (curPix == (height-1)*(width)){ //Bottom left corner
    int list[4] = {curPix-width+1,curPix-width,curPix+1, curPix};
    for  (i = 0; i < 4; i++){
	pixel[list[i]].r= floor((image[curPix - width+1].r + image[curPix-width].r + image[curPix+1].r + image[curPix].r)/4);
	pixel[list[i]].g= floor((image[curPix - width+1].g + image[curPix-width].g + image[curPix+1].g + image[curPix].g)/4);
	pixel[list[i]].b= floor((image[curPix - width+1].b + image[curPix-width].b + image[curPix+1].b + image[curPix].b)/4);
    	printf("%d\n", list[i]);
    	//printf("%d %d %d \n", pixel[list[i]].r,pixel[list[i]].g,pixel[list[i]].b);
     }
    }else if (((curPix % width) == 0)&&(curPix != (height-1)*(width))&&(curPix != 0)){ //left Row, Ned's write ppm is bugged
    int list[6] = {curPix + 1, curPix , curPix - width, curPix - width + 1,curPix + width, curPix + width + 1}; 
    for  (i = 0; i < 6; i++){
	pixel[list[i]].r= floor((image[curPix + 1].r + image[curPix].r + image[curPix -width].r + image[curPix -width+1].r+image[curPix+width+1].r+image[curPix+width].r)/6);
	pixel[list[i]].g= floor((image[curPix + 1].g + image[curPix].g + image[curPix -width].g + image[curPix -width+1].g+image[curPix+width+1].g+image[curPix+width].g)/6);
	pixel[list[i]].b= floor((image[curPix + 1].b + image[curPix].b + image[curPix -width].b + image[curPix -width+1].b+image[curPix+width+1].b+image[curPix+width].b)/6);
    	//printf("%d %d %d \n", pixel[list[i]].r,pixel[list[i]].g,pixel[list[i]].b);
    	printf("%d\n", list[i]);
    }
  }else{ //all other
    int list[9] = {curPix, curPix+1,curPix-1,curPix+width,curPix+width-1,curPix+width+1,curPix-width,curPix-width-1,curPix-width+1}; 
    for  (i = 0; i < 9; i++){
	pixel[list[i]].r= floor((image[curPix].r + image[curPix+1].r + image[curPix -1].r + image[curPix +width-1].r+image[curPix+width+1].r+image[curPix+width].r+ image[curPix-width-1].r+image[curPix-width+1].r+image[curPix-width].r)/9);

	pixel[list[i]].g= floor((image[curPix].g + image[curPix+1].g + image[curPix -1].g + image[curPix +width-1].g+image[curPix+width+1].g+image[curPix+width].g+ image[curPix-width-1].g+image[curPix-width+1].g+image[curPix-width].g)/9);

	pixel[list[i]].b= floor((image[curPix].b + image[curPix+1].b + image[curPix -1].b + image[curPix +width-1].b+image[curPix+width+1].b+image[curPix+width].b+ image[curPix-width-1].b+image[curPix-width+1].b+image[curPix-width].b)/9);
    	//printf("%d %d %d \n", pixel[list[i]].r,pixel[list[i]].g,pixel[list[i]].b);
    	printf("%d\n", list[i]);
    }

}

  image = pixel;
  writePPM("test1.ppm", width, height, max, pixel);
  free(pixel);
  //char        *buf = malloc(5*sizeof(int)+sizeof(RGB));  /*passing N, F, image height,segment top, segment bottom ,partial image for compression*/
  //if the top/bottom is (N/2)-0.5 away from 0 or image height, then the grid of the NxN must be made smaller when compressing.


/*
  if (N%2 == 0){ //Check N
   f("N must be odd");
   return;
  }*/



}

/*
  MPI_Init(&argc, &argv);	
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
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
	mean_sum_r += image[i].r;
	mean_sum_g += image[i].g;
	mean_sum_b += image[i].b;
	counter = 1;
	if ((i+1) % width !=0){ //checks if it is right edge pixel
	  mean_sum_r += image[i+1].r;
	  mean_sum_g += image[i+1].g;
	  mean_sum_b += image[i+1].b;
	  mean_sum_r += image[i+width+1].r;    	
	  mean_sum_g += image[i+width+1].g;
	  mean_sum_b += image[i+width+1].b;
	  counter += 2;
  	}
	if ((i+1)- width < 0 && (i+1) % width !=0){ //checks if it is top pixel and not a right edge pixel
	  mean_sum_r += image[i-width].r;
	  mean_sum_g += image[i-width].g;
	  mean_sum_b += image[i-width].b;
	  mean_sum_r += image[i-width+1].r;
	  mean_sum_g += image[i-width+1].g;
	  mean_sum_b += image[i-width+1].b;
	  mean_sum_r += image[i-width-1].r;
	  mean_sum_g += image[i-width-1].g;
	  mean_sum_b += image[i-width-1].b;
	  counter += 3;
  	}
  }

  if (my_rank == 0){




   
   //processImage(width, height, image);
   

//current line in loop of assigning lines to processes. Since 0 has to process its own lines,
//so I think we should start at process 1 and skip sub, so we skip lines between 0 and subN*N


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
  

}
   free(image);
   free(buf);
   MPI_Finalize();
   return(0);

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
