#ifndef SPATIALFILTERING_H_INCLUDED
#define SPATIALFILTERING_H_INCLUDED

#include "structs.h"

#define MASK_SIZE 5

int median_of(int *t, int size);
void median_filter(T_IMAGE *ptr_my_image);
void smoothing_by_averaging(T_IMAGE *ptr_my_image);
void gaussian1D(float *matrix, float radius);
void gaussian2Delliptical(float **matrix, float radius, float sigmax, float sigmay, int rotation);
void gaussian2D(float **matrix, float radius, float sigma);
void gaussian_filter(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image, float gauss_radius, float sigma);
void sharpening_using_laplacian_filter(T_IMAGE *ptr_my_image);

#endif // SPATIALFILTERING_H_INCLUDED
