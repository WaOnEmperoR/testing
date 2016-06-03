#include "iomalloc.h"

void *mymalloc( int size)
{
    void	 *buf;

    if((buf = (void*)malloc(size)) == NULL)
    {
        fprintf(stderr,"Not enough Memory.\n");
        exit(1);
    }

    return(buf);
}

void **mymalloc2( int n1, int n2, int size)
{
    void	**ptr;
    int	i;

    ptr = mymalloc((int)sizeof(void *)*n1);

    for(i = 0; i < n1; i++)
        ptr[i] = mymalloc(size * n2);

    return (ptr);
}

void ***mymalloc3(int n1, int n2, int n3, int size)
{
    void ***p;

    int i,j;

    //printf("%d %d %d %d\n", n1, n2, n3, size);

    p = mymalloc((int)sizeof(void*)*n1);
    //p = mymalloc(size*n1);

//	p = *(mymalloc2(n1, n2, (int)sizeof(void *) * n1));

    for (i = 0; i < n1; i++)
        p[i] = mymalloc((int)sizeof(void*) * n2);

    for (j = 0; j < n1; j++)
        for (i = 0; i < n2; i++)
            p[j][i] = mymalloc(size * n3);

    return (p);
}

void myfree2(void **ptr,int n1)
{
    int i;

    for (i = 0; i < n1; i++)
        free(ptr[i]);

    free(ptr);
}

void myfree3(void ***ptr, int n1, int n2)
{
    int i,j;

    for (j = 0; j < n1; j++)
        for (i = 0; i < n2; i++)
            free(ptr[j][i]);

    for (i = 0; i < n1; i++)
        free(ptr[i]);

    free(ptr);
}

void myfread( void *ptr, int size, int nitems, FILE *stream, char *name)
{
    if(fread(ptr,size,nitems,stream)!=nitems)
    {
        fprintf(stderr,"Read error.Care about %s.\n",name);
        exit(1);
    }
}


void myfwrite( void *ptr, int size, int nitems, FILE *stream, char *name)
{
    if(fwrite(ptr,size,nitems,stream)!=nitems)
    {
        //fprintf(Fp_log,"Write error.Care about %s.\n",name);
        exit(1);
    }
}


FILE *myfopen( char *filename, char *mode)
{
    FILE	*fp;

    if((fp=fopen(filename,mode))==NULL)
    {
        fprintf(stderr,"Cannot open file %s.\n",filename);
        exit(1);
    }
    return(fp);
}

void myfgets(void *ptr, int size, FILE *stream)
{
    if(fgets(ptr,size,stream)==NULL)
    {
        fprintf(stderr,"Read error.\n");
        exit(1);
    }
}

unsigned char **read_image(char *filename,int *height,int *width)
{
    FILE *fp;
    char line[100];
    int  i,j;
    unsigned char **input_image;

    fp=fopen(filename,"r");
    fseek(fp,0,0);

    myfgets(line,100,fp);
    myfgets(line,100,fp);

    if(line[0]=='#')
        myfgets(line,100,fp);

    sscanf(line,"%d %d",width,height);
    myfgets(line,100,fp);

    input_image=(unsigned char**)mymalloc2(*height,*width,sizeof(unsigned char));

    for(j=0; j<*height; j++)
        for(i=0; i<*width; i++)
            myfread(&input_image[j][i],sizeof(unsigned char),1,fp,"Error in loading input image");

    fclose(fp);

    return input_image;
}

void write_output(char *filename, T_IMAGE *ptr_my_image)
{
    FILE *fp;
    int  i,j;
    //printf("filename = %s\n", filename);
    fp = fopen(filename, "w");
    fprintf(fp, "P5\n%d\t%d\n255\n", ptr_my_image->width, ptr_my_image->height);

    for(j = 0; j < ptr_my_image->height; j++)
        for(i = 0; i < ptr_my_image->width; i++)
        {
            fwrite(&ptr_my_image->pixel[j][i], sizeof(unsigned char), 1, fp);
        }

    fclose(fp);
}
