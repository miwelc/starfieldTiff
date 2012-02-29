PSPSDK=$(shell psp-config --pspsdk-path)
PSPDEV=$(shell psp-config --pspdev-path)
INCLUDE=$(PSPSDK)/include

all:    starfield

clean:
	rm -rf *~ *.o *.elf *.bin *.s *.bin

CC       = psp-gcc
CFLAGS   := -D PSP -I $(INCLUDE) -W -Wall -O2 -G0 -fno-pic -mno-abicalls -w -fomit-frame-pointer

ASM      = psp-as

tiffsdk.o: tiffsdk.c
	# Optimisation level 2 (-O2) MUST be used.
	$(CC) $(CFLAGS) -O2 -S tiffsdk.c -o tiffsdk.s
	$(ASM) tiffsdk.s -o tiffsdk.o

gfx.o: gfx.c gfx.h
	$(CC) $(CFLAGS) -S gfx.c -o gfx.s
	$(ASM) gfx.s -o gfx.o

starfield.o: starfield.c gfx.h
	$(CC) $(CFLAGS) -S starfield.c -o starfield.s
	$(ASM) starfield.s -o starfield.o

starfield: starfield.o gfx.o tiffsdk.o tiffsdk.x
	$(PSPDEV)/bin/psp-ld -T tiffsdk.x -L$(PSPSDK)/lib starfield.o gfx.o tiffsdk.o -o starfield.elf
	$(PSPDEV)/bin/psp-strip -s starfield.elf
	$(PSPDEV)/bin/psp-objcopy -O binary starfield.elf starfield.bin
