#ifndef USART_H
#define USART_H

#define SR_TXE    (1 << 7)
#define SR_RXNE   (1 << 5)

#define SR_TC     (1 << 6)
#define CR1_UE    (1 << 13)
#define CR1_M     (1 << 12)
#define CR1_PCE   (1 << 10)
#define CR1_PS    (1 << 9)
#define CR1_PEIE  (1 << 8)
#define CR1_TXEIE  (1 << 7)
#define CR1_TCIE   (1 << 6)
#define CR1_TE    (1 << 3)
#define CR1_RE    (1 << 2)
#define CR1_SBK   1

#define CR2_STOP_1  0x00
#define CR2_STOP_2  0x10
#define CR2_CLKEN   (1 << 11)


typedef struct usart_t {
    volatile uint32_t sr;
    volatile uint32_t dr;
    volatile uint32_t brr;
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t cr3;
    volatile uint32_t gtpr;
} usart_t;

void usart_init(usart_t *usart);
void usart_putchar(usart_t *usart, uint8_t ch);
uint8_t usart_getchar(usart_t *usart);
void usart_puts(usart_t *usart, uint8_t *str);


#endif

