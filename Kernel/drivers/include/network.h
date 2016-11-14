#ifndef NETWORK_H
#define NETWORK_H

void network_init(void);
void network_handler(void);
void send_packet (uint64_t dest_mac, char * data, uint16_t size);

#endif