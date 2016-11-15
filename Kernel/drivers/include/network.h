#ifndef NETWORK_H
#define NETWORK_H

uint8_t * network_init(void);
void network_handler(void);
void send_packet (char * dest_mac, char * data, uint16_t size);

#endif