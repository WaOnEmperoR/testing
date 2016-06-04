#include "padding.h"

void padding(T_IMAGE *ptr_my_image, int pad_size, T_IMAGE *ptr_padded_image)
{
	int new_height = ptr_padded_image->height;
	int new_width = ptr_padded_image->width;

	int height = ptr_my_image->height;
	int width = ptr_my_image->width;

	int j, i;

	for (j = 0; j < new_height; j++) {
		for (i = 0; i < new_width; i++) {
			ptr_padded_image->pixel[j][i] = 0;
		}
	}

	for (j = pad_size; j < new_height - pad_size; j++)
		for (i = pad_size; i < new_width - pad_size; i++)
			ptr_padded_image->pixel[j][i] = ptr_my_image->pixel[j-pad_size][i-pad_size];

}

void unpadding(T_DIMAGE *ptr_padded_image, int pad_size, T_DIMAGE *ptr_my_image)
{
	int pheight = ptr_padded_image->height;
	int pwidth = ptr_padded_image->width;

	int height = ptr_my_image->height;
	int width = ptr_my_image->width;

	int j, i;

	for (j = pad_size; j < pheight - pad_size; j++) {
		for (i = pad_size; i < pwidth - pad_size; i++) {
			ptr_my_image->pixel[j-pad_size][i-pad_size] = ptr_padded_image->pixel[j][i];
		}
	}
}
