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

#define BTN_LEFTTOP 33788
#define BTN_RIGHTTOP 33730

#define BTN_DOWN 33772
#define BTN_UP 33767
#define BTN_LEFT 33769
#define BTN_RIGHT 33770

#define BTN_OK 33780

#define BTN_ACCEPT 33751
#define BTN_QUIT 33752

#define BTN_NUMBER(x) (x-33666+1)
#define BTN_NUMBER_ZERO 33657

#define BTN_STAR 33719
#define BTN_HASH 33748

#define BTN_VIDEO 33728
#define BTN_LOUPE 33754

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

    int offset = 0;

	while(true)
	{
        for(int i = 0; i < 180; i++)
            for(int j = 0; j < 220; j++)
                fb[240*i + j] = 0;

        for(int i = 0; i < bmp->Width; i++)
            for(int j = 0; j < bmp->Height; j++)
                fb[240*(i+ offset) + j] = bmp->Color[240*i + j];

        select(buttons_fd + 1, &rds, NULL, NULL, NULL);

        if (FD_ISSET(buttons_fd, &rds))
        {
            read(buttons_fd, &key_value, sizeof key_value);

            if(key_value == BTN_UP)
                offset += 10;

            if(key_value == BTN_DOWN)
                offset -= 10;

            if(key_value == BTN_NUMBER_ZERO || key_value == BTN_QUIT)
                break;


        }


	}
	close(buttons_fd);


	return 0;
}
