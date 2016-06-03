#ifndef INTENSITYTRANSFORM_H_INCLUDED
#define INTENSITYTRANSFORM_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
//#define    CLK_TCK   CLOCKS_PER_SEC

#define RADIUS 20
#define THETA 180

void binarization(T_IMAGE *ptr_my_image, int threshold);
void complement (T_IMAGE *ptr_my_image);
void gamma_correction (T_IMAGE *ptr_my_image, double gamma);
void scaling(float** array, int height, int width);

#endif // INTENSITYTRANSFORM_H_INCLUDED
