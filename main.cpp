#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>
#include "EasyBMP.h"

#define _MODE_565_RGB16BIT(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))


int main(void)
{
	int fb_nr = open("/dev/fb", O_RDWR);
	void* fb_void = mmap(0, 2*240*220, PROT_WRITE, MAP_SHARED , fb_nr, 0);
	short* fb = (short*) fb_void;


	BMP *bmp = new BMP();

	bmp->ReadFromFile("cube.bmp");


	int x,y;
    int r,g,b;
	int r255, g255, b255;
    short color;
    RGBApixel rgba;

	for(x = 0; x < bmp->Width; x++)
	{
		for(y = 0; y < bmp->Height; y++)
		{
		    rgba = bmp->GetPixel(x, y);

            r255 = (int)(rgba.Green);
            g255 = (int)(rgba.Red);
            b255 = (int)(rgba.Blue);


            if(r255 > 255)
                printf("RED %d\n", r255);
            if(g255 > 255)
                printf("GREEN %d\n", g255);
            if(b255 > 255)
                printf("BLUE %d\n", b255);

            //printf("R: %d, G: %d, B: %d\n", r, g, b);
            //printf("R: %d, G: %d, B: %d\n", r255, g255, b255);

            r = floor(((double)(r255))/255.0*31.0);
            g = floor(((double)(g255))/255.0*63.0);
            b = floor(((double)(b255))/255.0*31.0);

            color = 0;

            color |= b;
            color |= g << 6;
            color |= r << 11;

			fb[240 * y + x] = color;
		}
	}

	return 0;
}
