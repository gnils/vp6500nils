ARMBASE=/opt/VP5500/toolchain/usr/local/arm/3.3.2
INCLUDEPATH=$(ARMBASE)/include
LIBPATH=$(ARMBASE)/arm-linux/lib
ARMPATH=$(ARMBASE)/bin
TOOLPREFIX=/arm-linux-

###############################################################
#####
##### Compiler, Linker and Tools
#####
###############################################################

CC=$(ARMPATH)$(TOOLPREFIX)gcc
CPP=$(ARMPATH)$(TOOLPREFIX)g++
AS=$(ARMPATH)$(TOOLPREFIX)as
LD=$(ARMPATH)$(TOOLPREFIX)gcc
OC=$(ARMPATH)$(TOOLPREFIX)objcopy
OD=$(ARMPATH)$(TOOLPREFIX)objdump

CPUFLAGS=-mcpu=arm9
OPTFLAGS=-Os
#OPTFLAGS=

CFLAGS=$(CPUFLAGS) -c -Wall -I$(INCLUDEPATH)
ASFLAGS=$(CPUFLAGS) -D --gstabs
ROMLDFLAGS=-lc -s -Wl,-warn-common -helloworld.map
THUMBFLAGS=

PROJECT=vp6500nils

-include Makefile.local

all: $(PROJECT)

$(PROJECT): main.o EasyBMP.o
	$(CPP) $(ROMLDFLAGS) -o $(PROJECT) main.o EasyBMP.o

main.o:
	$(CPP) $(CFLAGS) $(THUMBFLAGS) -L $(LIBPATH) -o main.o main.cpp

EasyBMP.o:
	$(CPP) $(CFLAGS) $(THUMBFLAGS) -L $(LIBPATH) -o EasyBMP.o EasyBMP.cpp


clean:
	$(RM) -v $(PROJECT) *.o *.elf *.bin *.hex *~

### EOF
