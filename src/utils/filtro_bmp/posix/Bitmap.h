//
// Desenvolvido por:
//   Pedro Luiz Eira Velha
//   eira@ime.usp.br
//   http://www.ime.usp.br/~eira
//
//   11 de setembro de 2000
//

#ifndef _bitmap_h
#define _bitmap_h

/* ============================================================	*/
/* =			   Autor: Dr. Miguel Angel Garcia		      = */
/* ============================================================	*/


/* Header file to be included in all files calling functions
   from the bmp library */
   
#define PRIVATE static
 
#define TRUE	1
#define FALSE	0

typedef unsigned char tColorPixel[3];
typedef unsigned char tGrayPixel;

typedef struct
{
	unsigned int rows, cols;
	tGrayPixel *image;
} tGrayImage;

typedef struct
{
	unsigned int rows, cols;
	tColorPixel *image;
} tColorImage;

int BMP_Create_Gray_Image( tGrayImage*, unsigned int, unsigned int );
int BMP_Read_Gray_Image( char*, tGrayImage* );
int BMP_Write_Gray_Image( char*, tGrayImage* );
int BMP_Read_Gray_Pixel( tGrayImage*, unsigned int, unsigned int, tGrayPixel* );
int BMP_Write_Gray_Pixel( tGrayImage*, unsigned int, unsigned int, tGrayPixel );
void BMP_Free_Gray_Image( tGrayImage* );

int BMP_Create_Color_Image( tColorImage*, unsigned int, unsigned int );
int BMP_Read_Color_Image( char*, tColorImage* );
int BMP_Write_Color_Image( char*, tColorImage* );
int BMP_Read_Color_Pixel( tColorImage*, unsigned int, unsigned int, tColorPixel* );
int BMP_Write_Color_Pixel( tColorImage*, unsigned int, unsigned int, tColorPixel );
void BMP_Free_Color_Image( tColorImage* );

#endif
