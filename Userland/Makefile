include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin
FRACTAL_DATA=fractals_parameters

all: codeModule sampleDataModule

codeModule:
	cd CodeModule; make

sampleDataModule:
	cat $(FRACTAL_DATA) >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd CodeModule; make clean
	rm -rf *.bin


.PHONY: codeModule all clean
