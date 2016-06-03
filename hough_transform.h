#ifndef HOUGH_TRANSFORM_H_INCLUDED
#define HOUGH_TRANSFORM_H_INCLUDED
#include "structs.h"

void hough_line(T_IMAGE *ptr_binary_image, T_IMAGE *ptr_accu_image, T_COORDINATE *ptr_hough_coordinate);
void hough_circle(T_IMAGE *ptr_binary_image, T_PUPIL_INFO *ptr_pupil_info);
int getHighestHoughValue(T_IMAGE *ptr_accu_image, T_COORDINATE *ptr_hough_coordinate);

#endif // HOUGH_TRANSFORM_H_INCLUDED
