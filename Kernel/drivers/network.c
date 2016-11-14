#include <io.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <video.h>
#include <lib.h>

#define ioaddr 0xC000
#define RECEIVE_BUFFER 0x5F0000

/* RTL-8139 Registers */
#define TSD0 0x10
#define TSD1 0x14
#define TSD2 0x18
#define TSD3 0x1C
#define TSAD0 0x20
#define TSAD1 0x24
#define TSAD2 0x28
#define TSAD3 0x2C

	uint8_t mac[6];

	uint8_t *rx_buf;
	uint16_t rx_pos;

	uint8_t * tx_buf[4];
	uint8_t * tx_buf0 = (unsigned char *)0x600000;
	uint8_t tx_pos;
	uint8_t tx_buffers_free;

/* Source: http://wiki.osdev.org/RTL8139 */

void turn_on(){

	write_port(ioaddr + 0x52, 0x0);
}

void clear_buffer(){
	write_port( ioaddr + 0x37, 0x10);
 	while( (read_port(ioaddr + 0x37) & 0x10) != 0) { } 	
}

void init_receive_buffer(){	
	rx_buf = (unsigned char *) RECEIVE_BUFFER;
	write_port_dword(ioaddr + 0x30, (uint32_t) rx_buf); // send uint32_t memory location to RBSTART (0x30)}
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

	puts("Mac Address: ",DEFAULT);
	ncPrintHex(mac[0]);
	puts(":",DEFAULT);
	ncPrintHex(mac[1]);
	puts(":",DEFAULT);
	ncPrintHex(mac[2]);
	puts(":",DEFAULT);
	ncPrintHex(mac[3]);
	puts(":",DEFAULT);
	ncPrintHex(mac[4]);
	puts(":",DEFAULT);
	ncPrintHex(mac[5]);
	puts("\n",DEFAULT);
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

}

void network_handler(){	

	ncPrint("Network Interruption");

	/* Interrupt acknowledge */
	uint16_t status=read_port_word(ioaddr + 0x3E);
	write_port_word(ioaddr + 0x3E, 0x1);

}

void send_packet (uint64_t dest_mac, char * data, uint16_t size){

	uint16_t len;

	memcpy(tx_buf0,"\xff\xff\xff\xff\xff\xff",6);
	memcpy(tx_buf0+6,&mac[0],1);	
	memcpy(tx_buf0+7,&mac[1],1);
	memcpy(tx_buf0+8,&mac[2],1);
	memcpy(tx_buf0+9,&mac[3],1);
	memcpy(tx_buf0+10,&mac[4],1);
	memcpy(tx_buf0+11,&mac[5],1);
	memcpy(tx_buf0+12,"\x00\x00",2);
	memcpy(tx_buf0+14,data,size);	

	len = 14 + size;

	uint32_t status;	

	write_port_dword(ioaddr + TSAD0 + (4*tx_pos), tx_buf0 );
	
	// Clears the OWN bit and sets the length,
	// sets the early transmit treshold to 8 bytes

	status = 0;
	status |= len & 0x1FFF;	// 0-12: Length
	status |= 0 << 13;	// 13: OWN bit
	status |= (0 & 0x3F) << 16;	// 16-21: Early TX threshold (zero atm, TODO: check)


	write_port_dword(ioaddr + TSD0 + (4*tx_pos), status);

	//tx_pos = (tx_pos + 1) % 4;
	//tx_buffers_free--;		
}
