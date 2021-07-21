#include "types.h"
#include "stmf103xxx.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "i2c.h"
#include "interrupts.h"
#include "pcf8574.h"
#include "keypad.h"

#define KEYPAD_ADDR 0x20

#define KEYPAD_I2C I2C1
#define KEYPAD_RCC RCC

void keypad_init() {
    i2c_init(KEYPAD_I2C, KEYPAD_RCC);

    pcf8574_write(KEYPAD_I2C, KEYPAD_ADDR, 0xff);

    //clearing out pending interrupts
    keypad_read();
}

uint8_t keypad_read(){
    return pcf8574_read(KEYPAD_I2C, KEYPAD_ADDR);
}
