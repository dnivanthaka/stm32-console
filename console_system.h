#ifndef CONSOLE_SYSTEM_H
#define CONSOLE_SYSTEM_H

#include "types.h"
#include "stm32f103xxx.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "i2c.h"
#include "interrupts.h"
#include "keypad.h"
#include "timer.h"
#include "st7565r.h"
#include "sound.h"

//screen routines

#define SCREEN_WIDTH 128
#define SCREEN_WIDTH_MID (SCREEN_WIDTH / 2)
#define SCREEN_HEIGHT 64
#define SCREEN_HEIGHT_MID (SCREEN_HEIGHT / 2)

inline void screen_init() {
    //ili9341_init(SPI1, GPIOA);
    st7565r_init();
}

inline void screen_reset() {
    //ili9341_hwreset(GPIOA);
}

inline void screen_fill(uint8_t color) {
    //ili9341_fill_screen(color, SPI1, GPIOA);
    st7565r_fill_screen(color);
}

inline void screen_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    //ili9341_set_addr_window(x0, y0, x1, y1, SPI1, GPIOA);
}

inline void screen_streampixel(uint16_t color) {
    //ili9341_streampixel(color, SPI1);
}

inline void screen_putpixel(int16_t x, int16_t y, uint16_t color) {
    //ili9341_draw_pixel(x,  y, color, SPI1, GPIOA);
}

inline void screen_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
    //ili9341_fill_rect(x, y, w, h, color, SPI1, GPIOA);
}

inline void system_init() {
    rcc_init(RCC);
    systick_init(SYSTICK);

    /*Enable PORT A, PA9 = TX, PA10 = RX*/
    gpio_init(GPIOA, RCC, 9,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_PUSH);
    gpio_init(GPIOA, RCC, 10,  GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT);
    gpio_init(GPIOA, RCC, 8, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //RS
    gpio_init(GPIOA, RCC, 2, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //RSE
    gpio_init(GPIOB, RCC, 9, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //CS
    gpio_init(GPIOB, RCC, 8, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //LED 1
    gpio_init(GPIOA, RCC, 0, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //LED 2
    gpio_init(GPIOA, RCC, 1, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);

    //sound
    gpio_init(GPIOA, RCC, 3, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(GPIOA, RCC, 0, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);

    gpio_init(GPIOB, RCC, 4, GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT );

    gpio_init(GPIOB, RCC, 5, GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT );

    gpio_init(GPIOB, RCC, 3, GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT );

    gpio_init(GPIOC, RCC, 13, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);

    gpio_init(GPIOA, RCC, 5,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(gpio_a, rcc, 5,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //gpio_init(GPIOA, RCC, 6,  GPIO_MODE_INPUT | GPIO_CNF_IN_PULL);
    gpio_init(GPIOA, RCC, 6,  GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT);
    gpio_init(GPIOA, RCC, 7,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);

    gpio_init(GPIOB, RCC, 6,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_OPEN);              //I2C pins
    gpio_init(GPIOB, RCC, 7,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_OPEN);

    // Disable Reset
    RCC->apb2rstr &= ~(1 << 14);
    RCC->apb2rstr &= ~(1 << 12);

    //Enable USART1 clock
    RCC->apb2enr |= (1 << 14) | 1;
    //Enable SPI1 clock
    RCC->apb2enr |= (1 << 12) | 1;
    //rcc->apb2enr |= 0x4000;

    //usart_init(USART1);

    spi_init(SPI1, SPI_MASTER, SPI_MODE_UNI2 | SPI_OUTPUT_ENABLE, 
            SPI_DFF_8BIT, SPI_CLK_DIV2, SPI_CPOL_0, SPI_CPHA_1, SPI_SLAVE_MGMT_DISABLE);


    //GPIOB->odr &= ~(1 << 4); //without pullup for pb4
    //gpio_b->odr &= ~(1 << 3); //without pullup for pb3
    //GPIOB->odr &= ~(1 << 5); //without pullup for pb5
    GPIOB->odr |= (1 << 4); //pullup for pb4

    //Setting up interrupts
    //AFIO->exticr[0] = 1 << 12;   //PB3 as input
    AFIO->exticr[1] = 1 << 0;   //PB4 as input
    AFIO->exticr[1] |= 1 << 4;   //PB4 as input
    
    //EXTI->ftsr = (1 << 3); //falling edge
    //EXTI->ftsr = (1 << 5); //falling edge
    EXTI->ftsr |= (1 << 4); //falling edge

    //EXTI->imr = (1 << 3);  // enable interrupt exti3
    //EXTI->imr = (1 << 5);  // enable interrupt exti3
    EXTI->imr |= (1 << 4);  // enable interrupt exti4

    //nvic_enable_irq(EXTI3_IRQ);
    //nvic_enable_irq(EXTI9_5_IRQ);
    nvic_enable_irq(EXTI4_IRQ);

    keypad_init(I2C1, RCC);
    gpio_out(GPIOC, 13, 0);

    sound_init();
    //sound_beep(500);

    gpio_out(GPIOA, 0, 1);
    delay_ms(250);
    gpio_out(GPIOA, 0, 0);
    gpio_out(GPIOA, 1, 1);
    delay_ms(250);
    gpio_out(GPIOA, 1, 0);
    //screen backlight
    gpio_out(GPIOA, 8, 1);

    screen_init();

    //gpio_out(GPIOA, 0, 0);
    //gpio_out(GPIOA, 1, 0);

    //st7735_tearing_off(GPIOA, SPI1);

    //we start interrupts
    systick_interrupt_start(SYSTICK);
}

#endif

