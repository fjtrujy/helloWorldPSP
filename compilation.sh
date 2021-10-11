rm -f hello.elf  main.o PARAM.SFO EBOOT.PBP EBOOT.PBP

psp-gcc -I. -I/Users/fjtrujy/toolchains/psp/pspdev-new/psp/sdk/include -Os -G0 -fdata-sections -ffunction-sections -DSCREEN_DEBUG -D_PSP_FW_VERSION=150   -c -o main.o main.c
psp-ld -L. -L/Users/fjtrujy/toolchains/psp/pspdev-new/psp/sdk/lib main.o -lpspuser -o hello.elf

psp-fixup-imports hello.elf

# psp-prxgen hello.elf hello.prx

# mksfoex -d MEMSIZE=1 'Hello World' PARAM.SFO

# pack-pbp EBOOT.PBP PARAM.SFO NULL  \
#                 NULL NULL NULL  \
#                 NULL  hello.prx NULL