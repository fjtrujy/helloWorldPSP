# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

BUILD_PRX          = 0
PSP_LARGE_MEMORY   = 1

TARGET = gprof
OBJS = main.o

CFLAGS = -Os -g -pg
LDFLAGS = -pg

LIBS= -lpspgu -lpspge -lpspdisplay

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = GProfExample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

sim:
	/Applications/PPSSPPSDL.app/Contents/MacOS/PPSSPPSDL $(shell pwd)/EBOOT.PBP
