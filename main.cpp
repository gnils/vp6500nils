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

#define BTN_LEFTTOP 124
#define BTN_RIGHTTOP 66

#define BTN_DOWN 105
#define BTN_UP 103
#define BTN_LEFT 108
#define BTN_RIGHT 106

#define BTN_OK 116

#define BTN_ACCEPT 87
#define BTN_QUIT 88

#define BTN_NUMBER(x) (x-1)
#define BTN_NUMBER_ZERO 11

#define BTN_STAR 55
#define BTN_HASH 84

#define BTN_VIDEO 64
#define BTN_LOUPE 90

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

	BMP *bg = new BMP();

	cube_red->ReadFromFile("tetris_red.bmp");
	cube_blue->ReadFromFile("tetris_blue.bmp");
	cube_yellow->ReadFromFile("tetris_yellow.bmp");
	cube_green->ReadFromFile("tetris_green.bmp");
	cube_bam->ReadFromFile("tetris_bam.bmp");

	bg->ReadFromFile("bg.bmp");

    cube_red->GenerateShortArray();
	cube_blue->GenerateShortArray();
	cube_yellow->GenerateShortArray();
	cube_green->GenerateShortArray();
	cube_bam->GenerateShortArray();

	bg->GenerateShortArray();


    printf("Opening device: Buttons...\n");

	int buttons_fd= open("/dev/buttons", O_RDWR | O_NONBLOCK);
	if (buttons_fd < 0)
	{
        printf("Error: Cant open Button Device\n");
        return(-1);
    }

    printf("Starting MainLoop...\n");

    fd_set rds;
    int key_value;

    FD_ZERO(&rds);
    FD_SET(buttons_fd, &rds);

    select(buttons_fd + 1, &rds, NULL, NULL, NULL);

    int xPos = 80;
    int yPos = 0;

    key_value %= 100;
    read(buttons_fd, &key_value, sizeof key_value);

    if(key_value == BTN_OK)
    {
        while(true)
        {
            key_value %= 100;
            read(buttons_fd, &key_value, sizeof key_value);

            if(key_value == BTN_DOWN || key_value == BTN_NUMBER(8))
                yPos += 1;

            if(key_value == BTN_UP || key_value == BTN_NUMBER(2))
                yPos -= 1;

            if(key_value == BTN_LEFT || key_value == BTN_NUMBER(4))
                xPos -= 1;

            if(key_value == BTN_RIGHT || key_value == BTN_NUMBER(6))
                xPos += 1;

            if(key_value == BTN_NUMBER_ZERO || key_value == BTN_QUIT)
                break;


            //yPos += 1;
            //printf("%d\n", yPos);

            for(int i = 0; i < 180; i++)
                for(int j = 0; j < 220; j++)
                {
                    if(i >= xPos && i < xPos+cube_red->Width && j >= yPos && j < yPos+cube_red->Height)
                        fb[240*j + i] = cube_red->Color[240*(j-yPos) + (i-xPos)];
                    else
                        fb[240*j + i] = bg->Color[240*j + i];
                }


        }
    }
    printf("Closing...\n");

	close(buttons_fd);


	return 0;
}
