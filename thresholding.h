#ifndef THRESHOLDING_H_INCLUDED
#define THRESHOLDING_H_INCLUDED
#include "structs.h"

void otsu_optimum_global_thr_binarization(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image);
void otsu_multithreshold(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image, int mlevel);
void basic_global_thr_binarization(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image);
void otsu128(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image);

#endif // THRESHOLDING_H_INCLUDED
