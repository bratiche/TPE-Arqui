include ../Makefile.inc

MODULE=0000-codeModule.bin
SOURCES=$(wildcard [^_]*.c)



all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T codeModule.ld _loader.c $(SOURCES) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean print
