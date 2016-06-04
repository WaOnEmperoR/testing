#include "thresholding.h"

// Otsu's Optimum Global Thresholding based on Gonzales, Woods, Sec. 10.3.3
// N.Otsu,"A Threshold Selection Method from Gray-Level Histograms," IEEE Trans. Systems, Man, and Cybernetics, Vol.9, No.1, p.62-66

void otsu_optimum_global_thr_binarization(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image)
{
    int i,j;
    int histo[L_MAX];
    float nhisto[L_MAX],sum_p,sum_m,P1[L_MAX],m[L_MAX],mG,between_class_var[L_MAX],max,eta_separability_measure,global_var;
    unsigned char threshold;
    int height = ptr_my_image->height;
    int width = ptr_my_image->width;

    for(i = 0; i < L_MAX; i++)
    {
        histo[i]=0;
        nhisto[i]=0;
    }
    //  for(j=0;j<height;j++)    for(i=0;i<width;i++)      fprintf(stdout,"%d\n",i2mage_pixel[j][i]);   exit(0);

    for(j = 0; j < height; j++)
        for(i = 0; i < width; i++)
            histo[ptr_my_image->pixel[j][i]]++;

    for(i = 0; i < L_MAX; i++)
        nhisto[i] = (float)histo[i]/(height*width);  // normalized histogram
    //  for(i=0;i<L_MAX;i++)    fprintf(stdout,"%d\t%f\n",i,nhisto[i]);  exit(0);

    sum_p=0;

    for (i = 0; i < L_MAX; i++)
    {
        sum_p += nhisto[i];
        P1[i] = sum_p; // P1[i] is the probability that a pixel to be assigned to class C1
    }
    //  for(i=0;i<L_MAX;i++)    fprintf(stdout,"%d\t%f\n",i,P1[i]);  exit(0);

    sum_m=0;

    for(i = 0; i < L_MAX; i++)
    {
        sum_m += i * nhisto[i];
        m[i] = sum_m;  // cumulative mean (average intensity) up to level k Eq.(10.3-8)
    }
    //  for(i=0;i<L_MAX;i++)    fprintf(stdout,"%d\t%f\n",i,m[i]);  exit(0);

    for(mG = 0, i = 0; i < L_MAX; i++)
        mG += i * nhisto[i]; // the average intensity (global mean) Eq.(10.3-9)
    //  fprintf(stderr,"Global Mean: %f\n",mG);

    for(global_var = 0, i = 0; i < L_MAX; i++)
        global_var += ((float)i - mG) * ((float)i- mG) * nhisto[i]; // global variance Eq.(10.3-13)

//  fprintf(Fp_log,"Searching maximum between class variance\n");
    max = -1;

    for(i = 0; i < L_MAX; i++)
        if(P1[i] > 0.1 && P1[i] < 1)
        {
            between_class_var[i]=(mG * P1[i] - m[i]) * (mG * P1[i] - m[i]) / (P1[i] * (1 - P1[i]));
            if(between_class_var[i] > max)
            {
                max = between_class_var[i];
                threshold = i;
            }

//			fprintf(Fp_log,"%d\t%f\t%f\t%f\t%f\n",i,mG,P1[i],m[i],between_class_var[i]);
        }

    eta_separability_measure = between_class_var[threshold] / global_var;
//  fprintf(Fp_log,"binarization threshold obtained by Otsu method is %d with separability measure eta:%f\n",threshold,eta_separability_measure);

    for(j = 0; j < height; j++)
        for(i = 0; i < width; i++)
            if(ptr_my_image->pixel[j][i] > threshold)
                ptr_result_image->pixel[j][i] = HIGH;
            else
                ptr_result_image->pixel[j][i] = LOW;

}


//MULTILEVEL OTSU THRESHOLDING
void otsu_multithreshold(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image, int mlevel)
{
    int i, j, k, m, k1, k2, k3, k4, best_k1, best_k2, best_k3, best_k4;
    int histo[L_MAX];
    float nhisto[L_MAX], sum_p, sum_m, P1, P2, P3, P4, m1, m2, m3, m4, mG, between_class_var, eta_separability_measure, global_var;
    int height = ptr_my_image->height;
    int width = ptr_my_image->width;

    for (i = 0; i < L_MAX; i++)
    {
        histo[i] = 0;
        nhisto[i] = 0;
    }

    for (j = 0; j < height; j++)
        for(i = 0; i < width; i++)
            histo[ptr_my_image->pixel[j][i]]++;

    for (i = 0; i < L_MAX; i++)
        nhisto[i] = (float)histo[i] / (height * width);  // normalized histogram

    for (mG = 0, i = 0; i < L_MAX; i++)
        mG += i * nhisto[i]; // the average intensity (global mean) Eq.(10.3-9)

    for (global_var = 0, i = 0; i < L_MAX; i++)
        global_var += ((float)i - mG) * ((float)i - mG) * nhisto[i]; // global variance Eq.(10.3-13)

    //printf("hello1");

    float **P = (float**)mymalloc2(L_MAX,L_MAX,sizeof(float));
    float **S = (float**)mymalloc2(L_MAX,L_MAX,sizeof(float));
    float **H = (float**)mymalloc2(L_MAX,L_MAX,sizeof(float));

    //initialize
    for (j = 0; j < L_MAX; j++)
        for (i = 0; i < L_MAX; i++)
        {
            P[i][j] = 0;
            S[i][j] = 0;
            H[i][j] = 0;
        }

    //diagonally
    for (i = 1; i < L_MAX; i++)
    {
        P[i][i] = nhisto[i];
        S[i][i] = (float) i * nhisto[i];
    }

    for (i = 1; i < L_MAX-1; i++)
    {
        P[1][i+1] = P[1][i] + nhisto[i+1];
        S[1][i+1] = S[1][i] + (float) (i + 1) * nhisto[i+1];
    }

    for (i = 2; i < L_MAX; i++)
        for (j = i + 1; j < L_MAX; j++)
        {
            P[i][j] = P[1][j] - P[1][i-1];
            S[i][j] = S[1][j] - S[1][i-1];
        }

    for (i = 1; i < L_MAX; i++)
        for (j = i + 1; j < L_MAX; j++)
        {
            if (P[i][j] != 0)
                H[i][j] = (S[i][j] * S[i][j]) / P[i][j];
            else
                H[i][j] = 0;
        }
    //printf("hello2");

    between_class_var = 0;

    if (mlevel == 2)
        for (i = 1; i < L_MAX - mlevel; i++)
        {
            float sq = H[1][i] + H[i+1][255];
            if (between_class_var < sq)
            {
                best_k1 = i;
                between_class_var  = sq;
            }
        }

    if (mlevel == 3)
        for (i = 1; i < L_MAX - mlevel; i++)
            for (j = i + 1; j < L_MAX - mlevel + 1; j++)
            {
                float sq = H[1][i] + H[i+1][j] + H[j+1][255];
                if (between_class_var < sq)
                {
                    best_k1 = i;
                    best_k2 = j;
                    between_class_var  = sq;
                }
            }

    if (mlevel == 4)
        for (i = 1; i < L_MAX - mlevel; i++)
            for (j = i + 1; j < L_MAX - mlevel + 1; j++)
                for (k = j + 1; k < L_MAX - mlevel + 2; k++)
                {
                    float sq = H[1][i] + H[i+1][j] + H[j+1][k] + H[k+1][255];
                    if (between_class_var < sq)
                    {
                        best_k1 = i;
                        best_k2 = j;
                        best_k3 = k;
                        between_class_var = sq;
                    }
                }

    if (mlevel == 5)
        for (i = 1; i < L_MAX - mlevel; i++)
            for (j = i + 1; j < L_MAX - mlevel + 1; j++)
                for (k = j + 1; k < L_MAX - mlevel + 2; k++)
                    for (m = k + 1; m < L_MAX - mlevel + 3; m++)
                    {
                        float sq = H[1][i] + H[i+1][j] + H[j+1][k] + H[k+1][m] + H[m+1][255];
                        if (between_class_var < sq)
                        {
                            best_k1 = i;
                            best_k2 = j;
                            best_k3 = k;
                            best_k4 = m;
                            between_class_var = sq;
                        }
                    }

    //printf("k1:%d\tk2:%d\tk3:%d\tk4:%d\n", best_k1, best_k2, best_k3, best_k4);

    for(j = 0; j < height; j++)
        for(i = 0; i < width; i++)
        {
            if (ptr_my_image->pixel[j][i] < best_k1)
                ptr_result_image->pixel[j][i] = LOW;
            else if (ptr_my_image->pixel[j][i] < best_k2)
                ptr_result_image->pixel[j][i] = MID2; // MID1;
//			else if (ptr_my_image->pixel[j][i] < best_k3)
//				ptr_my_image->pixel[j][i] = MID2;
//			else if (ptr_my_image->pixel[j][i] < best_k4)
//				ptr_my_image->pixel[j][i] = MID3;
            else
                ptr_result_image->pixel[j][i]=HIGH;
        }
}
//END OTSU MULTILEVEL OTSU THRESHOLDING


// Basic Global Thresholding based on Gonzales, Woods, Sec. 10.3.2

void basic_global_thr_binarization(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image)
{
    int i,j;
    int histo[L_MAX];
    int delta_T,T,m1,m2,cnt1,cnt2;
    int height = ptr_my_image->height;
    int width = ptr_my_image->width;

    for(i = 0; i < L_MAX; i++)
        histo[i] = 0;

    for(j = 0; j < height; j++)
        for(i = 0; i < width; i++)
            histo[ptr_my_image->pixel[j][i]]++;

    delta_T=257;

    T=10;

    while(delta_T > 2)
    {
        m1 = 0;
        cnt1 = 0;
        m2 = 0;
        cnt2 = 0;
        for(j = 0; j < height; j++)
            for(i = 0; i < width; i++)
            {
                if(ptr_my_image->pixel[j][i] > T)
                {
                    m2 += ptr_my_image->pixel[j][i];
                    cnt2++;
                }
                else
                {
                    m1 += ptr_my_image->pixel[j][i];
                    cnt1++;
                }
            }

        if(cnt1 == 0)
            m1=0;
        else
            m1=(int)((float)m1 / cnt1);

        if(cnt2 == 0)
            m2=0;
        else
            m2=(int)((float)m2 / cnt2);

        delta_T = abs(T - (unsigned char)(0.5 * (float)(m1 + m2)));
        T = (unsigned char)(0.5 * (float)(m1 + m2));
    }

    fprintf(Fp_log,"binarization threshold obtained by Basic Global Thresholding is %d\n",T);

    for(j = 0; j < height; j++)
        for(i = 0; i < width; i++)
            if(ptr_my_image->pixel[j][i]>T)
                ptr_result_image->pixel[j][i]=HIGH;
            else
                ptr_result_image->pixel[j][i]=LOW;
}

void otsu128(T_IMAGE *ptr_my_image, T_IMAGE *ptr_result_image)
{
    int i,j;
    int histo[L_MAX];
    float nhisto[L_MAX],sum_p,sum_m,P1[L_MAX/3],m[L_MAX/3],mG,between_class_var[L_MAX/3],max,eta_separability_measure,global_var;
    unsigned char threshold;
    int height = ptr_my_image->height;
    int width = ptr_my_image->width;

    for(i = 0; i < L_MAX; i++)
    {
        histo[i]=0;
//    nhisto[i]=0;
    }

    for (i = 0; i < L_MAX/3; i++)
        nhisto[i] = 0;

    for(j = 0; j < height; j++)
        for(i = 0; i < width; i++)
            histo[ptr_my_image->pixel[j][i]]++;

    int sum128 = 0;
    for (i = 0; i < L_MAX/3; i++)
        sum128 += histo[i];

    for(i = 0; i < L_MAX/3; i++)
        nhisto[i] = (float)histo[i]/sum128;//(height*width);  // normalized histogram

    sum_p = 0;

    for (i = 0; i < L_MAX/3; i++)
    {
        sum_p += nhisto[i];
        P1[i] = sum_p; // P1[i] is the probability that a pixel to be assigned to class C1
    }

    sum_m=0;

    for(i = 0; i < L_MAX/3; i++)
    {
        sum_m += i * nhisto[i];
        m[i] = sum_m;  // cumulative mean (average intensity) up to level k Eq.(10.3-8)
    }

    for(mG = 0, i = 0; i < L_MAX/3; i++)
        mG += i * nhisto[i]; // the average intensity (global mean) Eq.(10.3-9)

    for(global_var = 0, i = 0; i < L_MAX/3; i++)
        global_var += ((float)i - mG) * ((float)i- mG) * nhisto[i]; // global variance Eq.(10.3-13)

    max = -1;

    for(i = 0; i < L_MAX/3; i++)
        if(P1[i] > 0.1 && P1[i] < 1)
        {
            between_class_var[i]=(mG * P1[i] - m[i]) * (mG * P1[i] - m[i]) / (P1[i] * (1 - P1[i]));
            if(between_class_var[i] > max)
            {
                max = between_class_var[i];
                threshold = i;
            }
        }

    eta_separability_measure = between_class_var[threshold] / global_var;

    //printf("threshold = %d\n", threshold);

    for(j = 0; j < height; j++)
        for(i = 0; i < width; i++)
            if(ptr_my_image->pixel[j][i] > threshold)
                ptr_result_image->pixel[j][i] = HIGH;
            else
                ptr_result_image->pixel[j][i] = LOW;

}
