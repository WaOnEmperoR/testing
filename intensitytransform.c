#include <stdio.h>
#include <stdlib.h>
#include "intensitytransform.h"

void binarization(T_IMAGE *ptr_my_image, int threshold)
{
	int height = ptr_my_image->height;
	int width = ptr_my_image->width;

	unsigned char **output = (unsigned char**)mymalloc2(height,width,sizeof(unsigned char));

	int j, i;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (ptr_my_image->pixel[j][i] <= threshold)
				output[j][i] = 0;
			else
				output[j][i] = 255;
		}
	}

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			ptr_my_image->pixel[j][i] = output[j][i];
		}
	}
}

void complement (T_IMAGE *ptr_my_image)
{
	int i, j;
	int height = ptr_my_image->height;
	int width = ptr_my_image->width;

	unsigned char **negoutput = (unsigned char**)mymalloc2(height, width, sizeof(unsigned char));

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			negoutput[j][i] = L_MAX - 1 - ptr_my_image->pixel[j][i];
		}
	}

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			ptr_my_image->pixel[j][i] = negoutput[j][i];
		}
	}
	//printf("complement\n");
	myfree2((void **)negoutput,height);
}

void gamma_correction (T_IMAGE *ptr_my_image, double gamma)
{
	int i, j;
	int height = ptr_my_image->height;
	int width = ptr_my_image->width;

	float c = L_MAX / pow(L_MAX, gamma);

	unsigned char **temp = (unsigned char**)mymalloc2(height, width, sizeof(unsigned char));

	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
			temp[j][i] = c * pow(ptr_my_image->pixel[j][i], gamma);

	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
			ptr_my_image->pixel[j][i] = temp[j][i];

	myfree2((void**)temp, height);
}

void scaling(float** array, int height, int width)
{
	int i,j;

	float max = 0;
	float min = 0;

	float **temp = (float**)mymalloc2(height, width, sizeof(float));

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (array[j][i] > max)
				max = array[j][i];
			if (array[j][i] < min)
				min = array[j][i];
		}
	}

	float max_fm = 0;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			temp[j][i] = array[j][i] - min;
			if (temp[j][i] > max_fm)
				max_fm = temp[j][i];
		}
	}

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			temp[j][i] = 255 * (temp[j][i] / max_fm);
			array[j][i] = (unsigned char)temp[j][i];
		}
	}

	myfree2((void**)temp, height);
}
