//
// Desenvolvido por:
//   Pedro Luiz Eira Velha
//   eira@ime.usp.br
//   http://www.ime.usp.br/~eira
//
//   25 de setembro de 2000
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "Bitmap.h"

#define MAXfilterWidth 36
#define MAXfilterHeight 36

//color arrays
int red[MAXfilterWidth * MAXfilterHeight];
int green[MAXfilterWidth * MAXfilterHeight];
int blue[MAXfilterWidth * MAXfilterHeight];



//combsort: bubble sort made faster by using gaps to eliminate turtles
void combsort(int * data, int amount)
{
    int gap = amount;
    int swapped = 0;
    int i;

    while(gap > 1 || swapped)
    {
        //shrink factor 1.3
        gap = (gap * 10) / 13;
        if(gap == 9 || gap == 10) gap = 11;
        if (gap < 1) gap = 1;
        swapped = 0;
        for (i = 0; i < amount - gap; i++)
        {
            int j = i + gap;
            if (data[i] > data[j])
            {
                data[i] += data[j]; 
                data[j] = data[i] - data[j]; 
                data[i] -= data[j]; 
                swapped = 1;
            }
        }
    }
} 


int main( int argc, char *argv[] )
{
  unsigned int x,y;
  tColorImage c1, c2;
  tColorPixel p, tmp;
  unsigned imageX;
  unsigned imageY;
  int filterX;
  int filterY;
  float tiempo;
  struct timeval inicio, fin;



  //Por defecto filtro de 3x3
  int filterHeight = 3;
  int filterWidth  = 3;

  
  setbuf(stdout,NULL);

  if(argc < 3) {
    printf("Uso: %s <fichero entrada.bmp> <fichero salida.bmp> [<filtroX> <filtroY>]\n", argv[0]);
    exit(0);
  } //argc != 3
 
  if (argc == 5) {
       filterHeight = atoi(argv[3]);
       filterWidth  = atoi(argv[4]);
  }

  BMP_Read_Color_Image( argv[1], &c1);
  
  BMP_Create_Color_Image( &c2, c1.rows, c1.cols );

  gettimeofday (&inicio,NULL); 
  printf ("\nTransformación de imagen. ORIGEN %s DESTINO %s\n",argv[1],argv[2]);
  printf ("Calculando |           |"); 
  printf ("\b\b\b\b\b\b\b\b\b\b\b\b");
  
  for(x=0;x<c1.cols;x++){

          if (x % (c1.cols / 10) == 0) {
              printf ("*");
              //printf ("\033[22;47m ");
              //printf ("\033[22;40m"); 
          }
          for(y=0;y<c1.rows;y++){

    	    	int n = 0;
    		//set the color values in the arrays
        	for(filterX = 0; filterX < filterWidth; filterX++)
        	for(filterY = 0; filterY < filterHeight; filterY++)
        	{
			
            		imageX = (x - filterWidth / 2 + filterX + c1.cols) % c1.cols;
            		imageY = (y - filterHeight / 2 + filterY + c1.rows) % c1.rows;
	  		BMP_Read_Color_Pixel(&c1, imageY, imageX, &p);
            		red[n]   = p[0];
            		green[n] = p[1];
            		blue[n]  = p[2];
            		n++;
        	}        

        	combsort(red, filterWidth * filterHeight);
        	combsort(green, filterWidth * filterHeight);
        	combsort(blue, filterWidth * filterHeight);

        	//take the median, if the length of the array is even, take the average of both center values
        	if((filterWidth * filterHeight) % 2 == 1)
        	{
		        tmp[0] = red[filterWidth * filterHeight / 2];
		        tmp[1] = green[filterWidth * filterHeight / 2];
		        tmp[2] = blue[filterWidth * filterHeight / 2];
			BMP_Write_Color_Pixel(&c2, y, x, tmp);
        	}
        	else if(filterWidth >= 2)
        	{
            		tmp[0] = (red[filterWidth * filterHeight / 2] + red[filterWidth * filterHeight / 2 + 1]) / 2;
            		tmp[1] = (green[filterWidth * filterHeight / 2] + green[filterWidth * filterHeight / 2 + 1]) / 2;
            		tmp[2] = (blue[filterWidth * filterHeight / 2] + blue[filterWidth * filterHeight / 2 + 1]) / 2;
			BMP_Write_Color_Pixel(&c2, y, x, tmp);
        	}

	}
	
	
	}
        gettimeofday (&fin,NULL);

        tiempo =  ((fin.tv_sec - inicio.tv_sec) * 1000000 +  (fin.tv_usec - inicio.tv_usec));

        printf ("\nFinalizado en %.3f seg.\n\n",tiempo / 1000000);

  	BMP_Write_Color_Image( argv[2], &c2);

  	BMP_Free_Color_Image(&c1);
  	BMP_Free_Color_Image(&c2);


  	return 0;

  }
