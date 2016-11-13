#include <naiveConsole.h>
#include <io.h>
#include <stdint.h>

#define RTL_VENDOR_ID 0x10EC
#define RTL_DEVICE_ID 0x8139

#define COMMAND_REGISTER 0x4

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

/* Source: http://wiki.osdev.org/PCI */

uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot,uint8_t func, uint8_t offset);
void pciConfigWriteWord (uint8_t bus, uint8_t slot,uint8_t func, uint8_t offset, uint16_t data);
uint8_t pciConfigReadByte (uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void enable_bus_master_bit(uint8_t bus, uint8_t slot,uint8_t func, uint8_t offset);

void checkDevice(uint8_t bus, uint8_t device, uint8_t function) {

	uint16_t vendor_id = pciConfigReadWord(bus, device, function, 0);

	uint16_t device_id = pciConfigReadWord(bus, device, function, 0x2);

	uint16_t io_address = pciConfigReadWord(bus, device, function, 0x10);

	uint16_t io_address1 = pciConfigReadWord(bus, device, function, 0x12);
	
	uint16_t interrupt_line = pciConfigReadWord(bus, device, function, 0x3c);

	uint8_t interrupt_pin = pciConfigReadByte(bus, device, function, 0x3d);

	if (vendor_id == RTL_VENDOR_ID && device_id==RTL_DEVICE_ID){

        enable_bus_master_bit(bus,device,function,COMMAND_REGISTER);

		/*ncPrint("  Interrupt Line: ");
		 ncPrintBin(interrupt_line);
        //ncPrintHex(interrupt_line); 	
		 ncNewline();
		ncPrint("  Interrupt Pin: ");
		//ncPrintBin(interrupt_pin);
        ncPrintHex(interrupt_pin);
        ncNewline();
		ncPrint(" IO Address");
		ncPrintHex(io_address);
		ncPrintHex(io_address1);*/
	}
	
}

void checkAllBuses() {
	 int i, j,k;     

     for(i = 0; i < 256; i++) {
     	for(j = 0; j < 32; j++) {
            for (k=0 ; k < 8; k++){         		
	            checkDevice(i,j,k);	            
        	}
         }
     }

 }

/* Enable bus master bit for RTL8139 */
void enable_bus_master_bit (uint8_t bus, uint8_t slot,uint8_t func, uint8_t pci_register){	
	    
	uint16_t cmd;    
 	cmd = pciConfigReadWord(bus,slot,func,pci_register);        
	cmd |= 0x4;    

	pciConfigWriteWord(bus,slot,func,pci_register,cmd);	
}

uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot,
                             uint8_t func, uint8_t offset) {

    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    write_port_dword (CONFIG_ADDRESS, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (uint16_t)((read_port_dword (CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
 }

/* no anda */
 uint8_t pciConfigReadByte (uint8_t bus, uint8_t slot,
                             uint8_t func, uint8_t offset) {

    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint8_t tmp = 0;
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    write_port_dword (CONFIG_ADDRESS, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = read_port (CONFIG_DATA);
    return (tmp);
 }

 void pciConfigWriteWord (uint8_t bus, uint8_t slot,
                             uint8_t func, uint8_t offset, uint16_t data) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;    
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    write_port_dword (CONFIG_ADDRESS, address);
    /* Write the data */    
    write_port_dword(CONFIG_DATA, data);    
    
 }