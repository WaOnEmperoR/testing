#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "spatialfiltering.h"

//unsigned char median_of(unsigned char *t, int size)
int median_of(int *t, int size)
{
	int tmp;
  int i,j;

//	for(i=0;i<8;i++)
	for (i = 0; i < size; i++)
//		for(j=8;j>i;j--)
		for (j = size; j > i; j--)
			if( t[j-1] < t[j])
			{
				tmp=t[j];
				t[j]=t[j-1];
				t[j-1]=tmp;
			}

	//  fprintf(Fp_log,"\t--->\t");
	//  for(i=0;i<9;i++) fprintf(Fp_log,"%3d ",t[i]);
	//  fprintf(Fp_log,"\t%d\n",t[4]);

//	return t[4];
	return t[size/2];
}

void median_filter(T_IMAGE *ptr_my_image)
{
  int i,j;
//  unsigned char t[9];
	int t[9];

	int height = ptr_my_image->height;
	int width = ptr_my_image->width;
//  unsigned char **tmppixel;
	int **tmppixel;
//  tmppixel=(unsigned char**)mymalloc2(height,width,sizeof(unsigned char));
	tmppixel = (int**)mymalloc2(height, width, sizeof(int));

  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++)
      tmppixel[j][i] = ptr_my_image->pixel[j][i];

  for(j = 1; j < height - 1; j++)
    for(i = 1; i < width - 1; i++)
		{
      t[0]=tmppixel[j-1][i-1];
      t[1]=tmppixel[j-1][i  ];
      t[2]=tmppixel[j-1][i+1];
      t[3]=tmppixel[j ][i-1];
      t[4]=tmppixel[j ][i  ];
      t[5]=tmppixel[j ][i+1];
      t[6]=tmppixel[j+1][i-1];
      t[7]=tmppixel[j+1][i  ];
      t[8]=tmppixel[j+1][i+1];
      ptr_my_image->pixel[j][i]=median_of(t, 8);
    }

  myfree2((void **)tmppixel, height);
}

// Noise image smoothed with 5x5 averaging mask

void smoothing_by_averaging(T_IMAGE *ptr_my_image)
{
  int i, j, ii, jj;
  float ave;
  int height = height;
  int width = ptr_my_image->width;

  unsigned char **tmppixel;

  tmppixel = (unsigned char**)mymalloc2(height, width, sizeof(unsigned char));
  for(j = 0; j < height; j++)
    for(i = 0; i < width;i++)
      tmppixel[j][i] = ptr_my_image->pixel[j][i];

  for(j = (MASK_SIZE-1)/2; j < height - (MASK_SIZE-1)/2; j++)
    for(i = (MASK_SIZE-1)/2; i < width -(MASK_SIZE-1)/2; i++)
		{
      ave = 0;
      for(jj = j - (MASK_SIZE-1)/2; jj < j + (MASK_SIZE-1)/2+1; jj++)
				for(ii = i - (MASK_SIZE-1)/2; ii < i + (MASK_SIZE-1)/2+1; ii++)
				  ave += tmppixel[jj][ii];
		      ptr_my_image->pixel[j][i] = ave / (MASK_SIZE * MASK_SIZE);
    }

  myfree2((void **)tmppixel,height);
}

void gaussian1D(float *matrix, float radius)
{
	int r = (int)radius;
	int rows = r * 2 + 1;
	//float[rows] matrix;
	float sigma = radius/3;
	int index = 0;
	float total = 0;
	int row = 0;
	for (row = -r; row <= r; row++) {
		float distance = row * row;
		if (distance > pow(radius, 2))
			matrix[index] = 0;
		else
			matrix[index] = (float)exp(-(distance)/(2 * sigma * sigma)) / sqrt(2 * M_PI * sigma);
		total += matrix[index];
		index++;
	}

	int i = 0;
	for (i = 0; i < rows; i++) {
		matrix[i] /= total;
		//printf("matrix %f\t", matrix[i]);
	}
}

void gaussian2Delliptical(float **matrix, float radius, float sigmax, float sigmay, int rotation)
{
	int r = (int)radius;
	int rows = r * 2 + 1;
	//if (sigma == 0)
	//	sigma = radius/3;

	int col, row;
	float total;

	//float sigmax = 3;
	//float sigmay = 2;
	for (col = -r; col <= r; col++) {
		for (row = -r; row <= r; row++) {
			float distance_row = row * row;
			float distance_col = col * col;
			float xg = fabs(row) * cos (rotation * M_PI / 180) - fabs(col) * sin (rotation * M_PI / 180);
			float yg = fabs(row) * sin (rotation * M_PI / 180) + fabs(col) * cos (rotation * M_PI / 180);

			if (distance > pow(radius, 2))
				matrix[col+r][row+r] = 0;
			else
				matrix[col+r][row+r] = (float)exp(-((xg * xg)/(sigmax * sigmax) + (yg * yg)/(sigmay * sigmay))) / (2 * M_PI * sigmax * sigmay);

			total += matrix[col+r][row+r];
		}
	}

	int i,j;
	//printf("gaussian2Delliptical\n");
	for (j = 0; j < rows; j++) {
		for (i = 0; i < rows; i++) {
			matrix[j][i] /= total;
			//printf("%f ", matrix[j][i]);
		}
		//printf("\n");
	}
	//printf("\n");
}

void gaussian2D(float **matrix, float radius, float sigma)
{
	int r = (int)radius;
	int rows = r * 2 + 1;
	if (sigma == 0)
		sigma = radius/3;

	int col, row;
	float total;

	for (col = -r; col <= r; col++) {
		for (row = -r; row <= r; row++) {
			float distance_row = row * row;
			float distance_col = col * col;
			if (distance > pow(radius, 2))
				matrix[col+r][row+r] = 0;
			else
				matrix[col+r][row+r] = (float)exp(-((distance_row)/(2 * sigma * sigma) + (distance_col)/(2 *sigma * sigma))) / sqrt(2 * M_PI * sigma);

			total += matrix[col+r][row+r];
		}
	}

	int i,j;
//	printf("gaussian2D\n");
	for (j = 0; j < rows; j++) {
		for (i = 0; i < rows; i++) {
			matrix[j][i] /= total;
//			printf("%f ", matrix[j][i]);
		}
//		printf("\n");
	}
//	printf("\n");
}

void gaussian_filter(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image, float gauss_radius, float sigma)
{
	int height = ptr_my_image->height;
	int width = ptr_my_image->width;

//	float gauss_radius = 3;
	int r = (int)gauss_radius;
	int rows = r * 2 + 1;

	float **matrix2D = (float**)mymalloc2(rows, rows, sizeof(float));

	gaussian2D(matrix2D, gauss_radius, sigma);

	int i,j,y,x;

	float sum = 0;
	for (j = 0; j < r; j++)
		for (i = 0;i < r; i++)
			sum += matrix2D[j][i];
//	printf("sum gaussian %f\n", sum);

	float **new_pixel = (float**)mymalloc2(height, width, sizeof(float));
	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
			new_pixel[j][i] = 0;

	for (j = r; j < height - r; j++) {
		for (i = r; i < width - r; i++) {
//			float new_pixel = 0;
			for (y = -r; y <= r; y++) {
				for (x = -r; x <= r; x++) {
					new_pixel[j][i] += matrix2D[y+r][x+r] * ptr_my_image->pixel[j+y][i+x];
				}
			}
//			ptr_result_image->pixel[j][i] = new_pixel;
		}
	}

	scaling(new_pixel, height, width);

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			ptr_result_image->pixel[j][i] = 0;
		}
	}

	for (j = r; j < height - r; j++) {
		for (i = r; i < width - r; i++) {
			ptr_result_image->pixel[j][i] = new_pixel[j][i];
		}
	}
	myfree2((void**)matrix2D, rows);
	myfree2((void**)new_pixel, height);
}

// Sharpening using Laplacian Filter
// Implementation of DIP Gonzales, Sec.3.6.3

void sharpening_using_laplacian_filter(T_IMAGE *ptr_my_image)
{
  int   i,j;
  float min,max;
  float **tmppixel;
	int height = ptr_my_image->height;
	int width = ptr_my_image->width;
  int   filter[3][3]=  {

    // Filter 3x3 di bawah bisa diganti dengan berbagai filter Laplacian yang lain
    {0, 1, 0},
    {1,-4, 1},
    {0, 1, 0}


    /* Jenis filter laplacian yang lain, menurut buku Gonzales Fig.3.37
// filter-a
    {0, 1, 0},
    {1,-4, 1},
    {0, 1, 0}

// filter-b
    {1, 1, 1},
    {1,-8, 1},
    {1, 1, 1},

// filter-c
    { 0, -1,  0},
    {-1,  4, -1},
    { 0, -1,  0},

// filter-d
    {-1,-1,-1},
    {-1, 8,-1},
    {-1,-1,-1},

     */
  };


  tmppixel = (float**)mymalloc2(height, width, sizeof(float));

  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++)
      tmppixel[j][i] = (float)ptr_my_image->pixel[j][i];

  for(j = 1; j < height - 1; j++)
    for(i = 1; i < width - 1; i++)
      tmppixel[j][i]=(
		      filter[0][0]*(float)ptr_my_image->pixel[j-1][i-1] +
		      filter[0][1]*(float)ptr_my_image->pixel[j-1][i  ] +
		      filter[0][2]*(float)ptr_my_image->pixel[j-1][i+1] +

		      filter[1][0]*(float)ptr_my_image->pixel[j  ][i-1] +
		      filter[1][1]*(float)ptr_my_image->pixel[j  ][i  ] +
		      filter[1][2]*(float)ptr_my_image->pixel[j  ][i+1] +

		      filter[2][0]*(float)ptr_my_image->pixel[j+1][i-1] +
		      filter[2][1]*(float)ptr_my_image->pixel[j+1][i  ] +
		      filter[2][2]*(float)ptr_my_image->pixel[j+1][i+1]
      );

  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++)
      if(tmppixel[j][i] < 0)
				tmppixel[j][i] = 0;

  // 7 baris berikutnya adalah menambahkan mask yang diperoleh kepada citra asli untuk membuat sharp. apabila anda ingin menampilkan mask-nya saja, maka bagian sharpening-dari sini sampai sharpening-sampai sini harap dibuat sebagai comment/di nonaktifkan

  // sharpening-dari sini

  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++){
      tmppixel[j][i] = (float)ptr_my_image->pixel[j][i]-tmppixel[j][i]; // use - if a or b filter is used
      //      tmppixel[j][i]=(float)image_pixel[j][i]+tmppixel[j][i]; // use + if c or d filter is used
      if(tmppixel[j][i] < 0)
				tmppixel[j][i] = 0;
      if(tmppixel[j][i] > 255)
				tmppixel[j][i] = 255;
    }

  // sharpening-sampai sini

  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++)
      ptr_my_image->pixel[j][i] = (unsigned char)tmppixel[j][i];

  myfree2((void **)tmppixel,height);
}

