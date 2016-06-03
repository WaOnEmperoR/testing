#ifndef EDGEDETECT_H_INCLUDED
#define EDGEDETECT_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "spatialfiltering.h"

void edge_detection(T_IMAGE *ptr_my_image, char *filtername, T_IMAGE *ptr_result_image);
void canny(T_IMAGE *ptr_my_image, T_IMAGE *ptr_canny_image, int tH, int tL);

#endif // EDGEDETECT_H_INCLUDED
