#ifndef KEYPAD_H
#define KEYPAD_H

#define IODIRA     0x00
#define IODIRB_0   0x01
#define IODIRB_1   0x10
#define IPOLA_0    0x02
#define IPOLA_1    0x01
#define GPINTENA_0 0x04
#define GPINTENA_1 0x02
#define GPINTENB_0 0x05
#define GPINTENB_1 0x12
#define DEFVALA_0  0x06
#define DEFVALA_1  0x03
#define DEFVALB_0  0x07
#define DEFVALB_1  0x13
#define INTCONA_0  0x08
#define INTCONA_1  0x04
#define INTCONB_0  0x09
#define INTCONB_1  0x14
#define IOCON_0    0x0A
#define IOCON_1    0x05
#define GPPUA_0    0x0C
#define GPPUA_1    0x06
#define GPPUB_0    0x0D
#define GPPUB_1    0x16
#define INTFA_0    0x0E
#define INTFA_1    0x07
#define INTFB_0    0x0F
#define INTFB_1    0x17
#define INTCAPA_0  0x10
#define INTCAPA_1  0x08
#define INTCAPB_0  0x11
#define INTCAPB_1  0x18
#define GPIOA_0    0x12
#define GPIOA_1    0x09
#define GPIOB_0    0x13
#define GPIOB_1    0x19
#define OLATA_0    0x14
#define OLATA_1    0x0A
#define OLATB_0    0x15
#define OLATB_1    0x1A

//IOCON register bits
#define IOBANK   (1 << 7)
#define MIRROR (1 << 6)
#define SEQP   (1 << 5)
#define DISSLW (1 << 4)
#define HAEN   (1 << 3)
#define ODR    (1 << 2)
#define INTPOL (1 << 1)

//keypad keys, check macro
#define KEYPAD_UP(val)    (val & (1 << 0))
#define KEYPAD_DOWN(val)  (val & (1 << 1))
#define KEYPAD_LEFT(val)  (val & (1 << 2))
#define KEYPAD_RIGHT(val) (val & (1 << 3))

void keypad_init();
uint16_t keypad_read();

#endif
