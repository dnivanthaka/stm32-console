#ifndef SYSTEM_H
#define SYSTEM_H

#include "types.h"
#include "stm32f103xxx.h"
#include "timer.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "i2c.h"
#include "interrupts.h"
#include "keypad.h"
#include "sound.h"
#include "console_system.h"
#include "adc.h"
#include "bitmap.h"
#include "font.h"
#include "eventq.h"
#include "dma.h"

extern void setup();
extern void loop();

#endif