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
    unsigned char r,g,b;
    int r255, g255, b255;
    short color;

	for(x = 0; x < 180; x+=1)
	{
		for(y = 0; y < 220; y+=1)
		{
		    RGBApixel rgba = bmp->GetPixel(x, y);

            r255 = (int)(rgba.Red);
            g255 = (int)(rgba.Green);
            b255 = (int)(rgba.Blue);



            //printf("R: %d, G: %d, B: %d\n", r, g, b);
            //printf("R: %d, G: %d, B: %d\n", r255, g255, b255);

            r = (int)(((double)(r255)/255.0*31.0));
            g = (int)(((double)(g255)/255.0*63.0));
            b = (int)(((double)(b255)/255.0*31.0));

            color = 0;

            r = rgba.Red;
            g = rgba.Green;
            b = rgba.Blue;

            color |= b;
            color |= g << 6;
            color |= r << 11;

			fb[180 * y + x] = color;
		}
	}

	return 0;
}
