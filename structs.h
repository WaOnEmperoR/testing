#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef struct HD_NORM
{
    float HD;
    int usable_bit;
} T_HD_NORM;

/*typedef struct image{
	unsigned char **pixel;
	int height;
	int width;
}T_IMAGE;*/

typedef struct image T_IMAGE;
struct image
{
    unsigned char **pixel;
    int height;
    int width;
};


typedef struct dimage
{
    double **pixel;
    int height;
    int width;
} T_DIMAGE;

typedef struct coordinate
{
    int x;
    int y;
} T_COORDINATE;

typedef struct iris_boundary
{
    int pupil;
    int limbus;
} T_IRIS_BOUNDARY;

typedef struct kernel
{
    double **value;
    int size;
} T_KERNEL;

typedef struct iriscode
{
    char *bit;
    int size;
    char *mask;
} T_IRISCODE;

typedef struct pupil_info
{
    int a;
    int b;
    int radius;
} T_PUPIL_INFO;

typedef struct peak_loc
{
    int first;
    int second;
} T_PEAK_LOC;

typedef struct iris_obj
{
    char *class_no;
    char *side;
    char *name;
    char *bit;
    char *bit_mask;
} T_IRIS_OBJ;

char* path;
char* result_dir_path;
T_IRIS_OBJ iris_object[44];
int distance;

FILE    	 *Fp_log, *Fp_log_median, *Fp_log_histo, *Fp_real, *Fp_imag;

char *gaussian_path;
char *canny_path;
char *roi_path;
char *localized_iris_path;
char *unwrapped_path;
char *gabor_real_path;
char *gabor_imag_path;

#endif // STRUCTS_H_INCLUDED




