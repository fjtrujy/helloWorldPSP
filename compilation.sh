PSP_SDK=$(psp-config --pspsdk-path)

rm -f hello.elf  main.o PARAM.SFO EBOOT.PBP hello.prx hello_strip.elf

psp-gcc -I. -I$PSP_SDK/include -Os -G0 -fdata-sections -ffunction-sections -DSCREEN_DEBUG -D_PSP_FW_VERSION=150   -c -o main.o main.c
psp-ld -L. -L$PSP_SDK/lib main.o -lpspuser -o hello.elf

psp-fixup-imports hello.elf

psp-strip hello.elf -o hello_strip.elf

mksfoex -d MEMSIZE=1 'Hello World' PARAM.SFO
pack-pbp EBOOT.PBP PARAM.SFO NULL  \
                NULL NULL NULL  \
                NULL  hello_strip.elf NULL

# psp-prxgen hello.elf hello.prx

# pack-pbp EBOOT.PBP PARAM.SFO NULL  \
#                 NULL NULL NULL  \
#                 NULL  hello.prx NULL