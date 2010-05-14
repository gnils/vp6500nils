#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <math.h>

#include <sys/stat.h>
#include <sys/select.h>
#include <errno.h>

#include "EasyBMP.h"

#define BUTTON_IOCTL_BASE               'b'
#define BUTTON_IOCTL_GET_STATUS         _IOR( BUTTON_IOCTL_BASE,7,unsigned int)

#define GETCFDETECT(x)   	   ((x&0x400)>>10)
#define GETPENDETECT(x)   	   ((x&0x100)>>8)
#define GETEVENT(x) (x&0xFF)


int main(void)
{
	int fb_nr = open("/dev/fb", O_RDWR);
	void* fb_void = mmap(0, 2*240*220, PROT_WRITE, MAP_SHARED , fb_nr, 0);
	short* fb = (short*) fb_void;

	BMP *bmp = new BMP();

	bmp->ReadFromFile("cube.bmp");
    bmp->GenerateShortArray();

	int buttons_fd= open("/dev/buttons", O_RDWR | O_NONBLOCK);
	if (buttons_fd < 0)
	{
        printf("cant open buttons device");
        return(-1);
    }

	int ret;
    fd_set rds;
    int key_value;

    FD_ZERO(&rds);
    FD_SET(buttons_fd, &rds);

	while(true)
	{
        ret = select(buttons_fd + 1, &rds, NULL, NULL, NULL);
        if (ret < 0)
        {
            printf("select");
            exit(1);
        }

        if (ret == 0)
        {
            printf("Timeout.\n");
        }
        else if (FD_ISSET(buttons_fd, &rds))
        {
            int ret = read(buttons_fd, &key_value, sizeof key_value);

            if(key_value > 33666 + 1)
            {
                int keynr = key_value - 33666 + 1;
                printf("buttons_value: %d\n", keynr);

                if(keynr == 0)
                    break;
            }

        }


        /*for(int i = 0; i < bmp->Width; i++)
            for(int j = 0; j < bmp->Height; j++)
                fb[240*i + j] = bmp->Color[240*i + j];*/

	}
	close(buttons_fd);


	return 0;
}
