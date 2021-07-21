#include "types.h"
#include "stmf103xxx.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "st7735.h"
#include "i2c.h"
#include "interrupts.h"
#include "mcp23x17.h"
#include "keypad.h"

#define KEYPAD_ADDR 0x20

#define KEYPAD_I2C I2C1
#define KEYPAD_RCC RCC

void keypad_init() {
    i2c_init(KEYPAD_I2C, KEYPAD_RCC);

    //reset iocon to default values
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, IOCON_0, 0x00);

    //clearing out pending interrupts
    keypad_read();

    //set ports as inputs
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, IODIRA,   0xff);
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, IODIRB_0, 0xff);

    //enable pullups
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, GPPUA_0, 0xff);
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, GPPUB_0, 0xff);

    //enable interrupts, defval
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, DEFVALA_0, 0xff);
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, DEFVALB_0, 0xff);
    // IoC setup
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, INTFA_0, 0);
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, INTFB_0, 0);
    //trigger IoC on previous value
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, INTCONA_0, 0);
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, INTCONB_0, 0);

    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, GPINTENA_0, 0xff);
    mcp23x17_write(KEYPAD_I2C, KEYPAD_ADDR, GPINTENB_0, 0xff);
}

uint16_t keypad_read(){
    uint8_t gpioa = mcp23x17_read(KEYPAD_I2C, KEYPAD_ADDR, 0x12);
    uint8_t gpiob = mcp23x17_read(KEYPAD_I2C, KEYPAD_ADDR, 0x13);

    return ((gpiob << 8) | gpioa);
}
