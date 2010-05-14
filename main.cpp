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
#define BTN_NUMBER_ZERO 33675

#define BTN_STAR 33719
#define BTN_HASH 33748

#define BTN_VIDEO 33728
#define BTN_LOUPE 33754

int main(void)
{
    printf("Opening Device: FrameBuffer...\n");

	int fb_nr = open("/dev/fb", O_RDWR);
	if(fb_nr < 0)
	{
        printf("Error: Cant open FrameBuffer Device\n");
        return(-1);
    }

	void* fb_void = mmap(0, 2*240*220, PROT_WRITE, MAP_SHARED , fb_nr, 0);
	short* fb = (short*) fb_void;

    printf("Loading Bitmaps...\n");

	BMP *cube_red = new BMP();
	BMP *cube_blue = new BMP();
	BMP *cube_yellow = new BMP();
	BMP *cube_green = new BMP();
	BMP *cube_bam = new BMP();

	cube_red->ReadFromFile("tetris_red.bmp");
	cube_blue->ReadFromFile("tetris_blue.bmp");
	cube_yellow->ReadFromFile("tetris_yellow.bmp");
	cube_green->ReadFromFile("tetris_green.bmp");
	cube_bam->ReadFromFile("tetris_bam.bmp");

    cube_red->GenerateShortArray();
	cube_blue->GenerateShortArray();
	cube_yellow->GenerateShortArray();
	cube_green->GenerateShortArray();
	cube_bam->GenerateShortArray();


    printf("Opening device: Buttons...\n");

	int buttons_fd= open("/dev/buttons", O_RDWR | O_NONBLOCK);
	if (buttons_fd < 0)
	{
        printf("Error: Cant open Button Device\n");
        return(-1);
    }

    printf("Starting MainLoop...\n");

	int ret;
    fd_set rds;
    int key_value;


    int xPos = 150;
    int yPos = 0;


	while(true)
	{
        FD_ZERO(&rds);
        FD_SET(buttons_fd, &rds);

        for(int i = 0; i < 180; i++)
            for(int j = 0; j < 220; j++)
            {
                if(i >= xPos && i < xPos+cube_red->Width && j >= yPos && j < yPos+cube_red->Height)
                    fb[240*j + i] = cube_red->Color[240*(j-yPos) + (i-xPos)];
                else
                    fb[240*j + i] = 0;
            }


        yPos += 10;

        select(buttons_fd + 1, &rds, NULL, NULL, NULL);
        if (FD_ISSET(buttons_fd, &rds))
        {
            read(buttons_fd, &key_value, sizeof key_value);


            if(key_value == BTN_DOWN)
                yPos += 10;

            if(key_value == BTN_LEFT)
                xPos -= 10;

            if(key_value == BTN_RIGHT)
                xPos += 10;

            if(key_value == BTN_NUMBER_ZERO || key_value == BTN_QUIT)
                break;
        }


	}

    printf("Closing...\n");

	close(buttons_fd);


	return 0;
}
