#include <io.h>
#include <stdint.h>
#include <naiveConsole.h>

#define ioaddr 0xC001
#define BUFFER 0x5F0000

extern uint64_t pci_read_reg();

/* Source: http://wiki.osdev.org/RTL8139 */

void turn_on(){

	write_port(ioaddr + 0x52, 0x0);
}

void clear_buffer(){
	write_port( ioaddr + 0x37, 0x10);
 	while( (read_port(ioaddr + 0x37) & 0x10) != 0) { }
}

void init_receive_buffer(){	

	write_port_dword(ioaddr + 0x30, BUFFER); // send uint32_t memory location to RBSTART (0x30)}
}

void set_imr_isr(){
	write_port_word(ioaddr + 0x3C, 0x0005); // Sets the TOK and ROK bits high
}

void configure_receive_buffer(){
	write_port_dword(ioaddr + 0x44, 0xf | (1 << 7)); // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
}

void enable_receiver_transmiter(){
	write_port(ioaddr + 0x37, 0x0C);
}

void network_init(){

	turn_on();
	clear_buffer();
	init_receive_buffer();
	set_imr_isr();
	configure_receive_buffer();
	enable_receiver_transmiter();

}

void network_handler(){	
	

}