include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: shellCodeModule sampleDataModule

shellCodeModule:
	cd ShellCodeModule; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd ShellCodeModule; make clean
	rm -rf *.bin


.PHONY: shellCodeModule all clean
