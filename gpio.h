#ifndef GPIO_H
#define GPIO_H

#define GPIO_CNF_IN_ANALOG    0x0
#define GPIO_CNF_IN_FLOAT     0x4
#define GPIO_CNF_IN_PULL      0x8
#define GPIO_CNF_RESERVED     0xC

#define GPIO_CNF_OUT_PUSH     0x0
#define GPIO_CNF_OUT_OPEN     0x4
#define GPIO_CNF_OUT_ALT_PUSH 0x8
#define GPIO_CNF_OUT_ALT_OPEN 0xC

#define GPIO_MODE_INPUT       0x0
#define GPIO_MODE_OUT_10_MHZ  0x1
#define GPIO_MODE_OUT_2_MHZ   0x2
#define GPIO_MODE_OUT_50_MHZ  0x3

#define GPIO_IOPAEN           1 << 2
#define GPIO_IOPBEN           1 << 3
#define GPIO_IOPCEN           1 << 4


typedef struct gpio_t {
    volatile uint32_t cr[2];
    volatile uint32_t idr;
    volatile uint32_t odr;
    volatile uint32_t bsrr;
    volatile uint32_t brr;
    volatile uint32_t lckr;
} gpio_t;

void gpio_init(gpio_t *gpio, rcc_t *rcc, uint8_t pin, uint8_t mode);
void gpio_out(gpio_t *gpio, uint8_t pin, uint8_t value);
uint8_t gpio_in(gpio_t *gpio, uint8_t pin);

#endif
