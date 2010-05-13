#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>
#include "CImg.h"

#define _MODE_565_RGB16BIT(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))

using namespace cimg_library;


int main(int argc, char** argv)
{
    if(argc != 2) {
        printf("usage: %s [filename]\n", argv[0]);
        exit(1);
    }

	int fb_nr = open("/dev/fb", O_RDWR);
	void* fb_void = mmap(0, 2*240*220, PROT_WRITE, MAP_SHARED , fb_nr, 0);
	short* fb = (short*) fb_void;

    CImg<u_int_32> image(argv[1]);

    short c;

    short *Color = new short[396000];

	for(short x = 0; x < image.width; x++)
	{
		for(short y = 0; y < image.height; y++)
		{
		    if(240 * y + x >= 396000)
                continue;

            //printf("R: %d, G: %d, B: %d\n", r, g, b);
            //printf("R: %d, G: %d, B: %d\n", r255, g255, b255);

            u_int_32 pixel = image.data(x,y);
            unsigned char red = pixel << 0 & ~(8);
            unsigned char green = pixel << 8 & ~(8);
            unsigned char blue = pixel << 16 & ~(8);

            c = (blue)/8;
            c |= (green)/4 << 5;
            c |= (red)/8 << 11;

			Color[240 * y + x] = c;
		}
	}

	for(short x = 0; x < image.width; x++)
		for(short y = 0; y < image.height; y++)
		{
            fb[240 * y + x] = Color[240 * y + x];

		}

	return 0;
}
