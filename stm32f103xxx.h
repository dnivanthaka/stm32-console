#ifndef STM32F103_H
#define STM32F103_H

#include "types.h"
#include "interrupts.h"
#include "timer.h"
#include "rcc.h"
#include "gpio.h"
#include "spi.h"
#include "i2c.h"
#include "usart.h"


#define SYSTEM_CORE_CLOCK 72000000

#define GPIOABASE   0x40010800
#define GPIOBBASE   0x40010C00
#define GPIOCBASE   0x40011000

#define RCCBASE     0x40021000
#define PLL_LOCK    0x02000000
#define SYSTICKBASE 0xE000E010

#define EXTIBASE    0x40010400 
#define SCBBASE     0xE000E008 
#define NVICBASE    0xE000E100
#define AFIOBASE    0x40010000

#define SPI1BASE    0x40013000
#define SPI2BASE    0x40003800
#define SPI3BASE    0x40003C00

#define I2C1BASE    0x40005400
#define I2C2BASE    0x40005800

#define USART1BASE  0x40013800
#define USART2BASE  0x40004400

#define ADC1BASE    0x40012400
#define ADC2BASE    0x40012800

#define TIM1BASE    0x40012C00
#define TIM2BASE    0x40000000
#define TIM3BASE    0x40000400
#define TIM4BASE    0x40000800
#define TIM5BASE    0x40000C00

#define DMA1BASE    0x40020000
#define DMA2BASE    0x40020400

#define RCC     ((rcc_t *) RCCBASE)
#define SYSTICK ((systick_t *) SYSTICKBASE)

#define EXTI    ((exti_t *) EXTIBASE)
#define AFIO    ((afio_t *) AFIOBASE)

#define GPIOA ((gpio_t *) GPIOABASE)
#define GPIOB ((gpio_t *) GPIOBBASE)
#define GPIOC ((gpio_t *) GPIOCBASE)

#define SPI1 ((spi_t *) SPI1BASE)
#define SPI2 ((spi_t *) SPI2BASE)

#define I2C1 ((i2c_t *) I2C1BASE)
#define I2C2 ((i2c_t *) I2C2BASE)

#define USART1 ((usart_t *) USART1BASE)
#define USART2 ((usart_t *) USART2BASE)

#define TIM1 ((timer_t *) TIM1BASE)
#define TIM2 ((timer_t *) TIM2BASE)
#define TIM3 ((timer_t *) TIM3BASE)

#endif
