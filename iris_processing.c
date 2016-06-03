#include <math.h>
#include "iris_processing.h"

void find_pupillary_limbus_boundary (T_IMAGE *ptr_my_image, T_IMAGE *ptr_roi_image, T_IMAGE *ptr_ori_image, T_IMAGE *ptr_ori_roi_image,
                                     T_PUPIL_INFO *ptr_pupil_info, T_IRIS_BOUNDARY *ptr_iris_boundary)
{
    int j,i;

    int height = ptr_my_image->height;
    int width = ptr_my_image->width;

    int roi_height = ptr_roi_image->height;
    int roi_width = ptr_roi_image->width;

    int radius_pupil = ptr_pupil_info->radius;

    int sum = 0;

    int startx = ptr_pupil_info->a - radius_pupil - 50;
    int stopx = ptr_pupil_info->a + radius_pupil + 50;

    int starty = ptr_pupil_info->b - radius_pupil - 50;
    int stopy = ptr_pupil_info->b + radius_pupil + 50;

    //printf("startx, starty = %d %d\n", startx, starty);
    //printf("stopx, stopy = %d %d\n", stopx, stopy);

    for (j = starty; j < stopy; j++)
    {
        for (i = startx; i < stopx; i++)
        {
            ptr_roi_image->pixel[j-starty][i-startx] = ptr_my_image->pixel[j][i];
//			ptr_ori_roi_image->pixel[j-starty][i-startx] = ptr_ori_image->pixel[j][i];
            if (pow(i - ptr_pupil_info->a, 2) + pow(j - ptr_pupil_info->b, 2) <= pow(radius_pupil, 2))
                ptr_roi_image->pixel[j-starty][i-startx] = 0;
        }
    }
    path = build_result_path(result_dir_path, "gaussian-roi.pgm");
    write_output(path, ptr_roi_image);
    sprintf(roi_path, "%s", path);
//	GtkTextviewAppend(text_field, "Pupil localization and ROI definition . . . OK\n");
    path[0] = '\0';

//	write_output("img/gaussian_roi.pgm", ptr_roi_image);
//	write_output("img/ori_roi.pgm", ptr_ori_roi_image);

    int *HIP = malloc(roi_width * sizeof(int));
    int *HIP_mirror_sum = malloc((roi_width / 2 - 1) * sizeof(int));

    FILE *fp_roi_HIP, *fp_roi_mirror_HIP, *fp_roi_mirror_dHIP;

    path = build_result_path(result_dir_path, "fp_roi_HIP.txt");
    //printf("path %s\n", path);
    fp_roi_HIP = fopen(path, "w");
    path[0] = '\0';

    path = build_result_path(result_dir_path, "fp_roi_mirror_HIP.txt");
    //printf("path %s\n", path);
    fp_roi_mirror_HIP = fopen(path, "w");
    path[0] = '\0';

    path = build_result_path(result_dir_path, "fp_roi_mirror_dHIP.txt");
    //printf("path %s\n", path);
    fp_roi_mirror_dHIP = fopen(path, "w");
    path[0] = '\0';

    for (i = 0; i < roi_width; i++)
        HIP[i] = 0;

    for (i = 0; i < roi_width; i++)
    {
        sum = 0;
        for (j = -3; j <= 3; j++)
        {
            sum += ptr_roi_image->pixel[roi_height/2 + j][i];
        }
        HIP[i] = sum / 7;
    }
    histo_smoothing(HIP, roi_width);

    for (j = 1; j < roi_width; j++)
    {
        fprintf(fp_roi_HIP, "%d %d\n", j, HIP[j]);
    }

    for (i = 0; i < roi_width/2 - 1; i++)
        HIP_mirror_sum[i] = 0;

    for (i = 1; i < roi_width/2; i++)
    {
        HIP_mirror_sum[i - 1] = pow(HIP[roi_width/2 - i],2) + pow(HIP[roi_width/2 + i], 2);
    }

    int *d_HIP_mirror_sum = malloc((roi_width/2 - 2) * sizeof(int));

    int max_dHIP_peak = 0;
    int radius_iris = 0;
    for (i = 1; i < roi_width/2 - 1; i++)
    {
        d_HIP_mirror_sum[i-1] = fabs(HIP_mirror_sum[i] - HIP_mirror_sum[i-1]);
//		if (d_HIP_mirror_sum[i-1] > max_dHIP_peak) {
//			max_dHIP_peak = d_HIP_mirror_sum[i-1];
//			radius_iris = i;
//		}
    }

    histo_smoothing(d_HIP_mirror_sum, roi_width/2 - 2);

    T_PEAK_LOC dHIP_peak;
    dHIP_peak.first = radius_pupil;
    dHIP_peak.second = find_second_peak(d_HIP_mirror_sum, roi_width/2 - 2, radius_pupil);
    T_PEAK_LOC *ptr_dHIP_peak = &dHIP_peak;

//	find_two_peaks(d_HIP_mirror_sum, roi_width/2 - 2, ptr_dHIP_peak);

    radius_iris = ptr_dHIP_peak->second;

    for (j = 0; j < roi_width/2 - 1; j++)
    {
        fprintf(fp_roi_mirror_HIP, "%d %d\n", j, HIP_mirror_sum[j]);
    }

    for (j = 0; j < roi_width/2 - 2; j++)
    {
        fprintf(fp_roi_mirror_dHIP, "%d %d\n", j, d_HIP_mirror_sum[j]);
    }

    ptr_iris_boundary->pupil = radius_pupil;
    ptr_iris_boundary->limbus = radius_iris;

    for (j = starty; j < stopy; j++)
    {
        for (i = startx; i < stopx; i++)
        {
            if (pow(i - ptr_pupil_info->a, 2) + pow(j - ptr_pupil_info->b, 2) > pow(radius_pupil, 2) &&
                    pow(i - ptr_pupil_info->a, 2) + pow(j - ptr_pupil_info->b, 2) < pow(radius_iris, 2))
            {
                ptr_ori_roi_image->pixel[j-starty][i-startx] = ptr_ori_image->pixel[j][i];
            }
        }
    }
    path = build_result_path(result_dir_path, "ori-roi-image.pgm");
    write_output(path, ptr_ori_roi_image);
    sprintf(localized_iris_path, "%s", path);
    printf("localized_iris_path = %s\n", localized_iris_path);
//	GtkTextviewAppend(text_field, "Iris localization . . . OK\n");
    path[0] = '\0';

//	write_output("img/ori_roi_image.pgm", ptr_ori_roi_image);

    fclose(fp_roi_HIP);
    fclose(fp_roi_mirror_HIP);
    fclose(fp_roi_mirror_dHIP);

    free(HIP);
    free(HIP_mirror_sum);
    free(d_HIP_mirror_sum);
//	myfree2((void **)ptr_roi_image->pixel, ptr_roi_image->height);
}

int find_second_peak(int *histo, int size, int radius_pupil)
{
    int i = 0;
    int max_peak = 0;
    int n_peak = 0;
    unsigned char b_peak = 0;
    int j = 0;
    int threshold = 500;
    for (i = radius_pupil + 10; i < size - 2; i++)
    {
        for (j = 1; j <= 8; j++)
        {
            if (histo[i] >= histo[i+j] && histo[i] >= histo[i-j])
                b_peak = 1;
            else
            {
                b_peak = 0;
                break;
            }
        }
        if (b_peak == 1 && histo[i] > max_peak && histo[i] > threshold)
        {
            max_peak = histo[i];
            n_peak = i;
        }
    }
    //printf("n_peak = %d\n", n_peak);

    if (n_peak == 0)
    {
        for (i = radius_pupil + 10; i < size - 2; i++)
        {
            for (j = 1; j <= 8; j++)
            {
                if (histo[i] >= histo[i+j] && histo[i] >= histo[i-j])
                    b_peak = 1;
                else
                {
                    b_peak = 0;
                    break;
                }
            }
            if (b_peak == 1 && histo[i] > max_peak && histo[i] > threshold - 300)
            {
                max_peak = histo[i];
                n_peak = i;
            }
        }
        //printf("n_peak 2 = %d\n", n_peak);
    }

    if (n_peak == 0)
    {
        for (i = radius_pupil + 10; i < size - 2; i++)
        {
            for (j = 1; j <= 5; j++)
            {
                if (histo[i] >= histo[i+j] && histo[i] >= histo[i-j])
                    b_peak = 1;
                else
                {
                    b_peak = 0;
                    break;
                }
            }
            if (b_peak == 1 && histo[i] > max_peak && histo[i] > threshold)
            {
                max_peak = histo[i];
                n_peak = i;
            }
        }
        //printf("n_peak 2 = %d\n", n_peak);
    }



    return n_peak;
}

void find_two_peaks(int *histo, int size, T_PEAK_LOC *ptr_peak_location)
{
    int i;
    int max_peak = 0;
    int max_peak_2 = 0;
    int peak = 0;
    int n_peak = 0;
    int n_peak_2 = 0;
    for (i = 2; i < size - 2; i++)
    {
        if (histo[i] > histo[i-1] && histo[i] > histo[i-2] && histo[i] > histo[i+1] && histo[i] > histo[i+2])
        {
            peak = histo[i];
            //printf("peak %d at %d\n", peak, i);
            //n_peak = i;

            if (peak > max_peak)
            {
                max_peak_2 = max_peak;
                max_peak = peak;

                n_peak_2 = n_peak;
                n_peak = i;
            }
            else if (peak > max_peak_2)
            {
                max_peak_2 = peak;
                n_peak_2 = i;
            }

            //printf("max_peak %d at %d \t max_peak_2 %d at %d\n", max_peak, n_peak, max_peak_2, n_peak_2);

        }
    }

    //printf("max_peak = %d\t max_peak_2 = %d\n", max_peak, max_peak_2);

    int n_temp = 0;
    int temp = 0;

    if (n_peak_2 < n_peak)
    {
        n_temp = n_peak_2;
        temp = max_peak_2;

        n_peak_2 = n_peak;
        max_peak_2 = max_peak;

        n_peak = n_temp;
        max_peak = temp;
    }

    //printf("max_peak = %d\t max_peak_2 = %d\n", max_peak, max_peak_2);

    //printf("n_peak = %d \t n_peak_2 = %d\n", n_peak, n_peak_2);

    ptr_peak_location->first = n_peak;
    ptr_peak_location->second = n_peak_2;
}

void iris_unwrapping(T_IMAGE *ptr_ori_roi_image, T_IRIS_BOUNDARY *ptr_iris_boundary, T_PUPIL_INFO *ptr_pupil_info, T_IMAGE *ptr_unwrapped_image)
{
    int j, i;
    int Rpupil = ptr_iris_boundary->pupil;
    int Rlimbus = ptr_iris_boundary->limbus;
    int a = ptr_pupil_info->a;
    int b = ptr_pupil_info->b;

    printf("Rpupil %d\t Rlimbus %d\t a %d\t b %d\n", Rpupil, Rlimbus, a, b);

    int x, y;
    int theta = ptr_unwrapped_image->width;
    int radius = ptr_unwrapped_image->height;
    int x_r_theta, y_r_theta, xp_theta, yp_theta, xl_theta, yl_theta, x_center, y_center;
    unsigned char **temp_unwrapped = (unsigned char **)mymalloc2(radius, theta * 2, sizeof(unsigned char));
    //printf("theta %d radius %d\n", theta, radius);

    for (j = 0; j < radius; j++)
    {
        for (i = 0; i < theta * 2; i++)
        {
            // find coordinate of the pupillary boundary point at angle = theta
            xp_theta = a + (int) ( (float)Rpupil  * cos( M_PI * (float)i / 180 ) );
            yp_theta = b - (int) ( (float)Rpupil  * sin( M_PI * (float)i / 180 ) );

            // find coordinate of the limbus boundary point  at angle = theta
            xl_theta = a + (int) ( (float)Rlimbus  * cos( M_PI * (float)i / 180 ) );
            yl_theta = b - (int) ( (float)Rlimbus  * sin( M_PI * (float)i / 180 ) );

            // find the intensity of pixel located at ( x_r_theta, y_r_theta) in the origin
            x_r_theta = (int)   (   (1.0 - (float)j/radius ) * xp_theta  +  (float)j/radius * xl_theta   );
            y_r_theta = (int)   (   (1.0 - (float)j/radius ) * yp_theta  +  (float)j/radius * yl_theta   );

            temp_unwrapped[j][i] = ptr_ori_roi_image->pixel[y_r_theta][x_r_theta];
        }
    }

    // Get only the lower half of iris region
    for (j = 0; j < radius; j++)
    {
        for (i = 0; i < theta; i++)
        {
            ptr_unwrapped_image->pixel[j][i] = temp_unwrapped[j][i + theta];
        }
    }

//	FILE *fp_unwrapped_histo;
//	fp_unwrapped_histo = fopen("fp_unwrapped_histo.txt", "w");

    contrast_stretching(ptr_unwrapped_image, ptr_unwrapped_image, 100);

//	int *histo = malloc(L_MAX * sizeof(int));
//	histo = histogram(ptr_unwrapped_image);

//  for(i = 0; i < L_MAX; i++)
//		fprintf(fp_unwrapped_histo,"%d %d\n",i,histo[i]);

    myfree2((void **)temp_unwrapped, radius);
//	fclose(fp_unwrapped_histo);
//	free(histo);
}

void median_filtering (T_IMAGE *ptr_my_iris, int size, T_IMAGE *ptr_result_image)
{
    int i,j,x,y;

    int r = (size - 1)/2;
    int med_matrix[size*size];

    int height = ptr_my_iris->height;
    int width = ptr_my_iris->width;
    int c = 0;

    T_IMAGE padded_image;
    padded_image.height = height + r * 2;
    padded_image.width = width + r * 2;
    padded_image.pixel = (unsigned char**)mymalloc2(padded_image.height, padded_image.width, sizeof(unsigned char));
    T_IMAGE *ptr_padded_image = &padded_image;

    padding(ptr_my_iris, r, ptr_padded_image);

//	for (j = r; j < height - r; j++) {
    for (j = r; j < padded_image.height - r; j++)
    {
//		for (i = r; i < width - r; i++) {
        for (i = r; i < padded_image.width - r; i++)
        {
            c = 0;
            for (x = -r; x <= r; x++)
            {
                for (y = -r; y <= r; y++)
                {
                    //med_matrix[c] = ptr_my_iris->pixel[j+y][i+x];
                    med_matrix[c] = padded_image.pixel[j+y][i+x];
                    c++;
                }
            }
            ptr_result_image->pixel[j-r][i-r] = median_of(med_matrix, size*size);
        }
    }

    myfree2((void**)padded_image.pixel, padded_image.height);
}

void median_encoding (T_IMAGE *ptr_med_small, T_IMAGE *ptr_med_large, T_IMAGE *ptr_result_image, T_IRISCODE *ptr_iriscode)
{
    int i,j;

    int height = ptr_result_image->height;
    int width = ptr_result_image->width;

    int diff = 0;
    int c = 0;
    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            diff = ptr_med_large->pixel[j][i] - ptr_med_small->pixel[j][i];
            if (diff >= 0)
            {
                ptr_result_image->pixel[j][i] = 255;
                ptr_iriscode->bit[c] = 1;
            }
            else
            {
                ptr_result_image->pixel[j][i] = 0;
                ptr_iriscode->bit[c] = 0;
            }
            //printf("med_encoding %f\n", (float)(ptr_med_large->pixel[j][i] - ptr_med_small->pixel[j][i]));
            c++;
        }
    }
}

void gabor_filtering (T_IMAGE *ptr_unwrapped_image, T_IMAGE *ptr_gabor_real_image, T_IMAGE *ptr_gabor_imag_image, T_IRISCODE *ptr_iriscode, T_IMAGE *ptr_otsu_unwrapped_image)
{
    int radius = ptr_unwrapped_image->height;
    int theta = ptr_unwrapped_image->width;

    int x,y;

    //float sigma;
    float u0 = 8;
    float v0 = 1;

    int j,i,k,l;

    int g_size = 15;

    int x0 = (g_size - 1) / 2;//theta/2;
    int y0 = (g_size - 1) / 2;//radius/2;

    float gauss_radius = (g_size - 1) / 2;
    int r = (int)gauss_radius;
    int rows = r * 2 + 1;

    float **matrix2D = (float**)mymalloc2(rows, rows, sizeof(float));
    for (j = 0; j < rows; j++)
        for (i = 0; i < rows; i++)
            matrix2D[j][i] = 0;
    gaussian2Delliptical(matrix2D, gauss_radius, 5, 4, 0);

    float **real = (float**)mymalloc2(rows, rows, sizeof(float));
    float **imag = (float**)mymalloc2(rows, rows, sizeof(float));
    float calc = 0;

//	float rotation = 45 * M_PI / 180;
    for (j = 0; j < g_size; j++)
    {
        for (i = 0; i < g_size; i++)
        {
            calc = -2 * M_PI * (u0 * (i - x0) + v0 * (j - y0)) * M_PI / 180;
            //		printf("calc %f\n", calc);
            real[j][i] = cos (calc);
            imag[j][i] = sin (calc);
        }
    }

    float **real_gabor = (float**)mymalloc2(rows, rows, sizeof(float));
    float **imag_gabor = (float**)mymalloc2(rows, rows, sizeof(float));

    //printf("real\n");
    for (j = 0; j < g_size; j++)
    {
        for (i = 0; i < g_size; i++)
        {
            real_gabor[j][i] = real[j][i] * matrix2D[j][i];
            //		printf("%f ", real_gabor[j][i]);
        }
        //	printf("\n");
    }

    //printf("imaginary\n");
    for (j = 0; j < g_size; j++)
    {
        for (i = 0; i < g_size; i++)
        {
            imag_gabor[j][i] = imag[j][i] * matrix2D[j][i];
            //		printf("%f ", imag_gabor[j][i]);
        }
        //	printf("\n");
    }

    float **real_gabor_scaled = (float**)mymalloc2(rows, rows, sizeof(float));
    float **imag_gabor_scaled = (float**)mymalloc2(rows, rows, sizeof(float));

    for (j = 0; j < g_size; j++)
        for (i = 0; i < g_size; i++)
        {
            real_gabor_scaled[j][i] = real_gabor[j][i];
            imag_gabor_scaled[j][i] = imag_gabor[j][i];
        }
    scaling(real_gabor_scaled, g_size, g_size);
    scaling(imag_gabor_scaled, g_size, g_size);


    T_IMAGE real_image;
    real_image.height = g_size;
    real_image.width = g_size;
    real_image.pixel = (unsigned char**)mymalloc2(rows, rows, sizeof(unsigned char));
    T_IMAGE *ptr_real_image = &real_image;

    T_IMAGE imag_image;
    imag_image.height = g_size;
    imag_image.width = g_size;
    imag_image.pixel = (unsigned char**)mymalloc2(rows, rows, sizeof(unsigned char));
    T_IMAGE *ptr_imag_image = &imag_image;

    for (j = 0; j < g_size; j++)
        for (i = 0; i < g_size; i++)
        {
            real_image.pixel[j][i] = (unsigned char)real_gabor_scaled[j][i];
            imag_image.pixel[j][i] = (unsigned char)imag_gabor_scaled[j][i];
        }

    float **new_pixel_real = (float**)mymalloc2(radius, theta, sizeof(float));
    float **new_pixel_imag = (float**)mymalloc2(radius, theta, sizeof(float));
    for (j = 0; j < radius; j++)
        for (i = 0; i < theta; i++)
        {
            new_pixel_real[j][i] = 0;
            new_pixel_imag[j][i] = 0;
        }

    T_IMAGE padded_image;
    padded_image.height = radius + r * 2;
    padded_image.width = theta + r * 2;
    padded_image.pixel = (unsigned char**)mymalloc2(padded_image.height, padded_image.width, sizeof(unsigned char));
    T_IMAGE *ptr_padded_image = &padded_image;

    padding(ptr_unwrapped_image, r, ptr_padded_image);

    for (j = r; j < padded_image.height-r; j++)
    {
        for (i = r; i < padded_image.width-r; i++)
        {
            for (y = -r; y <= r; y++)
            {
                for (x = -r; x <= r; x++)
                {
                    new_pixel_real[j-r][i-r] += real_gabor[y+r][x+r] * ptr_padded_image->pixel[j+y][i+x];
                    new_pixel_imag[j-r][i-r] += imag_gabor[y+r][x+r] * ptr_padded_image->pixel[j+y][i+x];
                }
            }
            //	printf("new_pixel_real %f\t", new_pixel_real[j-r][i-r]);
            //	printf("new_pixel_imag %f\n", new_pixel_imag[j-r][i-r]);
        }
    }

    for (j = 0; j < radius; j++)
        for (i = 0; i < theta; i++)
        {
            if (new_pixel_real[j][i] >= 0)
                ptr_gabor_real_image->pixel[j][i] = 255;
            else
                ptr_gabor_real_image->pixel[j][i] = 0;
            //	printf("%f\n", new_pixel_real[j][i]);
        }

    for (j = 0; j < radius; j++)
        for (i = 0; i < theta; i++)
        {
            if (new_pixel_imag[j][i] >= 0)
            {
                ptr_gabor_imag_image->pixel[j][i] = 255;
            }
            else
            {
                ptr_gabor_imag_image->pixel[j][i] = 0;
            }
            //	printf("%f\n", new_pixel_imag[j][i]);
        }

    bit_string = (char*) malloc ((iriscode.size + 1) * sizeof(char));
    bit_mask_string = (char*) malloc ((iriscode.size + 1) * sizeof(char));

    for (i = 0; i < ptr_iriscode->size + 1; i++)
    {
        bit_string[i] = '\0';
        bit_mask_string[i] = '\0';
    }


    int c = 0;

    //printf("iriscode :\n");
    for (j = 0; j < radius; j++)
    {
        for (i = 0; i < theta; i++)
        {
            if (new_pixel_real[j][i] >= 0)
            {
                ptr_gabor_real_image->pixel[j][i] = 255;
                ptr_iriscode->bit[c] = 1;
                bit_string[c] = '1';
            }
            else
            {
                ptr_gabor_real_image->pixel[j][i] = 0;
                ptr_iriscode->bit[c] = 0;
                bit_string[c] = '0';
            }
//			printf("%d ",ptr_iriscode->bit[c]);
            c++;
            if (new_pixel_imag[j][i] >= 0)
            {
                ptr_gabor_imag_image->pixel[j][i] = 255;
                ptr_iriscode->bit[c] = 1;
                bit_string[c] = '1';
            }
            else
            {
                ptr_gabor_imag_image->pixel[j][i] = 0;
                ptr_iriscode->bit[c] = 0;
                bit_string[c] = '0';
            }
            c++;
//			printf("%d ",ptr_iriscode->bit[c]);
        }
    }
    //printf("%s\n", ptr_iriscode->bit);

//	for (j = 0; j < ptr_iriscode->size; j++)
//		printf("%d", ptr_iriscode->bit[j]);
//	printf("\n");

    c = 0;
    for (j = 0; j < radius; j++)
    {
        for (i = 0; i < theta; i++)
        {
            if (ptr_otsu_unwrapped_image->pixel[j][i] == 0)
            {
                ptr_iriscode->mask[c] = 0;
                bit_mask_string[c] = '0';
                c++;
                ptr_iriscode->mask[c] = 0;
                bit_mask_string[c] = '0';
                c++;
            }
            else
            {
                ptr_iriscode->mask[c] = 1;
                bit_mask_string[c] = '0';
                c++;
                ptr_iriscode->mask[c] = 1;
                bit_mask_string[c] = '0';
                c++;
            }
        }
    }
//	for (j = 0; j < 2 * radius * theta; j++) {
//		printf("%d\n", ptr_iriscode->bit[j]);
//	}

    path = build_result_path(result_dir_path, "ptr_real_image.pgm");
    write_output(path, ptr_real_image);
    path[0] = '\0';

    path = build_result_path(result_dir_path, "ptr_imag_image.pgm");
    write_output(path, ptr_imag_image);
    path[0] = '\0';

    path = build_result_path(result_dir_path, "ptr_gabor_real_image.pgm");
    write_output(path, ptr_gabor_real_image);
    sprintf(gabor_real_path, "%s", path);
    path[0] = '\0';

    path = build_result_path(result_dir_path, "ptr_gabor_imag_image.pgm");
    write_output(path, ptr_gabor_imag_image);
    sprintf(gabor_imag_path, "%s", path);
    path[0] = '\0';

    myfree2((void**)padded_image.pixel, padded_image.height);
    myfree2((void**)new_pixel_imag, radius);
    myfree2((void**)new_pixel_real, radius);
    myfree2((void**)real_gabor_scaled, rows);
    myfree2((void**)imag_gabor_scaled, rows);
    myfree2((void**)imag_gabor, rows);
    myfree2((void**)real_gabor, rows);
    myfree2((void**)imag, rows);
    myfree2((void**)real, rows);
    myfree2((void**)matrix2D, rows);
}

void histo_smoothing (int *histo, int size)
{
    int i,sum;

    int *avg = malloc(size * sizeof(int));

    for (i = 0; i < size; i++)
        avg[i] = 0;

    for (i = 1; i < size - 1; i++)
    {
        sum = histo[i-1] + histo[i] + histo[i+1];
        avg[i] = sum / 3;
    }

    for (i = 1; i < size - 1; i++)
        histo[i] = avg[i];

    free(avg);
}

void check_otsu(T_IMAGE *ptr_binary_image)
{
    int height = ptr_binary_image->height;
    int width = ptr_binary_image->width;

    int i, j;

    int black = 0;
    int white = 0;

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            if (ptr_binary_image->pixel[j][i] == 0)
                black++;
            else
                white++;
        }
    }
//	printf("white %d black %d\n", white, black);
    if (white > black)
        complement(ptr_binary_image);
}
