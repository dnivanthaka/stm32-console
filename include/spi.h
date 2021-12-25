#ifndef SPI_H
#define SPI_H

#define SPI_CLK_DIV2   0b000
#define SPI_CLK_DIV4   0b001
#define SPI_CLK_DIV8   0b010
#define SPI_CLK_DIV16  0b011
#define SPI_CLK_DIV32  0b100
#define SPI_CLK_DIV64  0b101
#define SPI_CLK_DIV128 0b110
#define SPI_CLK_DIV256 0b111

#define SPI_CPOL_0     0
#define SPI_CPOL_1     1

#define SPI_CPHA_1     0
#define SPI_CPHA_2     1

#define SPI_DFF_8BIT   0
#define SPI_DFF_16BIT  1

#define SPI_ENDIAN_MSB 0
#define SPI_ENDIAN_LSB 1

#define SPI_SLAVE_MGMT_ENABLE  (0 << 1)
#define SPI_SLAVE_MGMT_DISABLE (1 << 1)
#define SPI_INTERNAL_SS        1

#define SPI_SLAVE      0
#define SPI_MASTER     1

#define SPI_SR_TXE     0x02
#define SPI_SR_RXNE    1

#define SPI_MODE_UNI2  (0 << 1)
#define SPI_MODE_BI1   (1 << 1)

#define SPI_OUTPUT_DISABLE 0
#define SPI_OUTPUT_ENABLE  1

typedef struct spi_t {
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t sr;
    volatile uint32_t dr;
    volatile uint32_t cpcpr;
    volatile uint32_t rxcrcr;
    volatile uint32_t txcrcr;
    volatile uint32_t i2scfgr;
    volatile uint32_t i2spr;
} spi_t;


void spi_init(spi_t *spi, uint8_t mode, uint8_t dir, uint8_t frame_format, 
        uint8_t baud, uint8_t idle_polarity, uint8_t clk_phase, uint8_t slave_mgmt);
void spi_write(spi_t *spi, uint16_t data);
uint8_t spi_read(spi_t *spi);
uint8_t spi_transfer(spi_t *spi, uint16_t data);
void spi_close(spi_t *spi);

#endif

