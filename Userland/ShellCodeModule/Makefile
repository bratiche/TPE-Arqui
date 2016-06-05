include ../Makefile.inc

MODULE=0000-shellCodeModule.bin
SOURCES=$(wildcard *.c)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T shellCodeModule.ld $(SOURCES) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
