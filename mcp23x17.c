#include "types.h"
#include "stmf103xxx.h"
#include "interrupts.h"
#include "i2c.h"
#include "mcp23x17.h"


void mcp23x17_write(i2c_t *i2c, uint8_t addr, uint8_t reg, uint8_t data) {

    _disable_irq();

    do{
      i2c_wait_for_ready(i2c);
      i2c_send_start(i2c);
    }while(i2c_send_addr_for_write(i2c, addr) != 0);

    i2c_write_data(i2c, reg);
    i2c_write_data(i2c, data);
    i2c_send_stop(i2c);

    _enable_irq();
}

uint8_t mcp23x17_read(i2c_t *i2c, uint8_t addr, uint8_t reg) {
    uint8_t ret = 0;

    _disable_irq();

    do{
      i2c_wait_for_ready(i2c);
      i2c_send_start(i2c);
    }while(i2c_send_addr_for_write(i2c, addr) != 0);

    i2c_write_data(i2c, reg);

    do{
      i2c_send_start(i2c);
    }while(i2c_send_addr_for_read(i2c, addr) != 0);

    ret = i2c_read_data(i2c, 0);
    i2c_send_stop(i2c);

    _enable_irq();

    return ret;
}

