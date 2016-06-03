#include <stdio.h>
#include <stdlib.h>
#include "iomalloc.h"

int main()
{
    int buffer[2000];
    char FMR[100], line[20];
    unsigned char buf_c[20];
    int c=100, image_width, image_height;
    unsigned char** pixel;

    FILE *fin;
    int character;
    fin = fopen("/home/waonemperor/Documents/MMU_db_input/5/right/chongpkr5.pgm", "rb");

    result_dir_path = "/home/waonemperor/PTIKBiometrik/Result_Iris";

    T_IMAGE ori_roi_image;
    T_IMAGE *ptr_ori_roi_image = &ori_roi_image;

    T_IMAGE ori_image;
    T_IMAGE *ptr_ori_image = &ori_image;

    T_IMAGE roi_image;
    T_IMAGE *ptr_roi_image = &roi_image;

    T_IMAGE my_image;
    T_IMAGE *ptr_my_image = &my_image;

    T_IMAGE canny_image;
    T_IMAGE *ptr_canny_image = &canny_image;

    T_PUPIL_INFO pupil_info;
    T_PUPIL_INFO *ptr_pupil_info = &pupil_info;

    /*printf("%d %d\n", sizeof(unsigned char), sizeof(short));
    int image_height, image_width;
    pixel = read_image("/home/waonemperor/Documents/MMU_db_input/5/right/chongpkr5.pgm", &image_height, &image_width);

    printf("LL %d %d\n", image_height, image_width);

    // CREATE IMAGE	//
    ptr_my_image->height = image_height;
    ptr_my_image->width = image_width;
    ptr_my_image->pixel = (unsigned char **)mymalloc2(image_height, image_width, sizeof(unsigned char));

    for (int j = 0; j < image_height; j++)
    	for (int i = 0; i < image_width; i++)
    		ptr_my_image->pixel[j][i] = pixel[j][i];
    // END CREATE IMAGE //

    path = build_result_path(result_dir_path, "very original.pgm");
    write_output(path, ptr_my_image);
    */

    gaussian_path = malloc((strlen(result_dir_path)+50) * sizeof(char));
    canny_path = malloc((strlen(result_dir_path)+50) * sizeof(char));
    roi_path = malloc((strlen(result_dir_path)+50) * sizeof(char));
    localized_iris_path = malloc((strlen(result_dir_path)+50) * sizeof(char));
    unwrapped_path = malloc((strlen(result_dir_path)+50) * sizeof(char));
    gabor_real_path = malloc((strlen(result_dir_path)+50) * sizeof(char));
    gabor_imag_path = malloc((strlen(result_dir_path)+50) * sizeof(char));

    fgets (line, sizeof(line), fin);
    printf("%s", line);
    fgets (line, sizeof(line), fin);
    printf("%s", line);
    sscanf(line,"%d %d",&image_width,&image_height);
    printf("height %d width %d\n", image_height, image_width);
    fgets (line, sizeof(line), fin);
    printf("%s", line);

    short ptr[image_height][image_width];

    ptr_my_image->height = image_height;
    ptr_my_image->width = image_width;
    ptr_my_image->pixel = (unsigned char **)mymalloc2(image_height, image_width, sizeof(unsigned char));

    for (int i=0; i<image_height; i++)
    {
        for (int j=0; j<image_width; j++)
        {
            fread(buf_c, 1, 1, fin);
            ptr[i][j]=buf_c[0];
        }
    }

    for (int j = 0; j < image_height; j++)
        for (int i = 0; i < image_width; i++)
            ptr_my_image->pixel[j][i] = ptr[j][i];

    // INITIALIZE OBJECTS //
    ptr_ori_image->height = image_height;
    ptr_ori_image->width = image_width;
    ptr_ori_image->pixel = (unsigned char **)mymalloc2(image_height, image_width, sizeof(unsigned char));

    ptr_canny_image->height = image_height;
    ptr_canny_image->width = image_width;
    ptr_canny_image->pixel = (unsigned char **)mymalloc2(image_height, image_width, sizeof(unsigned char));
    // END INITILIAZE OBJECTS //

    // BACKUP ORIGINAL IMAGE //
    for (int j = 0; j < image_height; j++)
        for (int i = 0; i < image_width; i++)
            ptr_ori_image->pixel[j][i] = ptr[j][i];
    // END BACKUP ORIGINAL IMAGE //

    path = build_result_path(result_dir_path, "backup original.pgm");
    write_output(path, ptr_ori_image);

    //previous value 150, 50
    canny(ptr_my_image, ptr_canny_image, 100, 25);
    complement(ptr_canny_image);
    erosion(ptr_canny_image->pixel, ptr_canny_image->height, ptr_canny_image->width);
    complement(ptr_canny_image);

    path = build_result_path(result_dir_path, "canny-image.pgm");
    write_output(path, ptr_canny_image);

    ptr_pupil_info->radius = 0;
    ptr_pupil_info->a = 0;
    ptr_pupil_info->b =0 ;

    hough_circle(ptr_canny_image, ptr_pupil_info);

    roi_image.height = ptr_pupil_info->radius * 2 + 100;
    roi_image.width = ptr_pupil_info->radius * 2 + 100;
    roi_image.pixel = (unsigned char**)mymalloc2(roi_image.height, roi_image.width, sizeof(unsigned char));

    ori_roi_image.height = ptr_pupil_info->radius * 2 + 100;
    ori_roi_image.width = ptr_pupil_info->radius * 2 + 100;
    ori_roi_image.pixel = (unsigned char**)mymalloc2(roi_image.height, roi_image.width, sizeof(unsigned char));

    for (j = 0; j < ori_roi_image.height; j++)
        for (i = 0; i < ori_roi_image.width; i++)
            ori_roi_image.pixel[j][i] = 255;

    printf("-- FINISHED --\n");
    return 0;
}
