#ifndef _INTERRUPS_H_
#define _INTERRUPS_H_

#include "types.h"

void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

void _cli(void);

void _sti(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

void _lidt(IDTR * idtr);

//Termina la ejecución de la cpu.
void haltcpu(void);

#endif /* INTERRUPS_H_ */