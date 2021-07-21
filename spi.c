#include "types.h"
#include "stmf103xxx.h"
#include "interrupts.h"
#include "spi.h"

void spi_init(spi_t *spi, uint8_t mode, uint8_t dir, uint8_t frame_format, 
        uint8_t baud, uint8_t idle_polarity, uint8_t clk_phase, uint8_t slave_mgmt){

    while((spi->sr & (1 << 7))); //BSY flag

    _disable_irq();

    spi->cr1 &= ~(1 << 6);
    //spi->cr1 = (baud << 3) | (idle_polarity << 1) | clk_phase | (slave_mgmt << 8) | (mode << 2) | (frame_format << 7) | (dir << 14);
    //spi->cr1 = (0b010 << 3) | (0 << 1) | 1 | (0b11 << 8) | (1 << 2) | (1 << 7) | (0b00 << 14);
    //spi->cr1 &= 0xff00;
    spi->cr1 = 0b0000001100000100;
    spi->cr2 = (1 << 2);
    //spi->cr1 = 0x35c;
    spi->cpcpr = 10;
    //spi->i2scfgr &= ~(1 << 11);
    //spi->cr1 = 0x35;
    //Enable the peripheral
    spi->cr1 |= (1 << 6);
    _enable_irq();
}

void spi_write(spi_t *spi, uint16_t data){
    _disable_irq();
    //Clear the overrun flag
    spi->sr &= ~(1 << 6);
    spi->dr = data;
    while(! (spi->sr & SPI_SR_TXE));
    while((spi->sr & (1 << 7))); //BSY flag
    _enable_irq();
}

uint8_t spi_read(spi_t *spi){
    _disable_irq();
    while((spi->sr & (1 << 7))); //BSY flag
    while(! (spi->sr & SPI_SR_RXNE));
    _enable_irq();
    return spi->dr;
}

uint8_t spi_transfer(spi_t *spi, uint16_t data){
    _disable_irq();
    spi->dr = data;
    while(! (spi->sr & SPI_SR_RXNE));
    while((spi->sr & (1 << 7))); //BSY flag
    _enable_irq();
    return spi->dr;
}

void spi_close(spi_t *spi){
    while((spi->sr & (1 << 7))); //BSY flag
    spi->cr1 &= ~(1 << 6);
}

