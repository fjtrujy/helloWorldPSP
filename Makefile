# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

BUILD_PRX          = 1
HAVE_KERNEL_PRX    = 1
PSP_LARGE_MEMORY   = 1

TARGET = hello
OBJS = main.o

# INCDIR =
# CFLAGS = -O0 -G0 -Wall -g
# CFLAGS += -fdata-sections -ffunction-sections
# LDFLAGS += -s

LIBS = -lstdc++ -lpspgu -lpspgum -lm -lpspaudio -lpspfpu -lpsppower -lpsprtc
CFLAGS = -O0 -g -G0 -std=gnu99 -ffast-math -fsingle-precision-constant

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Hello World

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

sim:
	/Applications/PPSSPPSDL.app/Contents/MacOS/PPSSPPSDL $(shell pwd)/EBOOT.PBP
