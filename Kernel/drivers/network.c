#include <io.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <stdlib.h>

#define ioaddr 0xC000
#define BUFFER 0x5F0000

/* RTL-8139 Registers */
#define TSD0 0x10
#define TSAD0 0x20

	uint8_t mac[6];

	uint8_t rx_buf;
	uint16_t rx_pos;

	uint8_t *tx_buf[4];
	uint8_t tx_pos;
	uint8_t tx_buffers_free;

/* Source: http://wiki.osdev.org/RTL8139 */

void send_packet();

void turn_on(){

	write_port(ioaddr + 0x52, 0x0);
}

void clear_buffer(){
	write_port( ioaddr + 0x37, 0x10);
 	//while( (read_port(ioaddr + 0x37) & 0x10) != 0) { }
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

void get_mac_address() {

	uint8_t mac_address_0 = read_port(ioaddr);	
	uint8_t mac_address_1 = read_port(ioaddr + 1);
	uint8_t mac_address_2 = read_port(ioaddr + 2);
	uint8_t mac_address_3 = read_port(ioaddr + 3);
	uint8_t mac_address_4 = read_port(ioaddr + 4);
	uint8_t mac_address_5 = read_port(ioaddr + 5);

	mac[0]=mac_address_0;
	mac[1]=mac_address_1;
	mac[2]=mac_address_2;
	mac[3]=mac_address_3;
	mac[4]=mac_address_4;
	mac[5]=mac_address_5;

	ncPrintHex(mac[0]);
	ncPrint(":");
	ncPrintHex(mac[1]);
	ncPrint(":");
	ncPrintHex(mac[2]);
	ncPrint(":");
	ncPrintHex(mac[3]);
	ncPrint(":");
	ncPrintHex(mac[4]);
	ncPrint(":");
	ncPrintHex(mac[5]);
}

void network_init(){

	turn_on();
	clear_buffer();
	init_receive_buffer();
	set_imr_isr();
	configure_receive_buffer();
	enable_receiver_transmiter();


	tx_pos=0;
	tx_buffers_free=4;
	rx_pos=0;
	get_mac_address();

	while(1){
		send_packet();
	}

}

int buffersize = 10;
char * buffer = (char *)0x5F0000;

void network_handler(){	

	ncPrint("Network Interruption");

	write_port_word(ioaddr + 0x3E, 0x1);

	char * video = (char *)0xB8000;
	int i;

	for(i = 0; i < buffersize; i++)  {
		video[i] = buffer[i];
		video[i + 1] = buffer[i];
	}


}

void send_packet (){	

	tx_buf[tx_pos]="packet";

	int len = 6;

	write_port_dword(ioaddr+ TSAD0 + (4*tx_pos), &tx_buf[tx_pos] );
	// Clears the OWN bit and sets the length,
	// sets the early transmit treshold to 8 bytes
	write_port_dword(ioaddr+ TSD0 + (4*tx_pos), len & 0xFFF);

	tx_pos = (tx_pos + 1) % 4;
	tx_buffers_free--;		
}
