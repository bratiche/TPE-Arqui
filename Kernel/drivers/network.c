#include <io.h>
#include <stdint.h>
#include <naiveConsole.h>

extern uint64_t pci_read_reg();

/* Source: http://wiki.osdev.org/RTL8139 */

static uint64_t ioaddr;

/*void turn_on(){

	write_port_word( ioaddr + 0x52, 0x0);

}

void clear_buffer(){
	write_port_word( ioaddr + 0x37, 0x10);
 	while( (read_port_word(ioaddr + 0x37) & 0x10) != 0) { }
}

void network_handler(){

	ioaddr=pci_read_reg();

	ncPrintDec(ioaddr);
	turn_on();
	clear_buffer();
	init_receive_buffer();
	set_imr_isr();
	configure_receive_buffer();
	enable_receiver_transmiter();

}

void init_receive_buffer(){

	//(uint32_t *) buffer = malloc ();

	write_port_word(ioaddr + 0x30, buffer); // send uint32_t memory location to RBSTART (0x30)}
}

void set_imr_isr(){
	write_port_word(ioaddr + 0x3C, 0x0005); // Sets the TOK and ROK bits high
}

void configure_receive_buffer(){
	write_port_word(ioaddr + 0x44, 0xf | (1 << 7)); // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
}

void enable_receiver_transmiter(){
	write_port_word(ioaddr + 0x37, 0x0C);
}*/
