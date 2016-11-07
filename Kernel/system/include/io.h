#ifndef IO_H_
#define IO_H_

#include <stdint.h>

uint8_t read_port(uint16_t port);

void write_port(uint16_t port, uint8_t data);

uint16_t read_port_word(uint16_t port);

void write_port_word(uint16_t port, uint16_t data);

uint32_t read_port_dword(uint16_t port);

void write_port_dword(uint16_t port, uint32_t data);

#endif

