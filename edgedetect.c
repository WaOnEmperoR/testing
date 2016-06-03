#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "edgedetect.h"
#include "structs.h"

#define PI 3.14

void edge_detection(T_IMAGE *ptr_my_image, char *filtername, T_IMAGE *ptr_result_image)
{
  int   i,j;
  float min,max;
  float **tmppixel,gx,gy;

	int height = ptr_my_image->height;
	int width = ptr_my_image->width;

  tmppixel=(float**)mymalloc2(height,width,sizeof(float));

  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++)
      tmppixel[j][i]=(float)ptr_my_image->pixel[j][i];

  for(j=1;j<height-1;j++)
    for(i=1;i<width-1;i++) {
      if(strcmp(filtername,"roberts")==0) {
	  gx=(float)ptr_my_image->pixel[j+1][i+1]-(float)ptr_my_image->pixel[j][i  ];
	  gy=(float)ptr_my_image->pixel[j+1][i  ]-(float)ptr_my_image->pixel[j][i+1];
	}
	else
	  if(strcmp(filtername,"prewitt")==0) {
	      gx=(float)ptr_my_image->pixel[j+1][i-1]+
		 (float)ptr_my_image->pixel[j+1][i  ]+
		 (float)ptr_my_image->pixel[j+1][i+1]-
		 (float)ptr_my_image->pixel[j-1][i-1]-
		 (float)ptr_my_image->pixel[j-1][i  ]-
 		 (float)ptr_my_image->pixel[j-1][i+1];
	      gy=(float)ptr_my_image->pixel[j-1][i+1]+
		 (float)ptr_my_image->pixel[j  ][i+1]+
		 (float)ptr_my_image->pixel[j+1][i+1]-
		 (float)ptr_my_image->pixel[j-1][i-1]-
		 (float)ptr_my_image->pixel[j  ][i-1]-
 		 (float)ptr_my_image->pixel[j+1][i-1];
	  }
	  else
	    if(strcmp(filtername,"sobel")==0) {
	      gx=(float)ptr_my_image->pixel[j+1][i-1]+
		2*(float)ptr_my_image->pixel[j+1][i  ]+
		 (float)ptr_my_image->pixel[j+1][i+1]-
		 (float)ptr_my_image->pixel[j-1][i-1]-
		2*(float)ptr_my_image->pixel[j-1][i  ]-
 		 (float)ptr_my_image->pixel[j-1][i+1];
	      gy=(float)ptr_my_image->pixel[j-1][i+1]+
		2*(float)ptr_my_image->pixel[j  ][i+1]+
		 (float)ptr_my_image->pixel[j+1][i+1]-
		 (float)ptr_my_image->pixel[j-1][i-1]-
		2*(float)ptr_my_image->pixel[j  ][i-1]-
 		 (float)ptr_my_image->pixel[j+1][i-1];
	  }
	  else {
	    fprintf(stderr,"Filter is not found. Please check the filter name.\n");
	    exit(0);
	  }
      // Calculation of Magnitude (length)
      tmppixel[j][i] = sqrt(gx * gx + gy * gy);  // original magnitude calculation based on Eq.(10.2-10)
      // tmppixel[j][i] = fabs(gx) + fabs(gy); // simplified magnitude calculation based on Eq.(10.2-20)
    }

  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++)
      if(tmppixel[j][i] < 136)
      	tmppixel[j][i] = 0;
      else
      	tmppixel[j][i] = 255;

  for(j = 0; j < height; j++)
    for(i = 0; i < width;i++)
      ptr_result_image->pixel[j][i] = (unsigned char)tmppixel[j][i];
  myfree2((void **)tmppixel,height);
}

void canny(T_IMAGE *ptr_my_image, T_IMAGE *ptr_canny_image, int tH, int tL) {

	int height = ptr_my_image->height;
	int width = ptr_my_image->width;

	int **arrmagnitude = (int**)mymalloc2(height, width, sizeof(int));
	float **arrtan = (float**)mymalloc2(height, width, sizeof(float));

	unsigned char **edgepixel_L = (unsigned char**)mymalloc2(height, width, sizeof(unsigned char));
	unsigned char **edgepixel_H = (unsigned char**)mymalloc2(height, width, sizeof(unsigned char));
	float gx, gy;
	int i, j;
	long sum = 0, mean;

	//gaussian_blur(ptr_my_image, 2.5, 7); 	// FOR CASIA v3 sigma = 4, kernel size = 9
																				// FOR MMU v1 sigma = , kernel size =
	gaussian_filter(ptr_my_image, ptr_my_image, 7, 2.5);
	path = build_result_path(result_dir_path, "gaussian-image.pgm");
	write_output(path, ptr_my_image);

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			edgepixel_L[j][i] = 0;
			edgepixel_H[j][i] = 0;
			arrmagnitude[j][i] = 0;
			arrtan[j][i] = 0;
		}
	}

	int *histo = histogram(ptr_my_image);


	for (i = 0; i < 256; i++) {
		sum += i * histo[i];
	}

	mean = sum / (height*width);
	//printf("mean intensity %ld\n", mean);

	for (j = 1; j < height-1; j++) {
		for (i = 1; i < width-1; i++) {
			gx=(float)ptr_my_image->pixel[j+1][i-1]+
				2*(float)ptr_my_image->pixel[j+1][i  ]+
				 (float)ptr_my_image->pixel[j+1][i+1]-
				 (float)ptr_my_image->pixel[j-1][i-1]-
				2*(float)ptr_my_image->pixel[j-1][i  ]-
		 		 (float)ptr_my_image->pixel[j-1][i+1];

			gy=(float)ptr_my_image->pixel[j-1][i+1]+
				2*(float)ptr_my_image->pixel[j  ][i+1]+
				 (float)ptr_my_image->pixel[j+1][i+1]-
				 (float)ptr_my_image->pixel[j-1][i-1]-
				2*(float)ptr_my_image->pixel[j  ][i-1]-
		 		 (float)ptr_my_image->pixel[j+1][i-1];

			arrmagnitude[j][i] = sqrt(gx * gx + gy * gy);

			arrtan[j][i] = atan(gx / gy) * 180 / PI;
			//printf("%4.2f\t", round(arrtan[j][i]));
		}
		//printf("\n");
	}
	//printf("halo1\n");

	for (j = 1; j < height-1; j++) {
		for (i = 1; i < width-1; i++) {
			if (arrtan[j][i] >= -22.5 && arrtan[j][i]  <= 22.5){
				if ((arrmagnitude[j][i] > arrmagnitude[j][i+1]) && (arrmagnitude[j][i] > arrmagnitude[j][i-1])) {
					edgepixel_L[j][i] = 255;
				}
			}
			else if ((arrtan[j][i] >= -90 && arrtan[j][i] <= 67.5) || (arrtan[j][i] >= 67.5 && arrtan[j][i] <= 90)) {
				if ((arrmagnitude[j][i] > arrmagnitude[j+1][i]) && (arrmagnitude[j][i] > arrmagnitude[j-1][i])) {
					edgepixel_L[j][i] = 255;
				}
			}
			else if (arrtan[j][i] >= -67.5 && arrtan[j][i] <= -22.5) {
				if ((arrmagnitude[j][i] > arrmagnitude[j+1][i+1]) && (arrmagnitude[j][i] > arrmagnitude[j-1][i-1])) {
					edgepixel_L[j][i] = 255;
				}
			}
			else if (arrtan[j][i] >= 22.5 && arrtan[j][i] <= 67.5){
				if ((arrmagnitude[j][i] > arrmagnitude[j+1][i-1]) && (arrmagnitude[j][i] > arrmagnitude[j-1][i+1])) {
					edgepixel_L[j][i] = 255;
				}
			}
			//printf("%d\t", edgepixel[j][i]);
		}
		//printf("\n");
	}

//	double tH = 150;
//	double tL = 50;


	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (arrmagnitude[j][i] >= tL)
			{
				edgepixel_L[j][i] = 255;
			}
		}
	}

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (arrmagnitude[j][i] >= tH)
			{
				edgepixel_H[j][i] = 255;
			}
		}
	}

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (edgepixel_L[j][i] == 255 && edgepixel_H[j][i] == 255) {
				edgepixel_L[j][i] == 0;
			}
		}
	}

	//printf("halo2\n");

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			ptr_canny_image->pixel[j][i] = edgepixel_L[j][i];
		}
	}

	myfree2((void **)edgepixel_L, height);
	myfree2((void **)edgepixel_H, height);
	myfree2((void **)arrtan, height);
	myfree2((void **)arrmagnitude, height);
}
