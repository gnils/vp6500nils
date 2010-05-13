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

    //short r,g,b;
    short color;
    RGBApixel rgba;

	for(short x = 0; x < bmp->Width; x++)
	{
		for(short y = 0; y < bmp->Height; y++)
		{
		    rgba = bmp->Pixels[x][y];

            //printf("R: %d, G: %d, B: %d\n", r, g, b);
            //printf("R: %d, G: %d, B: %d\n", r255, g255, b255);

            color = (int)((float)(rgba.Red)/255.0f*31.0f);
            color |= (int)((float)(rgba.Green)/255.0f*63.0f) << 6;
            color |= (int)((float)(rgba.Blue)/255.0f*31.0f) << 11;


			fb[240 * y + x] = color;
		}
	}

	return 0;
}
