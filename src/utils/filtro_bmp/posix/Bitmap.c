/* ============================================================	*/
/* =			   Autor: Dr. Miguel Angel Garcia		      = */
/* ============================================================	*/

/* Basic set of uncompressed BMP functions */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "Bitmap.h"

/* ============================================================	*/
/* =			    Read_Long			      = */
/* ============================================================	*/

unsigned long Read_Long( FILE *f )
{
/* Assemble a long word from 4 bytes read from the given
   open file. The bytes are sorted in little endian order */
   
   char buf[ 8 ];
   unsigned long *tmp;
   
   fgets( buf, 5, f );
   tmp = (unsigned long*)buf;

   return( *tmp );
}

/* ============================================================	*/
/* =			    Read_Short			      = */
/* ============================================================	*/

unsigned short Read_Short( FILE *f )
{
/* Assemble a short word from 2 bytes read from the given
   open file. The bytes are sorted in little endian order */
   
   char buf[ 8 ];
   unsigned short *tmp;
   
   fgets( buf, 3, f );
   tmp = (unsigned short*)buf;

   return( *tmp );
}

/* ============================================================	*/
/* =			    BMP_Create_Gray_Image	      = */
/* ============================================================	*/

int BMP_Create_Gray_Image( tGrayImage *image, unsigned int rows, unsigned int cols )
{
/* Create a gray-level image in memory space given the image 
   descriptor, number of rows and columns. The value of all
   pixels at creation is undefined. The function returns 
   TRUE in case of success and FALSE in case of failure. */
   
	image->rows = rows;
	image->cols = cols;
	image->image = (tGrayPixel*) malloc( image->rows * image->cols * sizeof(tGrayPixel));

	if (image->image == 0) return( FALSE );
	else return( TRUE );
}

/* ============================================================	*/
/* =			    BMP_Create_Color_Image	      = */
/* ============================================================	*/

int BMP_Create_Color_Image( tColorImage *image, unsigned int rows, unsigned int cols )
{
/* Create a color image in memory space given the image 
   descriptor, number of rows and columns. At creation, the 
   value of all pixels is undefined. The function returns 
   TRUE in case of success and FALSE in case of failure. */
   
	image->rows = rows;
	image->cols = cols;
	image->image = (tColorPixel*) malloc( image->rows * image->cols * sizeof(tColorPixel));

	if (image->image == 0) return( FALSE );
	else return( TRUE );
}
	
/* ============================================================	*/
/* =			    BMP_Read_Gray_Image		      = */
/* ============================================================	*/

int BMP_Read_Gray_Image( char *filename, tGrayImage *image )
{
/* Read a BMP image contained in a file with the given filename. 
   The image is interpreted in gray level. The image is returned 
   in the second parameter. The function returns TRUE in case of 
   success and FALSE in case of failure. */
   
	FILE *f;
//	char *st;
	int padding;
	unsigned char buf[ 100 ];
	unsigned int row, col;
	unsigned int r, g, b, tmp;
	char buf2[64*1024];
	
	f = fopen(filename, "rb");
	if (f == NULL) return( FALSE ); /* File does not exist */

	setbuffer(f, buf2, sizeof(buf2));
	
	/* Check signature */
	
	buf[0] = fgetc( f );
	buf[1] = fgetc( f );
	
	if (buf[0] != 'B' || buf[1] != 'M')
		return( FALSE ); /* File is not BMP */

	/* Read image width & height */
	
	fgets( buf, 17, f );		/* skip header bytes */
	image->cols = Read_Long( f );	/* read image width */
	image->rows = Read_Long( f );	/* read image height */
	
	/* Ensure there is no compression */
	
	tmp = Read_Short( f );		/* Skip bitplanes */
	tmp = Read_Short( f );		/* read Color depth */
	if (tmp != 24) return( FALSE ); /* Image is color compressed */
	tmp = Read_Long( f );		/* read compression type */
	if (tmp != 0) return( FALSE ); /* Image is compressed */

	image->image = (tGrayPixel*) malloc( image->rows * image->cols * sizeof(tGrayPixel));

	fgets( buf, 21, f );	/* skip rest of header */
	
	/* Read image pixels */

	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   {
		buf[ 2 ] = fgetc( f );	/* blue */
		buf[ 1 ] = fgetc( f );	/* green */
		buf[ 0 ] = fgetc( f );  /* red */

		r = buf[ 0 ];
		g = buf[ 1 ];
		b = buf[ 2 ];
	
		tmp = (r + g + b) / 3;
		
		image->image[ row * image->cols + col ] = tmp;
	   }
	   
	   /* skip padding */
	   
	   padding = ((int)((double)image->cols / 4.0 + 0.5))*4 - image->cols;
	   if (padding > 0) fgets( buf, padding+1, f ); 
	}
		
	fclose( f );
	return( TRUE );
}

/* ============================================================	*/
/* =			  BMP_Write_Gray_Image		      = */
/* ============================================================	*/

int BMP_Write_Gray_Image( char *filename, tGrayImage *image )
{
/* Write the given gray image into a file with the given filename. */
   
	FILE *f;
//	int st;
	unsigned char buf[ 100 ];
	unsigned int row, col;
//	unsigned int r, g, b;
	long longtmp;
	short shortmp;
	int padding;
	char buf2[64*1024];
	
	f = fopen(filename, "wb");
	if (f == NULL) return( FALSE ); /* File cannot be opened */

	setbuffer(f, buf2, sizeof(buf2));
	
	/* Write header */
	buf[0] = 'B';
	buf[1] = 'M';
	fwrite( buf, 1, 2, f );
	
	padding = ((int)((double)image->cols / 4.0 + 0.5))*4 - image->cols;
	longtmp = image->cols*image->rows*3 + padding*image->rows + 54;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	
	longtmp = 0;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 54;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 40;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = image->cols;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = image->rows;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	shortmp = 1;
	fwrite( &shortmp, sizeof(shortmp), 1, f );
	shortmp = 24;
	fwrite( &shortmp, sizeof(shortmp), 1, f );
	longtmp = 0;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = image->rows*image->cols;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 2834;					/* Hor. resolution */
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 2834;					/* ver. resolution */
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 0;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 0;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
		
	/* Write image pixels */
	
	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   {
		buf[0] = buf[1] = buf[2] = image->image[ row * image->cols + col];
		fwrite( buf, 1, 3, f );
	   }
	   
	   /* padding */
	   padding = ((int)((double)image->cols / 4.0 + 0.5))*4 - image->cols;
	   if (padding > 0) fwrite( buf, 1, padding, f ); 
	}
		
	fclose( f );
        return( TRUE );
}

/* ============================================================	*/
/* =			BMP_Read_Gray_Pixel		      = */
/* ============================================================	*/

int BMP_Read_Gray_Pixel( tGrayImage *image, unsigned int row, unsigned int col, tGrayPixel *value )
{
/* Return the value associated with the pixel located at the
   given row and column of the given gray image */
   
	if (row<0 || row >= image->rows || col<0 || col >= image->cols) 
		return( FALSE );
	*value = image->image[ row * image->cols + col ];
	return( TRUE );
}

/* ============================================================	*/
/* =			   BMP_Write_Gray_Pixel		      = */
/* ============================================================	*/

int BMP_Write_Gray_Pixel( tGrayImage *image, unsigned int row, unsigned int col, tGrayPixel value )
{
/* Set the value associated with the pixel located at the given
   row and column of the given image */
   
	if (row<0 || row >= image->rows || col<0 || col >= image->cols) 
		return( FALSE );
	image->image[ row * image->cols + col ] = value;
	return( TRUE );
}

/* ============================================================	*/
/* =			   BMP_Free_Gray_Image		      = */
/* ============================================================	*/

void BMP_Free_Gray_Image( tGrayImage *image )
{
/* Free dynamic memory utilized by the image */
   
	free( image->image );
}


/* ============================================================	*/
/* =			    BMP_Read_Color_Image	      = */
/* ============================================================	*/

int BMP_Read_Color_Image( char *filename, tColorImage *image )
{
/* Read a BMP image contained in a file with the given filename. 
   The image is interpreted in true color. The image is returned 
   in the second parameter. The function returns TRUE in case of 
   success and FALSE otherwise. */
   
	FILE *f;
//	char *st;
	int padding;
	unsigned char buf[ 100 ];
	unsigned int row, col;
	unsigned int r, g, b, tmp;
	char *buf2;
	
	f = fopen(filename, "rb");
	if (f == NULL) return( FALSE ); /* File does not exist */
/*	
printf("_IO_read_ptr=%p\n", f->_IO_read_ptr);
printf("_IO_read_end=%p\n", f->_IO_read_end);
printf("_IO_read_base=%p\n", f->_IO_read_base);
printf("_IO_write_base=%p\n", f->_IO_write_base);
printf("_IO_write_ptr=%p\n", f->_IO_write_ptr);
printf("_IO_write_end=%p\n", f->_IO_write_end);
printf("_IO_buf_base=%p\n", f->_IO_buf_base);
printf("_IO_buf_end=%p\n", f->_IO_buf_end);
printf("_IO_save_base=%p\n", f->_IO_save_base);
printf("_IO_backup_base=%p\n", f->_IO_backup_base);
printf("_IO_save_end=%p\n", f->_IO_save_end);
*/
	buf2 = malloc(64*1024);
	setbuffer(f, buf2, 64*1024);

/*
printf("buf2=%p, %d\n", buf2, 64*1024);
printf("_IO_read_ptr=%p\n", f->_IO_read_ptr);
printf("_IO_read_end=%p\n", f->_IO_read_end);
printf("_IO_read_base=%p\n", f->_IO_read_base);
printf("_IO_write_base=%p\n", f->_IO_write_base);
printf("_IO_write_ptr=%p\n", f->_IO_write_ptr);
printf("_IO_write_end=%p\n", f->_IO_write_end);
printf("_IO_buf_base=%p\n", f->_IO_buf_base);
printf("_IO_buf_end=%p\n", f->_IO_buf_end);
printf("_IO_save_base=%p\n", f->_IO_save_base);
printf("_IO_backup_base=%p\n", f->_IO_backup_base);
printf("_IO_save_end=%p\n", f->_IO_save_end);
*/	
	/* Check signature */
	
	buf[0] = fgetc( f );
/*
printf("_IO_read_ptr=%p\n", f->_IO_read_ptr);
printf("_IO_read_end=%p\n", f->_IO_read_end);
printf("_IO_read_base=%p\n", f->_IO_read_base);
printf("_IO_write_base=%p\n", f->_IO_write_base);
printf("_IO_write_ptr=%p\n", f->_IO_write_ptr);
printf("_IO_write_end=%p\n", f->_IO_write_end);
printf("_IO_buf_base=%p\n", f->_IO_buf_base);
printf("_IO_buf_end=%p\n", f->_IO_buf_end);
printf("_IO_save_base=%p\n", f->_IO_save_base);
printf("_IO_backup_base=%p\n", f->_IO_backup_base);
printf("_IO_save_end=%p\n", f->_IO_save_end);
*/
	buf[1] = fgetc( f );
	
	if (buf[0] != 'B' || buf[1] != 'M') {
		fclose(f);
		free(buf2);
		return( FALSE ); /* File is not BMP */
	}

	/* Read image width & height */
	
	fgets( buf, 17, f );		/* skip header bytes */
	image->cols = Read_Long( f );	/* read image width */
	image->rows = Read_Long( f );	/* read image height */
	
	/* Ensure there is no compression */
	
	tmp = Read_Short( f );		/* Skip bitplanes */
	tmp = Read_Short( f );		/* read Color depth */
	if (tmp != 24) {
	    free(buf2);
	    return( FALSE ); /* Image is color compressed */
	}

	tmp = Read_Long( f );		/* read compression type */
	if (tmp != 0) return( FALSE ); /* Image is compressed */

	image->image = (tColorPixel*) malloc( image->rows * image->cols * sizeof(tColorPixel));

	fgets( buf, 21, f );	/* skip rest of header */
	
	/* Read image pixels */

	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   {
		b = fgetc( f );
		g = fgetc( f );
		r = fgetc( f );

		image->image[ row * image->cols + col ][0] = r;	
		image->image[ row * image->cols + col ][1] = g;	
		image->image[ row * image->cols + col ][2] = b;	
	   }
	   /* skip padding */
	   
	   padding = ((int)((double)image->cols / 4.0 + 0.5))*4 - image->cols;
	   if (padding > 0) fgets( buf, padding+1, f );    
	}
		
	fclose( f );
	return( TRUE );
}

/* ============================================================	*/
/* =			  BMP_Write_Color_Image		      = */
/* ============================================================	*/

int BMP_Write_Color_Image( char *filename, tColorImage *image )
{
/* Write the given gray image into a file with the given filename. */
   
	FILE *f;
//	int st;
	unsigned char buf[ 100 ];
	unsigned int row, col;
//	unsigned int r, g, b;
	long longtmp;
	short shortmp;
	int padding;
	char buf2[64*1024];

	f = fopen(filename, "wb");
	if (f == NULL) return( FALSE ); /* File cannot be opened */

	setbuffer(f, buf2, sizeof(buf2));
	
	/* Write header */
	buf[0] = 'B';
	buf[1] = 'M';
	fwrite( buf, 1, 2, f );
	
	padding = ((int)((double)image->cols / 4.0 + 0.5))*4 - image->cols;
	longtmp = image->cols*image->rows*3 + padding*image->rows + 54;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	
	longtmp = 0;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 54;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 40;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = image->cols;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = image->rows;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	shortmp = 1;
	fwrite( &shortmp, sizeof(shortmp), 1, f );
	shortmp = 24;
	fwrite( &shortmp, sizeof(shortmp), 1, f );
	longtmp = 0;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = image->rows*image->cols;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 2834;					/* Hor. resolution */
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 2834;					/* ver. resolution */
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 0;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 0;
	fwrite( &longtmp, sizeof(longtmp), 1, f );
		
	/* Write image pixels */
	
	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   { 
		buf[2] = image->image[ row * image->cols + col][0];
		buf[1] = image->image[ row * image->cols + col][1];
		buf[0] = image->image[ row * image->cols + col][2];
		fwrite( buf, 1, 3, f );
	   }
	   
	   /* padding */
	   padding = ((int)((double)image->cols / 4.0 + 0.5))*4 - image->cols;
	   if (padding > 0) fwrite( buf, 1, padding, f ); 
	}
		
	fclose( f );
        return( TRUE );
}

/* ============================================================	*/
/* =			BMP_Read_Color_Pixel		      = */
/* ============================================================	*/

int BMP_Read_Color_Pixel( tColorImage *image, unsigned int row, unsigned int col, tColorPixel *value )
{
/* Return the value associated with the pixel located at the
   given row and column of the given color image */
   	
	if (row<0 || row >= image->rows || col<0 || col >= image->cols) 
		return( FALSE );
	(*value)[0] = image->image[ row * image->cols + col ][0];
	(*value)[1] = image->image[ row * image->cols + col ][1];
	(*value)[2] = image->image[ row * image->cols + col ][2];
	return( TRUE );
}

/* ============================================================	*/
/* =			   BMP_Write_Color_Pixel	      = */
/* ============================================================	*/

int BMP_Write_Color_Pixel( tColorImage *image, unsigned int row, unsigned int col, tColorPixel value )
{
/* Set the value associated with the pixel located at the given
   row and column of the given image */
   
	if (row<0 || row >= image->rows || col<0 || col >= image->cols) 
		return( FALSE );
	image->image[ row * image->cols + col ][0] = value[0];
	image->image[ row * image->cols + col ][1] = value[1];
	image->image[ row * image->cols + col ][2] = value[2];
	return( TRUE );
}

/* ============================================================	*/
/* =			   BMP_Free_Color_Image		      = */
/* ============================================================	*/

void BMP_Free_Color_Image( tColorImage *image )
{
/* Free dynamic memory utilized by the image */
   
	free( image->image );
}


