#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hough_transform.h"
#include "structs.h"

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

void hough_line(T_IMAGE *ptr_binary_image, T_IMAGE *ptr_accu_image, T_COORDINATE *ptr_hough_coordinate)
{
	int height = ptr_binary_image->height;
	int width = ptr_binary_image->width;

	int rho_height = ptr_accu_image->height;
	int theta_width = ptr_accu_image->width;

	double *sin_array = malloc(theta_width * sizeof(double));
	double *cos_array = malloc(theta_width * sizeof(double));

	int i,j,k;
	int t,r;

	for (t = 0; t < theta_width; t++) {
		sin_array[t] = sin((t - 90) * 2 * M_PI / 360);
		cos_array[t] = cos((t - 90) * 2 * M_PI / 360);
	}

	int rho;

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (ptr_binary_image->pixel[j][i] == HIGH) {
				for (t = 0; t < theta_width; t++) {
					rho = (int) (i * sin_array[t] + j * cos_array[t]);
					if (rho >= 0 && rho < rho_height)
						ptr_accu_image->pixel[rho][t]++;
				}
			}
		}
	}

	int max_hough_value = getHighestHoughValue(ptr_accu_image, ptr_hough_coordinate);
	//printf("max_hough_value %d\n", max_hough_value);
	//printf("%d, %d\n", ptr_hough_coordinate->x, ptr_hough_coordinate->y);

	free (sin_array);
	free (cos_array);
}

void hough_circle(T_IMAGE *ptr_binary_image, T_PUPIL_INFO *ptr_pupil_info)
{
	int height = ptr_binary_image->height;
	int width = ptr_binary_image->width;

	int radius_limit_min = 15;
	int radius_limit_max = 50;
	int c_theta_limit = 360;

	int i,j,k;
	int t,r;

	double **r_sin_array = (double**)mymalloc2(radius_limit_max - radius_limit_min, c_theta_limit, sizeof(double));
	double **r_cos_array = (double**)mymalloc2(radius_limit_max - radius_limit_min, c_theta_limit, sizeof(double));

	int center_a = 0;
	int center_b = 0;

	for (r = radius_limit_min; r < radius_limit_max; r++) {
		for (t = 0; t < c_theta_limit; t++) {
			r_sin_array[r - radius_limit_min][t] = r * sin(t * M_PI / 180);
			r_cos_array[r - radius_limit_min][t] = r * cos(t * M_PI / 180);
		}
	}

    int range = radius_limit_max - radius_limit_min;

	int ***hough_circle_accu = (int***)mymalloc3(height, width, range, sizeof(int));
/*
	int height_start = height/2 - 55;
	int height_stop = height/2 + 55;

	int width_start = width/2 - 55;
	int width_stop = width/2 + 55;
*/

	int height_start = 0;
	int height_stop = height;

	int width_start = 0;
	int width_stop = width;

    for (j = 0; j < height; j++)
    {
		for (i = 0; i < width; i++)
		{
			for (k = 0; k < range; k++)
			{
                hough_circle_accu[j][i][k] = 0;
			}
        }
    }


	//for (j = 0; j < height; j++)
		//for (i = 0; i < width; i++)
			//for (k = 0; k < (radius_limit_max - radius_limit_min); k++)
				//hough_circle_accu[j][i][k] = 0;

	for (j = height_start; j < height_stop; j++) {
		for (i = width_start; i < width_stop; i++) {
			if (ptr_binary_image->pixel[j][i] == HIGH) {
				for (r = radius_limit_min; r < radius_limit_max; r++) {
					for (t = 0; t < c_theta_limit; t++) {
						center_a = (int)(i - r_cos_array[r - radius_limit_min][t]);
						center_b = (int)(j - r_sin_array[r - radius_limit_min][t]);
						//printf("a,b = %d %d\n", center_a, center_b);
						if (center_a >= 0 && center_a < height && center_b >= 0 && center_b < height) {
							hough_circle_accu[center_b][center_a][r - radius_limit_min]++;
						}
					}
				}
			}
		}
	}

	int maxr = 0;
	int maxa = 0;
	int maxb = 0;
	int max = 0;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			for (r = 0; r < (radius_limit_max - radius_limit_min); r++) {
				if (hough_circle_accu[j][i][r] > max) {
					max = hough_circle_accu[j][i][r];
					maxr = r + radius_limit_min;
					maxa = i;
					maxb = j;
				}
			}
		}
	}
	printf("PUPIL TRIPLETS: %d %d %d\n", maxr, maxa, maxb);

	ptr_pupil_info->a = maxa;
	ptr_pupil_info->b = maxb;
	ptr_pupil_info->radius = maxr;

	myfree2((void **)r_sin_array, radius_limit_max - radius_limit_min);
	myfree2((void **)r_cos_array, radius_limit_max - radius_limit_min);
	myfree3((void ***)hough_circle_accu, height, width);

}

int getHighestHoughValue(T_IMAGE *ptr_accu_image, T_COORDINATE *ptr_hough_coordinate) {
	int max = 0;

	int rho_height = ptr_accu_image->height;
	int theta_width = ptr_accu_image->width;

	int r, t;
	int max_r = 0;
	int max_t = 0;
	for (r = 0; r < rho_height; r++) {
		for (t = 0; t < theta_width; t++) {
			if (ptr_accu_image->pixel[r][t] > max) {
				max = ptr_accu_image->pixel[r][t];
				max_r = r;
				max_t = t;
			}
		}
	}
	//printf("max_r %d max_t %d\n", max_r, max_t);

	ptr_hough_coordinate->x = max_t;
	ptr_hough_coordinate->y = max_r;
	return max;
}

