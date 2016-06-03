#include <stdio.h>
#include <stdlib.h>
#include "histopreprocessing.h"

#define    L_MAX     256
#define    L_BASE    5        // base value for connected component extraction
#define    MASK_SIZE 5        // filter mask 5x5
#define    LOW       0        // object is black
#define    MID1      64					// 85					// 64					//128      // object is black
#define 	 MID2			 128						// 170				// 128
#define 	 MID3 	   192
#define    HIGH      255      // background is white
#define    OBJECT_INTENSITY   HIGH
#define 	 PI				 3.1415926536

int* histogram(T_IMAGE *ptr_my_image)
{
//  int histo[L_MAX];
	int* histo = (int*)malloc(L_MAX * sizeof(int));
  int i,j;

  for(i = 0; i < L_MAX; i++)
		histo[i] = 0;

  for(j = 0; j < ptr_my_image->height; j++)
    for(i = 0; i < ptr_my_image->width; i++)
      histo[ptr_my_image->pixel[j][i]]++;

//  for(i = 0; i < L_MAX; i++)
//		fprintf(Fp_log_histo,"%d %d\n",i,histo[i]);

	return histo;
}

void histogram_equalization(T_IMAGE *ptr_my_image, T_IMAGE *ptr_histoeq_image)
{
  int histogram[L_MAX];
  int i,j;
  int s[L_MAX];
  float p[L_MAX];
  float sum_p;

  int height = ptr_my_image->height;
  int width = ptr_my_image->width;

	FILE *fp_histoeq;
	fp_histoeq = fopen("fp_histoeq.txt", "w");

  for(i = 0; i < L_MAX; i++)
		histogram[i] = 0;

  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++)
      histogram[ptr_my_image->pixel[j][i]]++;

  for(i = 0; i < L_MAX; i++)
		fprintf(Fp_log_histo,"%d\t%d\n",i,histogram[i]);  // show the histogram
  //  for(i=0;i<L_MAX;i++) fprintf(Fp_log,"%d\t%d\n",i,histogram[i]);  show the histogram

  // probability of each intensity value
  for(i = 0; i < L_MAX; i++)
		p[i]=(float)histogram[i]/(height*width);

  // s = T(rk) = (L-1) * (p[0] + p[1] + ... + p[k])
  sum_p=0;
  for (i = 0; i< L_MAX;i++) {
    sum_p += p[i];
    s[i] = (int)(((L_MAX-1) * sum_p) + 0.5);
   }

  // mapping
  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++)
//      ptr_my_image->pixel[j][i] = ptr_my_image->pixel[j][i];
				ptr_histoeq_image->pixel[j][i] = ptr_my_image->pixel[j][i];

  for(i = 0; i < L_MAX; i++)
		histogram[i]=0;

  for(j = 0; j < height; j++)
    for(i = 0; i < width; i++)
      histogram[ptr_my_image->pixel[j][i]]++;

  for(i = 0; i < L_MAX; i++)
		fprintf(fp_histoeq,"%d\t%d\n",i,histogram[i]);  // show the intensity histogram

	fclose(fp_histoeq);
}

void histo_median_filter (int * histo, int length, int degree)
{
	int *temp = malloc(5 * sizeof(int));

	int *median_histo = malloc(length * sizeof(int));

	int i, j, c;
	for (i = 0; i < length; i++)
		median_histo[i] = 0;

	for (i = degree; i < length - degree; i++) {
		c = 0;
		for (j = -degree; j <= degree; j++) {
			temp[c] = histo[i+j];
			c++;
		}

		median_histo[i] = median_of(temp, 4);
		//printf("i %d\t median %d\n", i, median_histo[i]);

	}

	for (i = 0; i < length; i++)
		histo[i] = median_histo[i];

  for(i = 0; i < length; i++)
		fprintf(Fp_log_median, "%d %d\n", i, median_histo[i]);

	free(temp);
	free(median_histo);
}

void contrast_stretching(T_IMAGE *ptr_ori_image, T_IMAGE *ptr_cs_ori_image, int threshold)
{
	int *my_histo = malloc (L_MAX * sizeof(int));
	int *my_histo_cs = malloc (L_MAX * sizeof(int));

	my_histo = histogram(ptr_ori_image);

	int width = ptr_ori_image->width;
	int height = ptr_ori_image->height;

//	int threshold = 50;
	int min_intensity = 0;
	int max_intensity = 0;

	int i, j;

	FILE *cs_histo, *cs_histo_2;
	cs_histo = fopen("cs_histo.txt", "w");
	cs_histo_2 = fopen("cs_histo_2.txt", "w");

	//histo = histogram(ptr_my_image);
	for(i = 0; i < L_MAX; i++)
		fprintf(cs_histo,"%d %d\n",i,my_histo[i]);

	for (i = 1; i < L_MAX; i++) {
		if (my_histo[i] > threshold)
			max_intensity = i;
	}
	//printf("max_intensity %d\n", max_intensity);

	for (i = L_MAX - 1; i >= 1; i--) {
		if (my_histo[i] > threshold)
			min_intensity = i;
	}
	//printf("min_intensity %d\n", min_intensity);

	float ** my_pixel = (float**)mymalloc2(height, width, sizeof(float));

	int max_cs_intensity = 0;
	int min_cs_intensity = 0;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
//			ptr_cs_ori_image->pixel[j][i] = (ptr_ori_image->pixel[j][i] - min_intensity) * (255 / (max_intensity - min_intensity));
			my_pixel[j][i] = (ptr_ori_image->pixel[j][i] - min_intensity) * (255 / (max_intensity - min_intensity));
//			printf("cs %f\n", my_pixel[j][i]);
			if (my_pixel[j][i] >= max_cs_intensity)
				max_cs_intensity = my_pixel[j][i];
			if (my_pixel[j][i] <= min_cs_intensity)
				min_cs_intensity = my_pixel[j][i];
		}
	}

	//printf("max_cs_intensity %d\n", max_cs_intensity);
	//printf("min_cs_intensity %d\n", min_cs_intensity);

	scaling(my_pixel, height, width);
/*
	double new_pixel = 0;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			new_pixel = 255 * (((double)my_pixel[j][i] - (double)min_cs_intensity) / (double)max_cs_intensity);
			printf("%f\n", new_pixel);
			ptr_cs_ori_image->pixel[j][i] = (int)new_pixel;
			//ptr_cs_ori_image->pixel[j][i] = my_pixel[j][i];
		}
	}
*/
	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
			ptr_cs_ori_image->pixel[j][i] = (int)my_pixel[j][i];

	my_histo_cs = histogram(ptr_cs_ori_image);
	for(i = 0; i < L_MAX; i++)
		fprintf(cs_histo_2,"%d %d\n",i,my_histo_cs[i]);

	fclose(cs_histo);
	fclose(cs_histo_2);
	free (my_histo);
	free (my_histo_cs);
	myfree2((void**)my_pixel, height);
}
