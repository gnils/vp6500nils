#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <math.h>
#include "EasyBMP.h"

#define _MODE_565_RGB16BIT(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))

#define POWER_ON 0x6C07
int main(void)
{
	int fb_nr = open("/dev/fb", O_RDWR);
	void* fb_void = mmap(0, 2*240*220, PROT_WRITE, MAP_SHARED , fb_nr, 0);
	short* fb = (short*) fb_void;

int button = 4;
	BMP *bmp = new BMP();

	bmp->ReadFromFile("cube.bmp");
    bmp->GenerateShortArray();

    int x0 = 0, y0 = 0;

    char btn[6] = {'0', '0', '0', '0', '0', '0'};
	int fd = open("/dev/buttons", 0);
	if (fd < 0) { printf("cant open buttons device"); return(-1); }

	while(true)
	{	// if a button is released, we read 0x30, if pressed 0x31
		if (read(fd, btn, sizeof btn) != sizeof btn
		{
			printf("cant read buttons!");
			close(fd);
			return(-1);
		}

		if(btn[button] != 0x30)
            x0++;

        y0++;

        for(short x = 0; x < bmp->Width; x++)
            for(short y = 0; y < bmp->Height; y++)
                fb[240 * y + x] = bmp->Color[240 * (y+y0) + (x+x0)];

	}
	close(fd);


	return 0;
}
