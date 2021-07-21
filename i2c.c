/**
 * 
 */
#include "types.h"
#include "stmf103xxx.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "interrupts.h"
#include "i2c.h"


void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

void i2c_init(i2c_t *i2c, rcc_t *rcc){
    _disable_irq();

    //enable i2c clocks, should have gpio clocks enabled	
    if(i2c == (struct i2c_t *)I2C1BASE){
   	rcc->apb1enr |= (1 << 21); 
    }else if(i2c == (struct i2c_t *)I2C2BASE){
   	rcc->apb1enr |= (1 << 22); 
    }

    i2c->ccr |= 0x00B4;
    i2c->trise = 37;

    //Set clock speed APB
    i2c->cr2 = 0x0020;

    //i2c->oar1 = 0 << 15;
    //i2c->oar1 |= 1 << 14;
    //i2c->oar2 |= 0b10101100;

    i2c->cr1 = 1;
    _enable_irq();
}

void i2c_wait_for_ready(i2c_t *i2c){
    while((i2c->sr2 & (I2C_STAT_BUSY)) != 0);
}

void i2c_send_start(i2c_t *i2c){
    i2c->cr1 |= (1<<8); /* start */
    while((i2c->sr1&(I2C_STAT_SB)) == 0); /* wait for SB */
    int stat = i2c->sr2;
}

void i2c_send_stop(i2c_t *i2c){
    i2c->cr1 |= (1<<9); /* stop */
    while((i2c->sr2 & (I2C_STAT_SB)) != 0); /* wait for becoming slave */
}

void i2c_close(i2c_t *i2c){
    i2c->cr1 &= ~(1);
}

uint8_t i2c_send_addr(i2c_t *i2c, uint8_t addr){
    i2c->dr = addr;
    uint32_t stat;
    do{
	stat = i2c->sr1;
        if((stat & (1 << 9)) != 0) //ARLO
            return 1;

        if((stat & (1 << 1)) != 0){
	    stat = i2c->sr2; //read sr2 to clear addr flag
	    return 0;
	}
    }while(1);
}

uint8_t i2c_send_addr_for_read(i2c_t *i2c, uint8_t addr){
    return i2c_send_addr(i2c, (addr << 1) + 1);
}

uint8_t i2c_send_addr_for_write(i2c_t *i2c, uint8_t addr){
    return i2c_send_addr(i2c, (addr << 1));
}

uint8_t i2c_write_data(i2c_t *i2c, uint8_t data){
    i2c->dr = data;

    uint32_t stat;
    do{
    	stat = i2c->sr1;
	if((stat & (I2C_STAT_TxE)) != 0)
	    return 0;
    }while(1);
}

uint8_t i2c_read_data(i2c_t *i2c, uint8_t ack){
    if(ack){
        i2c->cr1 |= (I2C_ACK_ENABLE);
    }else{
        i2c->cr1 &= ~(I2C_ACK_ENABLE);    
    }

    while((i2c->sr1 & (I2C_STAT_RxNE)) == 0); //wait for RxNE
    return i2c->dr;
}

