include ../Makefile.inc

MODULE=0000-codeModule.bin
SOURCES_C=$(wildcard *.c) $(wildcard lib/*.c)
OBJECTS_C=$(SOURCES_C:.c=.o)

SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)


all: $(MODULE)

$(MODULE): $(OBJECTS_C) $(OBJECTS_ASM)
	$(LD) -T codeModule.ld $(OBJECTS_C) $(OBJECTS_ASM) -o ../$(MODULE)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o

.PHONY: all clean print
