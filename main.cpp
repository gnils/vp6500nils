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

    printf("Saving last Screen...\n");

    short* before = new short[396000];
    for(int i = 0; i < 180; i++)
        for(int j = 0; j < 220; j++)
            before[240*j + i] = fb[240*j + i];

    printf("Opening device: Buttons...\n");

	int buttons_fd= open("/dev/buttons", O_RDWR | O_NONBLOCK);
	if (buttons_fd < 0)
	{
        printf("Error: Cant open Button Device\n");
        return(-1);
    }
    fd_set rds;
    unsigned char key_value;

    FD_ZERO(&rds);
    FD_SET(buttons_fd, &rds);

        printf("1\n");
    select(buttons_fd + 1, &rds, NULL, NULL, NULL);

        printf("2\n");
            key_value = BTN_OK;
            write(buttons_fd, &key_value, 1);
            close(buttons_fd);
/*
    printf("Loading Bitmaps...\n");

	BMP *start = new BMP();
	BMP *car = new BMP();
	BMP *car_mask = new BMP();
	BMP *bg = new BMP();

	start->ReadFromFile("start.bmp");
	car->ReadFromFile("car.bmp");
	car_mask->ReadFromFile("car_mask.bmp");
	bg->ReadFromFile("bg.bmp");

    start->GenerateShortArray();
    car->GenerateShortArray();
    car_mask->GenerateShortArray();
	bg->GenerateShortArray();

    printf("Starting MainLoop...\n");

    for(int i = 0; i < 180; i++)
        for(int j = 0; j < 220; j++)
        {
            fb[240*j + i] = start->Color[240*j + i];
        }


    fd_set rds;
    unsigned char key_value;

    FD_ZERO(&rds);
    FD_SET(buttons_fd, &rds);

    select(buttons_fd + 1, &rds, NULL, NULL, NULL);

    read(buttons_fd, &key_value, sizeof key_value);

    //if(key_value == BTN_OK || key_value == BTN_ACCEPT)
    //{
        int xPos = 80 - car->Width/2;
        int yPos = 110 - car->Height/2;

        while(true)
        {
            key_value = BTN_UP;
            write(buttons_fd, &key_value, 1);
            read(buttons_fd, &key_value, sizeof key_value);

            printf("Button pressed:%d\n", key_value);

            if(key_value == BTN_DOWN || key_value == BTN_NUMBER(8))
                yPos += 1;

            if(key_value == BTN_UP || key_value == BTN_NUMBER(2))
                yPos -= 1;

            if(key_value == BTN_LEFT || key_value == BTN_NUMBER(4))
                xPos -= 1;

            if(key_value == BTN_RIGHT || key_value == BTN_NUMBER(6))
                xPos += 1;

            if(key_value == BTN_VIDEO)
                break;

            for(int i = 0; i < 180; i++)
                for(int j = 0; j < 220; j++)
                {
                    if((i >= xPos && i < xPos+car->Width && j >= yPos && j < yPos+car->Height) &&
                        car_mask->Color[240*(j-yPos) + (i-xPos)] != 0)
                        fb[240*j + i] = car->Color[240*(j-yPos) + (i-xPos)];
                    else
                        fb[240*j + i] = bg->Color[240*j + i];
                }


        }
    //}

    printf("Loading last Screen...\n");

    for(int i = 0; i < 180; i++)
        for(int j = 0; j < 220; j++)
            fb[240*j + i] = before[240*j + i];


    printf("Closing Devices...\n");

	close(buttons_fd);
	close(fb_nr);

*/
	return 0;
}
