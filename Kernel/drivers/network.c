#include <io.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <video.h>
#include <lib.h>
#include <network.h>
#include <syscalls.h>


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
#define CAPR 0x38 /* Current adress of packet read*/


#define ROK 1
#define RX_READ_POINTER_MASK (~3)


#define ISR 0x3E

#define FLAG_ISR_SERR	0x8000	// System error
#define FLAG_ISR_TIMEO	0x4000	// Timer timeout (See TIMERINT)
#define FLAG_ISR_LENCHG	0x2000	// Cable length changed
#define FLAG_ISR_FOVW	0x0040	// Rx FIFO Underflow
#define FLAG_ISR_PUN	0x0020	// Packet Underrung
#define FLAG_ISR_RXOVW	0x0010	// Rx Buffer Overflow
#define FLAG_ISR_TER	0x0008	// Tx Error
#define FLAG_ISR_TOK	0x0004	// Tx OK
#define FLAG_ISR_RER	0x0002	// Rx Error
#define FLAG_ISR_ROK	0x0001	// Rx OK

uint8_t mac[6];

uint8_t *rx_buf;
uint16_t rx_pos;

uint8_t * tx_buf[4];
uint8_t * tx_buf0 = (unsigned char *)0x700000;
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
	write_port_dword(ioaddr + 0x30, (uint64_t) rx_buf); // send uint32_t memory location to RBSTART (0x30)}
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

// return the array containing the 6 bytes of a mac address
uint8_t * get_mac_address() {

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

	return mac;
}

uint8_t * network_init(){

	turn_on();
	clear_buffer();
	init_receive_buffer();
	set_imr_isr();
	configure_receive_buffer();
	enable_receiver_transmiter();

	tx_pos=0;
	tx_buffers_free=4;
	rx_pos=0;

	return get_mac_address();
}

void print_mac_address(uint8_t mac[6]) {
	for(int i = 0; i < 6; i++) {
		ncPrintHex(mac[i]);	
		if (i == 5){
			ncPrint(" ");
		}
		ncPrint(":");							
	}
}

// TODO test buffer
#define BUFFER_SIZE 128

packet buffer[BUFFER_SIZE];
uint8_t curr_pos = 0;

void handle_data(uint8_t * data){

	memcpy(buffer[curr_pos].dest_mac, data, 6);  
	memcpy(buffer[curr_pos].src_mac, data + 6, 6);

	buffer[curr_pos].msg = (char *)malloc(data[13] + 1);
	memcpy(buffer[curr_pos].msg, data + 14, data[13] + 1);

	curr_pos++;

	if (curr_pos >= BUFFER_SIZE) {
		curr_pos = 0;
	}

	//print buffer

	// for(int i = 0; i < curr_pos; i++) {
	// 	print_mac_address(buffer[i].src_mac);
	// 	ncPrint("\n");
	// 	print_mac_address(buffer[i].dest_mac);
	// 	ncPrint("\n");
	// 	ncPrint(buffer[i].msg);
	// 	ncPrint("\n");
	// }
}


int last_read_pos = 0;

int get_next_packet(uint8_t * src_mac, uint8_t * dest_mac, char * msg) {
	if (curr_pos == 0 || curr_pos == last_read_pos) {
		return -1; 
	}

	// ncPrint("CURR POS: ");
	// ncPrintDec(curr_pos);

	// ncPrint("  LAST READPOS: ");
	// ncPrintDec(last_read_pos);	

	memcpy(src_mac, buffer[last_read_pos].src_mac, 6);
	memcpy(dest_mac, buffer[last_read_pos].dest_mac, 6);
	char * aux = buffer[last_read_pos].msg;

	while (*aux != 0) {
		*msg++ = *aux++;
	}
	*msg = 0;

	last_read_pos++;

	if (last_read_pos == BUFFER_SIZE) {
		last_read_pos = 0;
	}

	return 0;
}

void network_handler(){	

	//ncPrint("Network Interruption");

	/* Interrupt acknowledge */
	uint16_t status;//=read_port_word(ioaddr + 0x3E);
	//write_port_word(ioaddr + 0x3E, status);

	/*if(status & ROK)
	{
		while((read_port(ioaddr+0x37) & 0x1) == 0)
		{
			uint16_t rx_len=*(uint16_t*)(rx_buf + rx_pos + 2);
			//kprintf("packet length: %d\n", rx_len);

			// Handle the packet and send reply if needed
			uint8_t *data=rx_buf + rx_pos + 4;	

			puts(data,DEFAULT);

			// Update CAPR. This is some higher level magic found from the manual
			// +4 is the header, +3 is dword alignment
			rx_pos=(rx_pos + rx_len + 4 + 3) & RX_READ_POINTER_MASK;
			write_port_word(ioaddr + 0x38, rx_pos - 0x10);
			rx_pos%=0x2000;
		}
	}
*/	
	int j=0;	
	status = read_port_word(ioaddr + ISR);
	if( !status )	return ;	
		
	// Transmit OK, a transmit descriptor is now free
	if( status & FLAG_ISR_TOK )
	{
		for( j = 0; j < 4; j ++ )
		{
			if( read_port_dword(ioaddr + TSD0 + j*4) & 0x8000 ) {	// TSD TOK

				
			}
		}

		tx_buffers_free++;
		puts("Message Sent.",DEFAULT);
		puts("\n",DEFAULT);

		write_port_word(ioaddr + ISR, FLAG_ISR_TOK);
	}

	// Transmit error, ... oops
	if( status & FLAG_ISR_TER )
	{
		puts("Message could not be sent.",DEFAULT);
		write_port_word(ioaddr + ISR, FLAG_ISR_TER);
	}
	
	// Recieve OK, inform read
	if( status & FLAG_ISR_ROK )
	{	

		uint16_t rx_len=*(uint16_t*)(rx_buf + rx_pos + 2);

		//uint32_t curr_add = read_port_dword(ioaddr+CAPR);

		//rx_pos=curr_add+rx_pos+10;

		//rx_pos=rx_pos+10;

		uint8_t *data=rx_buf + rx_pos + 4;

		handle_data(data);		

		rx_pos=(rx_pos + rx_len + 4 + 3) & RX_READ_POINTER_MASK;
		write_port_word(ioaddr + CAPR, rx_pos - 0x10);
		rx_pos%=0x2000;

		write_port_word(ioaddr + ISR, FLAG_ISR_ROK);
	}	
		
	
	// Recieve error
	if( status & FLAG_ISR_RER )
	{
		puts("Message reception error.",DEFAULT);
		write_port_word(ioaddr + ISR, FLAG_ISR_RER);
	}

	// Packet Underrun/Link Change
	if( status & FLAG_ISR_PUN )
	{
		// Set when CAPR is written but Rx is empty, OR when the link status changes
		puts("RTL8139: ISR[PUN]",DEFAULT);
		write_port_word(ioaddr + ISR, FLAG_ISR_PUN);
	}

	// Rx Overflow
	if( status & FLAG_ISR_RXOVW )
	{
		puts("RTL8139 Rx Overflow",DEFAULT);
		write_port_word(ioaddr + ISR, FLAG_ISR_RXOVW);
	}

	// Rx FIFO Overflow
	if( status & FLAG_ISR_FOVW )
	{
		puts("RTL8139: Rx FIFO Overflow",DEFAULT);
		write_port_word(ioaddr + ISR, FLAG_ISR_FOVW);
	}

}

void send_packet (char * dest_mac, char * data, uint16_t size){

	uint16_t len;	

	memcpy(tx_buf0,dest_mac,6);
	memcpy(tx_buf0+6,&mac[0],1);	
	memcpy(tx_buf0+7,&mac[1],1);
	memcpy(tx_buf0+8,&mac[2],1);
	memcpy(tx_buf0+9,&mac[3],1);
	memcpy(tx_buf0+10,&mac[4],1);
	memcpy(tx_buf0+11,&mac[5],1);
	memcpy(tx_buf0+13,&size,1);		// longitud del mensaje (maximo 256)
	memcpy(tx_buf0+14,data,size);

	len = 14 + size;

	uint32_t status;	

	write_port_dword(ioaddr + TSAD0 + (4*tx_pos), (uint64_t)tx_buf0 );
	
	// Clears the OWN bit and sets the length,
	// sets the early transmit treshold to 8 bytes

	status = 0;
	status |= len & 0x1FFF;	// 0-12: Length
	status |= 0 << 13;	// 13: OWN bit
	status |= (0 & 0x3F) << 16;	// 16-21: Early TX threshold (zero atm, TODO: check)


	write_port_dword(ioaddr + TSD0 + (4*tx_pos), (uint64_t)status);

	tx_pos = (tx_pos + 1) % 4;
	tx_buffers_free--;		
}

