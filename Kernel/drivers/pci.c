#include <naiveConsole.h>
#include <stdint.h>

uint32_t pci_read_reg(uint8_t bus, uint8_t device, uint8_t function);

uint32_t getVendorID(uint8_t bus, uint8_t device, uint8_t function) {

	return pci_read_reg(bus, device, function);

 }

void checkDevice(uint8_t bus, uint8_t device) {

	uint32_t vendor = getVendorID(bus, device, 0x0);
	uint32_t irq = pci_read_reg(bus, device, 0x3C);
	uint32_t base1 = pci_read_reg(bus, device, 0x00);
	uint32_t base2 = pci_read_reg(bus, device, 0x0C);
	uint32_t base3 = pci_read_reg(bus, device, 0x30);
	uint32_t base4 = pci_read_reg(bus, device, 0x34);
	uint32_t base5 = pci_read_reg(bus, device, 0x38);

	if(vendor == 0x813910EC) {
		ncPrint("Placa: ");
		ncPrintHex(base1);
		ncPrint(" - ");
		ncPrintHex(base2);
		ncPrint(" - ");
		ncPrintHex(base3);
		ncPrint(" - ");
		ncPrintHex(base4);
		ncPrint(" - ");
		ncPrintHex(base5);
		ncPrint(".");
	}

}

void checkAllBuses() {
	 int i, j;
     uint8_t bus;
     uint8_t device;

     for(i = 0; i < 256; i++) {
     	bus++;
         for(j = 0; j < 32; j++) {
         	device++;
            checkDevice(bus, device);
         }
     }

 }
