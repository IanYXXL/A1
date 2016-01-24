#include "math.h"

void top(RGB *pixel, RGB *image, int curPix, int width){
// USAGE: GIFF empty and oiginal image, puts mean filtered image into empty buffer.
 int i;
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
    }else{ //all other
    int list[9] = {curPix, curPix+1,curPix-1,curPix+width,curPix+width-1,curPix+width+1,curPix-width,curPix-width-1,curPix-width+1}; 
    for  (i = 0; i < 9; i++){
			pixel[list[i]].r= floor((image[curPix].r + image[curPix+1].r + image[curPix -1].r + image[curPix +width-1].r+image[curPix+width+1].r+image[curPix+width].r+ image[curPix-width-1].r+image[curPix-width+1].r+image[curPix-width].r)/9);
			pixel[list[i]].g= floor((image[curPix].g + image[curPix+1].g + image[curPix -1].g + image[curPix +width-1].g+image[curPix+width+1].g+image[curPix+width].g+ image[curPix-width-1].g+image[curPix-width+1].g+image[curPix-width].g)/9);
			pixel[list[i]].b= floor((image[curPix].b + image[curPix+1].b + image[curPix -1].b + image[curPix +width-1].b+image[curPix+width+1].b+image[curPix+width].b+ image[curPix-width-1].b+image[curPix-width+1].b+image[curPix-width].b)/9);
		}
}
}
void middle(RGB *pixel, RGB *image, int curPix, int width, int height){
	int i;
  if ((((curPix+1) % width) == 0)&&(curPix != (width-1))&&(curPix+1 != width*height)){ //Right Row
    int list[6] = {curPix - 1, curPix , curPix - width, curPix - width - 1,curPix + width, curPix + width - 1}; 
    for  (i = 0; i < 6; i++){
			pixel[list[i]].r= floor((image[curPix].r + image[curPix-1].r + image[curPix -width].r + image[curPix -width-1].r+image[curPix+width-1].r+image[curPix+width].r)/6);
			pixel[list[i]].g= floor((image[curPix].g + image[curPix-1].g + image[curPix -width].g + image[curPix -width-1].g+image[curPix+width-1].g+image[curPix+width].g)/6);
			pixel[list[i]].b= floor((image[curPix].b + image[curPix-1].b + image[curPix -width].b + image[curPix -width-1].b+image[curPix+width-1].b+image[curPix+width].b)/6);
			}
	}else if (((curPix % width) == 0)&&(curPix != (height-1)*(width))&&(curPix != 0)){ //Left Row
    int list[6] = {curPix + 1, curPix , curPix - width, curPix - width + 1,curPix + width, curPix + width + 1}; 
    for  (i = 0; i < 6; i++){
			pixel[list[i]].r= floor((image[curPix + 1].r + image[curPix].r + image[curPix -width].r + image[curPix -width+1].r+image[curPix+width+1].r+image[curPix+width].r)/6);
			pixel[list[i]].g= floor((image[curPix + 1].g + image[curPix].g + image[curPix -width].g + image[curPix -width+1].g+image[curPix+width+1].g+image[curPix+width].g)/6);
			pixel[list[i]].b= floor((image[curPix + 1].b + image[curPix].b + image[curPix -width].b + image[curPix -width+1].b+image[curPix+width+1].b+image[curPix+width].b)/6);
    }
	}else{ //all other
    int list[9] = {curPix, curPix+1,curPix-1,curPix+width,curPix+width-1,curPix+width+1,curPix-width,curPix-width-1,curPix-width+1}; 
    for  (i = 0; i < 9; i++){
			pixel[list[i]].r= floor((image[curPix].r + image[curPix+1].r + image[curPix -1].r + image[curPix +width-1].r+image[curPix+width+1].r+image[curPix+width].r+ image[curPix-width-1].r+image[curPix-width+1].r+image[curPix-width].r)/9);
			pixel[list[i]].g= floor((image[curPix].g + image[curPix+1].g + image[curPix -1].g + image[curPix +width-1].g+image[curPix+width+1].g+image[curPix+width].g+ image[curPix-width-1].g+image[curPix-width+1].g+image[curPix-width].g)/9);
			pixel[list[i]].b= floor((image[curPix].b + image[curPix+1].b + image[curPix -1].b + image[curPix +width-1].b+image[curPix+width+1].b+image[curPix+width].b+ image[curPix-width-1].b+image[curPix-width+1].b+image[curPix-width].b)/9);
		}
	}
}

void bottom(RGB *pixel, RGB *image, int curPix, int width, int height){
	int i;
  if (curPix+1 == width*height){ //Bottom right corner
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
		}
  }else if (curPix == (height-1)*(width)){ //Bottom left corner
    int list[4] = {curPix-width+1,curPix-width,curPix+1, curPix};
    for  (i = 0; i < 4; i++){
			pixel[list[i]].r= floor((image[curPix - width+1].r + image[curPix-width].r + image[curPix+1].r + image[curPix].r)/4);
			pixel[list[i]].g= floor((image[curPix - width+1].g + image[curPix-width].g + image[curPix+1].g + image[curPix].g)/4);
			pixel[list[i]].b= floor((image[curPix - width+1].b + image[curPix-width].b + image[curPix+1].b + image[curPix].b)/4);
     }
  }else{ //all other
    int list[9] = {curPix, curPix+1,curPix-1,curPix+width,curPix+width-1,curPix+width+1,curPix-width,curPix-width-1,curPix-width+1}; 
    for  (i = 0; i < 9; i++){
			pixel[list[i]].r= floor((image[curPix].r + image[curPix+1].r + image[curPix -1].r + image[curPix +width-1].r+image[curPix+width+1].r+image[curPix+width].r+ image[curPix-width-1].r+image[curPix-width+1].r+image[curPix-width].r)/9);
			pixel[list[i]].g= floor((image[curPix].g + image[curPix+1].g + image[curPix -1].g + image[curPix +width-1].g+image[curPix+width+1].g+image[curPix+width].g+ image[curPix-width-1].g+image[curPix-width+1].g+image[curPix-width].g)/9);
			pixel[list[i]].b= floor((image[curPix].b + image[curPix+1].b + image[curPix -1].b + image[curPix +width-1].b+image[curPix+width+1].b+image[curPix+width].b+ image[curPix-width-1].b+image[curPix-width+1].b+image[curPix-width].b)/9);
    }
	}
}
