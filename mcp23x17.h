#ifndef MCP23X17_H
#define MCP23X17_H

void mcp23x17_write(i2c_t *i2c, uint8_t addr, uint8_t reg, uint8_t data);
uint8_t mcp23x17_read(i2c_t *i2c, uint8_t addr, uint8_t reg);

#endif
