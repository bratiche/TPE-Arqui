#ifndef NETWORK_H
#define NETWORK_H

uint8_t * network_init(void);
void network_handler(void);
void send_packet(char * dest_mac, char * data, uint16_t size);

typedef struct {
	uint8_t dest_mac[6];
	uint8_t src_mac[6];
	char * msg;
} packet;


int get_next_packet(uint8_t * src_mac, uint8_t * dest_mac, char * buffer);

#endif