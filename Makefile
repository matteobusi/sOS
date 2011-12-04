#Compiler and linker options
CC=gcc
CFLAGS=-Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./src/headers/ -I./src/headers/boot -I./src/headers/clib -I./src/headers/interrupts -I./src/headers/memory -I./src/headers/multitasking -I./src/headers/structures -I./src/headers/utilities -c -o
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

#c and o files
SSOURCES=$(wildcard ./src/assembly/*.s)
CSOURCES=$(wildcard ./src/sources/*.c)
OBJECTS=$(SSOURCES:.s=.o) $(CSOURCES:.c=.o)

#Executable name
EXEC=./bin/voxos.bin

all: $(OBJECTS) link

clean:
	-rm $(OBJECTS) $(EXEC)

link:
	ld $(LDFLAGS) -o $(EXEC) $(OBJECTS);

.s.o:
	nasm $(ASFLAGS) $<
