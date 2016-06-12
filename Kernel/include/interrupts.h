#ifndef _INTERRUPS_H_
#define _INTERRUPS_H_

#include <types.h>

typedef void (*ISR)(void);		// (ISR = Interrupt Service Routine) puntero a funcion de manejo de interrupciones de hardware

#define ISR_SIZE 8

#define PIT_IRQ 0
#define KB_IRQ 1

void _cli(void);

void _sti(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

void _lidt(IDTR * idtr);

//Termina la ejecución de la cpu.
void haltcpu(void);

void init_interrupts(void);

#endif /* INTERRUPS_H_ */