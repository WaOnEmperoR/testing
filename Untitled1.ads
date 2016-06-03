
#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

typedef struct HD_NORM{
	float HD;
	int usable_bit;
}T_HD_NORM;

/*typedef struct image{
	unsigned char **pixel;
	int height;
	int width;
}T_IMAGE;*/

typedef struct image T_IMAGE;
struct image{
	unsigned char **pixel;
	int height;
	int width;
};


typedef struct dimage{
	double **pixel;
	int height;
	int width;
}T_DIMAGE;

typedef struct coordinate{
	int x;
	int y;
}T_COORDINATE;

typedef struct iris_boundary{
	int pupil;
	int limbus;
}T_IRIS_BOUNDARY;

typedef struct kernel{
	double **value;
	int size;
}T_KERNEL;

typedef struct iriscode{
	char *bit;
	int size;
	char *mask;
}T_IRISCODE;

typedef struct pupil_info{
	int a;
	int b;
	int radius;
}T_PUPIL_INFO;

typedef struct peak_loc{
	int first;
	int second;
}T_PEAK_LOC;

typedef struct iris_obj {
	char *class_no;
	char *side;
	char *name;
	char *bit;
	char *bit_mask;
}T_IRIS_OBJ;

FILE    	 *Fp_log, *Fp_log_median, *Fp_log_histo, *Fp_real, *Fp_imag;        // log file
int 			 area = 0;
int				 distance = 0;
int 			 pixelsum = 0;
FILE			 *fp_pupil_size;
char * bit_string;
char * bit_mask_string;

char *open_file_name = NULL;
char *image_file_name = NULL;
char *dir_path = NULL;
char *name_only = NULL;
char *result_dir_path = NULL;
char *path = NULL;

char *gaussian_path = NULL;
char *canny_path = NULL;
char *roi_path = NULL;
char *localized_iris_path = NULL;
char *unwrapped_path = NULL;
char *gabor_real_path = NULL;
char *gabor_imag_path = NULL;

T_IRISCODE iriscode;
T_IRISCODE *ptr_iriscode = &iriscode;

T_IRISCODE irismask;
T_IRISCODE *ptr_irismask = &irismask;

T_IRIS_OBJ iris_object[44];

#endif // STRUCTS_H_INCLUDED



