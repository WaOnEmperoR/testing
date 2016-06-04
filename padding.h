#ifndef PADDING_H_INCLUDED
#define PADDING_H_INCLUDED
#include "structs.h"

void padding(T_IMAGE *ptr_my_image, int pad_size, T_IMAGE *ptr_padded_image);
void unpadding(T_DIMAGE *ptr_padded_image, int pad_size, T_DIMAGE *ptr_my_image);

#endif // PADDING_H_INCLUDED
