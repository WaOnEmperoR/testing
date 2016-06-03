#ifndef HISTOPROCESSING_H_INCLUDED
#define HISTOPROCESSING_H_INCLUDED
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

int* histogram(T_IMAGE *ptr_my_image);
void histogram_equalization(T_IMAGE *ptr_my_image, T_IMAGE *ptr_histoeq_image);
void histo_median_filter (int * histo, int length, int degree);
void contrast_stretching(T_IMAGE *ptr_ori_image, T_IMAGE *ptr_cs_ori_image, int threshold);

#endif // HISTOPROCESSING_H_INCLUDED
