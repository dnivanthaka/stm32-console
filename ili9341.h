#ifndef ILI9341_H
#define ILI9341_H

#define ILI9341_WIDTH     240 
#define ILI9341_HEIGHT    320

#define ILI9341_NOP       0x00
#define ILI9341_SWRST     0x01
#define ILI9341_READDID   0x04
#define ILI9341_READDS    0x09
#define ILI9341_READPM    0x0A
#define ILI9341_MADCTL    0x0B
#define ILI9341_READPF    0x0C
#define ILI9341_READIF    0x0D
#define ILI9341_READSM    0x0E
#define ILI9341_READDIAG  0x0F
#define ILI9341_MSLEEP    0x10
#define ILI9341_SLEEPOUT  0x11
#define ILI9341_MPARTIAL  0x12
#define ILI9341_NORM_DISPON   0x13
#define ILI9341_INVON     0x20
#define ILI9341_INVOFF    0x21

#define ILI9341_GAMMASET  0x26
#define ILI9341_DISPOFF   0x28
#define ILI9341_DISPON    0x29
#define ILI9341_COLSET    0x2A
#define ILI9341_PAGESET   0x2B
#define ILI9341_MEMWR     0x2C
#define ILI9341_COLORSET  0x2D
#define ILI9341_MEMRD     0x2E
#define ILI9341_PARTAREA  0x30
#define ILI9341_VERTSCR_DEF   0x33
#define ILI9341_TEOFF     0x34
#define ILI9341_TEON      0x35

#define ILI9341_MEMACTRL  0x36
#define ILI9341_VERTSTART  0x37
#define ILI9341_IDLEOFF   0x38
#define ILI9341_IDLEON    0x39

#define ILI9341_PIXELFMT       0x3A
#define ILI9341_WRITEMEM_CONT  0x3C
#define ILI9341_READMEM_CONT   0x3E
#define ILI9341_SETTEARSCAN    0x44
#define ILI9341_GETSCAN_LINE   0x45
#define ILI9341_BRIGHTNESSWR   0x51
#define ILI9341_BRIGHTNESSRD   0x52
#define ILI9341_CTRLWR         0x53
#define ILI9341_CTRLRD         0x54

#define ILI9341_PWRCTL1        0xC0
#define ILI9341_PWRCTL2        0xC1
#define ILI9341_VCOMCTL1       0xC5
#define ILI9341_VCOMCTL2       0xC7

#define ILI9341_FRMNORM        0xB1
#define ILI9341_FRMIDLE        0xB2
#define ILI9341_FRMPART        0xB3

#define ILI9341_DFUNCCTL       0xB6

#define ILI9341_GMMPOSI        0xE0
#define ILI9341_GMMNEG         0xE1

#define ILI9341_MADCTL_MY      0x80
#define ILI9341_MADCTL_MX      0x40
#define ILI9341_MADCTL_MV      0x20
#define ILI9341_MADCTL_RGB     0x00
#define ILI9341_MADCTL_BGR     0x08


void ili9341_init(spi_t *spi, gpio_t *gpio);
void ili9341_swreset(spi_t *spi, gpio_t *gpio);
void ili9341_hwreset(gpio_t *gpio);
void ili9341_command(uint8_t cmd, spi_t *spi, gpio_t *gpio);
void ili9341_data(uint8_t dta, spi_t *spi, gpio_t *gpio);
void ili9341_set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, spi_t *spi, gpio_t *gpio);
void ili9341_streampixel(uint16_t color, spi_t *spi);
void ili9341_streampixel_bytes(uint16_t color, uint16_t count, spi_t *spi);
void ili9341_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color, spi_t *spi, gpio_t *gpio);
void ili9341_fill_screen(uint16_t color, spi_t *spi, gpio_t *gpio);
void ili9341_draw_pixel(int16_t x, int16_t y, uint16_t color, spi_t *spi, gpio_t *gpio);
void ili9341_set_rotation(uint8_t m, spi_t *spi, gpio_t *gpio);
uint8_t ili9341_get_rotation();
uint16_t ili9341_get_width();
uint16_t ili9341_get_height();
void ili9341_tearing_off();
void ili9341_tearing_on();

#endif
