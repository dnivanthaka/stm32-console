#ifndef PCF8574_H
#define PCF8574_H

void pcf8574_write(i2c_t *i2c, uint8_t addr, uint8_t data);
uint8_t pcf8574_read(i2c_t *i2c, uint8_t addr);

#endif
