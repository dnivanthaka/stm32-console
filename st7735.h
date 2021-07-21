#ifndef ST7735_H
#define ST7735_H

#define ST7735_ROTATION 1

#if ST7735_ROTATION 
#define ST7735_TFTWIDTH  160 
#define ST7735_TFTHEIGHT 128
#else
#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160
#endif

#define INITR_GREENTAB 0x0
#define INITR_REDTAB   0x1

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_TEOFF   0x34
#define ST7735_TEON    0x35

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
#define Color565(r,g,b) (uint16_t)((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3)

void st7735_hwreset(gpio_t *gpio);
void st7735_command(uint8_t cmd, gpio_t *gpio, spi_t *spi);
void st7735_data(uint8_t dta, gpio_t *gpio, spi_t *spi);
void st7735_init(gpio_t *gpio, spi_t *spi);
void st7735_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, gpio_t *gpio, spi_t *spi);
void st7735_streampixel(uint16_t color, spi_t *spi);
void st7735_draw_pixel(int16_t x, int16_t y, uint16_t color, gpio_t *gpio, spi_t *spi);
void st7735_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color, gpio_t *gpio, spi_t *spi);
void st7735_fill_screen(uint16_t color, gpio_t *gpio, spi_t *spi);
void st7735_tearing_on(gpio_t *gpio, spi_t *spi);
void st7735_tearing_off(gpio_t *gpio, spi_t *spi);
void st7735_test(gpio_t *gpio, spi_t *spi);
void st7735_set_rotation(uint8_t rotation, gpio_t *gpio, spi_t *spi);


#endif // ST7735_H_INCLUDED
