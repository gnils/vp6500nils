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


	BMP *bmp = new BMP();

	bmp->ReadFromFile("cube.bmp");
    bmp->GenerateShortArray();

    int fd = open("/dev/buttons", O_RDWR);

    if(fd == -1)
    {
        printf("can't open /dev/sensor\r\n");
        return 1;
    }

    while(true)
    {
        int result = getc(fd);
        printf("%d\n", result);
    }

	for(short x = 0; x < bmp->Width; x++)
		for(short y = 0; y < bmp->Height; y++)
			fb[240 * y + x] = bmp->Color[240 * y + x];

	return 0;
}
