#ifndef IOMALLOC_H_INCLUDED
#define IOMALLOC_H_INCLUDED

#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

void *mymalloc( int size);
void **mymalloc2( int n1, int n2, int size);
void ***mymalloc3(int n1, int n2, int n3, int size);
void myfree2(void **ptr,int n1);
void myfree3(void ***ptr, int n1, int n2);
void myfread( void *ptr, int size, int nitems, FILE *stream, char *name);
void myfwrite( void *ptr, int size, int nitems, FILE *stream, char *name);
FILE *myfopen( char *filename, char *mode);
void myfgets(void *ptr, int size, FILE *stream);
unsigned char **read_image(char *filename,int *height,int *width);
void write_output(char *filename, T_IMAGE *ptr_my_image);

#endif // IOMALLOC_H_INCLUDED
