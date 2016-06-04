#include <stdlib.h>
#include <string.h>
#include "structs.h"

extern	FILE    	 *Fp_log, *Fp_log_median;        // log file
extern	int 			 area;


dilation(unsigned char **image_pixel,int height,int width)
{
  int i,j;
  unsigned char **tmppixel;


  tmppixel=(unsigned char**)mymalloc2(height,width,sizeof(unsigned char));
  for(j=0;j<height;j++)
    for(i=0;i<width;i++)
      tmppixel[j][i]=image_pixel[j][i];
  for(j=1;j<height-1;j++)
    for(i=1;i<width-1;i++){
      if(tmppixel[j-1][i-1]==LOW  ||
         tmppixel[j-1][i  ]==LOW  ||
         tmppixel[j-1][i+1]==LOW  ||
	 tmppixel[j  ][i-1]==LOW  ||
         tmppixel[j  ][i+1]==LOW  ||
	 tmppixel[j+1][i-1]==LOW  ||
         tmppixel[j+1][i  ]==LOW  ||
         tmppixel[j+1][i+1]==LOW  )

	image_pixel[j][i]=LOW;
    }
  myfree2((void **)tmppixel,height);
}

erosion(unsigned char **image_pixel,int height,int width)
{
  int i,j;
  unsigned char **tmppixel;


  tmppixel=(unsigned char**)mymalloc2(height,width,sizeof(unsigned char));
  for(j=0;j<height;j++)
    for(i=0;i<width;i++)
      tmppixel[j][i]=image_pixel[j][i];
  for(j=1;j<height-1;j++)
    for(i=1;i<width-1;i++){
      if(tmppixel[j-1][i-1]==HIGH  ||
         tmppixel[j-1][i  ]==HIGH  ||
         tmppixel[j-1][i+1]==HIGH  ||
	 tmppixel[j  ][i-1]==HIGH  ||
         tmppixel[j  ][i+1]==HIGH  ||
	 tmppixel[j+1][i-1]==HIGH  ||
         tmppixel[j+1][i  ]==HIGH  ||
         tmppixel[j+1][i+1]==HIGH  )

	image_pixel[j][i]=HIGH;
    }
  myfree2((void **)tmppixel,height);
}

opening(unsigned char **image_pixel,int height,int width)
{
  erosion(image_pixel,height,width);
  dilation(image_pixel,height,width);
}

closing(unsigned char **image_pixel,int height,int width)
{
  dilation(image_pixel,height,width);
  erosion(image_pixel,height,width);
}
