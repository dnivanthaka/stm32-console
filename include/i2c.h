#ifndef I2C_H
#define I2C_H

#define I2C_MODE_MASTER_SM    (0 << 15)
#define I2C_MODE_MASTER_FM    (1 << 15)
#define I2C_FM_MODE_DUTY_2    (0 << 14)
#define I2C_FM_MODE_DUTY_16_9 (1 << 14)

#define I2C_STAT_SB      (1 << 0)
#define I2C_STAT_BUSY    (1 << 1)
#define I2C_STAT_TxE     (1 << 7)
#define I2C_STAT_RxNE    (1 << 6)

#define I2C_ACK_ENABLE   (1 << 10)

typedef struct i2c_t {
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t oar1;
    volatile uint32_t oar2;
    volatile uint32_t dr;
    volatile uint32_t sr1;
    volatile uint32_t sr2;
    volatile uint32_t ccr;
    volatile uint32_t trise;
} i2c_t;

void i2c_init(i2c_t *i2c, rcc_t *rcc);
void i2c_wait_for_ready(i2c_t *i2c);
void i2c_send_start(i2c_t *i2c);
void i2c_send_stop(i2c_t *i2c);
void i2c_close(i2c_t *i2c);
uint8_t i2c_send_addr(i2c_t *i2c, uint8_t addr);
uint8_t i2c_send_addr_for_read(i2c_t *i2c, uint8_t addr);
uint8_t i2c_send_addr_for_write(i2c_t *i2c, uint8_t addr);
uint8_t i2c_write_data(i2c_t *i2c, uint8_t data);
uint8_t i2c_read_data(i2c_t *i2c, uint8_t ack);

#endif

