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

    char btn[6] = {'0', '0', '0', '0', '0', '0'};
	int fd = open("/dev/buttons", O_RDWR | O_NONBLOCK);
	if (fd < 0) { printf("cant open buttons device"); return(-1); }

	int status, ret;

	while(true)
	{

        ret = ioctl(fd, BUTTON_IOCTL_GET_STATUS, &status);
        if (ret < 0)
        {
            printf("ioctl invalid status\n");
            break;
        }

        printf("%d\n", status);

        /*for(int i = 0; i < bmp->Width; i++)
            for(int j = 0; j < bmp->Height; j++)
                fb[240*i + j] = bmp->Color[240*i + j];*/

	}
	close(fd);


	return 0;
}
