#ifndef MORPHPROCESSING_H_INCLUDED
#define MORPHPROCESSING_H_INCLUDED

#include <stdio.h>
#include <string.h>

void dilation(unsigned char **image_pixel,int height,int width);
void erosion(unsigned char **image_pixel,int height,int width);
void opening(unsigned char **image_pixel,int height,int width);
void closing(unsigned char **image_pixel,int height,int width);

#endif // MORPHPROCESSING_H_INCLUDED
