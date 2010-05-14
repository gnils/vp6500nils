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

#define BTN_DOWN 108
#define BTN_UP 103
#define BTN_LEFT 105
#define BTN_RIGHT 106

#define BTN_OK 116

#define BTN_ACCEPT 87
#define BTN_QUIT 88

#define BTN_NUMBER(x) (x+1)
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

    printf("Opening device: Buttons...\n");

	int buttons_fd= open("/dev/buttons", O_RDWR | O_NONBLOCK);
	if (buttons_fd < 0)
	{
        printf("Error: Cant open Button Device\n");
        return(-1);
    }

    printf("Loading Bitmaps...\n");

	BMP *start = new BMP();
	BMP *car = new BMP();
	BMP *bg = new BMP();

	start->ReadFromFile("start.bmp");
	car->ReadFromFile("car.bmp");
	bg->ReadFromFile("bg.bmp");

    start->GenerateShortArray();
    car->GenerateShortArray();
	bg->GenerateShortArray();

    printf("Starting MainLoop...\n");

    for(int i = 0; i < 180; i++)
        for(int j = 0; j < 220; j++)
        {
            fb[240*j + i] = car->Color[240*j + i];
        }


    fd_set rds;
    int key_value;

    FD_ZERO(&rds);
    FD_SET(buttons_fd, &rds);

    select(buttons_fd + 1, &rds, NULL, NULL, NULL);

    int xPos = 80-cube_red->Width/2;
    int yPos = 110-cube_red->Height/2;

    key_value %= 1000;
    read(buttons_fd, &key_value, sizeof key_value);

    if(key_value == BTN_OK || key_value == BTN_ACCEPT)
    {
        while(true)
        {
            key_value %= 1000;
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

            for(int i = 0; i < 180; i++)
                for(int j = 0; j < 220; j++)
                {
                    if(i >= xPos && i < xPos+car->Width && j >= yPos && j < yPos+car->Height)
                        fb[240*j + i] = car->Color[240*(j-yPos) + (i-xPos)];
                    else
                        fb[240*j + i] = bg->Color[240*j + i];
                }


        }
    }
    printf("Closing...\n");

	close(buttons_fd);


	return 0;
}
