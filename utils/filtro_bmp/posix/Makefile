OPTIONS=-g -Wall -Wextra

EXECS=bmp_est

all: $(EXECS)

Bitmap.o: Bitmap.c Bitmap.h
	gcc $(OPTIONS) -c Bitmap.c

bmp_est.o: bmp_est.c Bitmap.o
	gcc $(OPTIONS) -c bmp_est.c

bmp_est: bmp_est.o Bitmap.o
	gcc -o bmp_est bmp_est.o Bitmap.o

clean:
	rm -f *.o
	rm -f bmp_est
