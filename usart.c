#include "types.h"
#include "timer.h"
#include "rcc.h"
#include "gpio.h"
#include "interrupts.h"
#include "usart.h"

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);


void usart_init(usart_t *usart){
 _disable_irq();
 /*Setting up TX*/
 //usart1->cr1 |= CR1_UE; //We only set the ones that we need, all others are set to 0
 //usart1->cr2 = CR2_STOP_1;
 usart->cr1 = 0x340c; //We only set the ones that we need, all others are set to 0
 usart->cr2 = 0;
 usart->cr3 = 0;
 usart->gtpr = 0;

 usart->brr = 9000000 / 9600;
 usart->cr1 |= CR1_TE | CR1_RE;
 //usart1->cr1 = 0x200C;
 _enable_irq();
}

void usart_putchar(usart_t *usart, uint8_t ch){
 _disable_irq();
 usart->dr = ch;
 while(! (usart->sr & SR_TXE));
 _enable_irq();
}

uint8_t usart_getchar(usart_t *usart){
 while(! (usart->sr & SR_RXNE));
  return usart->dr;
}

void usart_puts(usart_t *usart, uint8_t *str){
 _disable_irq();
 while(*str){
  usart_putchar(usart, *str);
  str++;
 }
 _enable_irq();
}

