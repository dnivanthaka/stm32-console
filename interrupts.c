#include "types.h"
#include "stmf103xxx.h"
#include "interrupts.h"


void nvic_enable_irq(uint32_t irq){
    struct nvic *ptr = (struct nvic *)NVICBASE;

    if(irq > MAX_IRQ){
       return; 
    }

    ptr->iser[irq / 32] |= 1 << (irq % 32);
}

void nvic_disable_irq(uint32_t irq){
    struct nvic *ptr = (struct nvic *)NVICBASE;

    if(irq > MAX_IRQ){
       return; 
    }

    ptr->iser[irq / 32] &= ~(1 << (irq % 32));
}

void _disable_irq() {
    asm volatile ("cpsid i");
}

void _enable_irq() {
    asm volatile ("cpsie i");
}

