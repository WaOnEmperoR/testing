#ifndef IRIS_PROCESSING_H_INCLUDED
#define IRIS_PROCESSING_H_INCLUDED
#include "structs.h"
#include "iomalloc.h"

void find_pupillary_limbus_boundary (T_IMAGE *ptr_my_image, T_IMAGE *ptr_roi_image, T_IMAGE *ptr_ori_image, T_IMAGE *ptr_ori_roi_image,
                                     T_PUPIL_INFO *ptr_pupil_info, T_IRIS_BOUNDARY *ptr_iris_boundary);
int find_second_peak(int *histo, int size, int radius_pupil);
void find_two_peaks(int *histo, int size, T_PEAK_LOC *ptr_peak_location);
void iris_unwrapping(T_IMAGE *ptr_ori_roi_image, T_IRIS_BOUNDARY *ptr_iris_boundary, T_PUPIL_INFO *ptr_pupil_info, T_IMAGE *ptr_unwrapped_image);
void median_filtering (T_IMAGE *ptr_my_iris, int size, T_IMAGE *ptr_result_image);
void median_encoding (T_IMAGE *ptr_med_small, T_IMAGE *ptr_med_large, T_IMAGE *ptr_result_image, T_IRISCODE *ptr_iriscode);
void gabor_filtering (T_IMAGE *ptr_unwrapped_image, T_IMAGE *ptr_gabor_real_image, T_IMAGE *ptr_gabor_imag_image, T_IRISCODE *ptr_iriscode, T_IMAGE *ptr_otsu_unwrapped_image);
void histo_smoothing (int *histo, int size);
void check_otsu(T_IMAGE *ptr_binary_image);

#endif // IRIS_PROCESSING_H_INCLUDED
