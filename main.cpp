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

    int r,g,b;
    short color;
    RGBApixel rgba;

	for(int x = 0; x < bmp->Width; x++)
	{
		for(int y = 0; y < bmp->Height; y++)
		{
		    rgba = bmp->Pixels[x][y];

            //printf("R: %d, G: %d, B: %d\n", r, g, b);
            //printf("R: %d, G: %d, B: %d\n", r255, g255, b255);

            r = floor((double)(rgba.Red)/255.0*31.0);
            g = floor((double)(rgba.Green)/255.0*63.0);
            b = floor((double)(rgba.Blue)/255.0*31.0);

            color = 0;

            color |= b;
            color |= g << 6;
            color |= r << 11;

			fb[240 * y + x] = color;
		}
	}

	return 0;
}
