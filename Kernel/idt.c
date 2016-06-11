#include <types.h>
#include <interrupts.h>
#include <stdint.h>

static void set_idt_entry (int index, uint8_t selector, uint64_t offset, uint8_t access);

DESCR_INT * idt;

void set_idt () {

    idt=0;

    set_idt_entry (0x20, 0x08, (uint64_t)&_irq00Handler , 0x8E);
    set_idt_entry (0x21, 0x08, (uint64_t)&_irq01Handler , 0x8E);    

    picMasterMask(0xFC); 
    picSlaveMask(0xFF);

    _sti();
}

void set_idt_entry (int index, uint8_t selector, uint64_t offset, uint8_t access) {
    idt[index].selector = selector;
    idt[index].offset_l = offset & 0xFFFF;
    idt[index].offset_m = (offset >> 16) & 0xFFFF;
    idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
    idt[index].access = access;
    idt[index].zero1 = 0;
    idt[index].zero2 = 0;
}

