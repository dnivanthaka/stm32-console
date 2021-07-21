#include "types.h"
#include "stmf103xxx.h"
#include "spi.h"
#include "ili9341.h"

// Pin configuration
// RES -> PA2
// CE  -> PA3
// D/C -> PA4

#define ILI9341_DC 4
#define ILI9341_CE 3
#define ILI9341_RES 2

static uint8_t rotation = 0;
static uint16_t g_ili9341_width = ILI9341_WIDTH;
static uint16_t g_ili9341_height = ILI9341_HEIGHT;


void ili9341_init(spi_t *spi, gpio_t *gpio) {
    gpio_out(gpio, ILI9341_CE, 1);
    gpio_out(gpio, ILI9341_DC, 1);
    gpio_out(gpio, ILI9341_RES, 1);

    delay_ms(100);

    ili9341_hwreset(gpio);
    ili9341_swreset(spi, gpio);

    delay_ms(100);

    //Select display
    gpio_out(gpio, ILI9341_CE, 0);

    ili9341_command(0xEF, spi, gpio); 
    ili9341_data(0x03, spi, gpio);
    ili9341_data(0x80, spi, gpio);
    ili9341_data(0x02, spi, gpio);

    ili9341_command(0xCF, spi, gpio); 
    ili9341_data(0x00, spi, gpio);
    ili9341_data(0xc1, spi, gpio);
    ili9341_data(0x30, spi, gpio);

    ili9341_command(0xED, spi, gpio); 
    ili9341_data(0x64, spi, gpio);
    ili9341_data(0x03, spi, gpio);
    ili9341_data(0x12, spi, gpio);
    ili9341_data(0x81, spi, gpio);

    ili9341_command(0xE8, spi, gpio); 
    ili9341_data(0x85, spi, gpio);
    ili9341_data(0x00, spi, gpio);
    ili9341_data(0x78, spi, gpio);

    ili9341_command(0xCB, spi, gpio); 
    ili9341_data(0x39, spi, gpio);
    ili9341_data(0x2C, spi, gpio);
    ili9341_data(0x00, spi, gpio);
    ili9341_data(0x34, spi, gpio);
    ili9341_data(0x02, spi, gpio);

    ili9341_command(0xF7, spi, gpio); 
    ili9341_data(0x20, spi, gpio);

    ili9341_command(0xEA, spi, gpio); 
    ili9341_data(0x00, spi, gpio);
    ili9341_data(0x00, spi, gpio);

    ili9341_command(ILI9341_PWRCTL1, spi, gpio); 
    ili9341_data(0x23, spi, gpio);

    ili9341_command(ILI9341_PWRCTL2, spi, gpio); 
    ili9341_data(0x10, spi, gpio);

    ili9341_command(ILI9341_VCOMCTL1, spi, gpio); 
    ili9341_data(0x3E, spi, gpio);
    ili9341_data(0x28, spi, gpio);

    ili9341_command(ILI9341_VCOMCTL2, spi, gpio); 
    ili9341_data(0x86, spi, gpio);

    ili9341_command(ILI9341_MADCTL, spi, gpio); 
    ili9341_data(0x48, spi, gpio);

    ili9341_command(ILI9341_VERTSTART, spi, gpio); 
    ili9341_data(0x00, spi, gpio);

    ili9341_command(ILI9341_PIXELFMT, spi, gpio); 
    ili9341_data(0x55, spi, gpio);

    ili9341_command(ILI9341_FRMNORM, spi, gpio); 
    ili9341_data(0x00, spi, gpio);
    ili9341_data(0x18, spi, gpio);

    ili9341_command(ILI9341_DFUNCCTL, spi, gpio); 
    ili9341_data(0x08, spi, gpio);
    ili9341_data(0x82, spi, gpio);
    ili9341_data(0x27, spi, gpio);

    ili9341_command(0xf2, spi, gpio); 
    ili9341_data(0x00, spi, gpio);

    ili9341_command(ILI9341_GAMMASET, spi, gpio); 
    ili9341_data(0x01, spi, gpio);

    ili9341_command(ILI9341_GMMPOSI, spi, gpio); 
    ili9341_data(0x0f, spi, gpio);
    ili9341_data(0x31, spi, gpio);
    ili9341_data(0x2b, spi, gpio);
    ili9341_data(0x0c, spi, gpio);
    ili9341_data(0x0e, spi, gpio);
    ili9341_data(0x08, spi, gpio);
    ili9341_data(0x4e, spi, gpio);
    ili9341_data(0xf1, spi, gpio);
    ili9341_data(0x37, spi, gpio);
    ili9341_data(0x07, spi, gpio);
    ili9341_data(0x10, spi, gpio);
    ili9341_data(0x03, spi, gpio);
    ili9341_data(0x0e, spi, gpio);
    ili9341_data(0x09, spi, gpio);
    ili9341_data(0x00, spi, gpio);

    ili9341_command(ILI9341_GMMNEG, spi, gpio); 
    ili9341_data(0x00, spi, gpio);
    ili9341_data(0x0e, spi, gpio);
    ili9341_data(0x14, spi, gpio);
    ili9341_data(0x03, spi, gpio);
    ili9341_data(0x11, spi, gpio);
    ili9341_data(0x07, spi, gpio);
    ili9341_data(0x31, spi, gpio);
    ili9341_data(0xc1, spi, gpio);
    ili9341_data(0x48, spi, gpio);
    ili9341_data(0x08, spi, gpio);
    ili9341_data(0x0f, spi, gpio);
    ili9341_data(0x0c, spi, gpio);
    ili9341_data(0x31, spi, gpio);
    ili9341_data(0x36, spi, gpio);
    ili9341_data(0x0f, spi, gpio);

    ili9341_command(ILI9341_SLEEPOUT, spi, gpio); 
    ili9341_data(0x80, spi, gpio);
    delay_ms(150);
    
    ili9341_command(ILI9341_DISPON, spi, gpio); 
    ili9341_data(0x80, spi, gpio);
    delay_ms(150);

    //deselect display
    gpio_out(gpio, ILI9341_CE, 1);
}

void ili9341_swreset(spi_t *spi, gpio_t *gpio) {
    gpio_out(gpio, ILI9341_CE, 0);
    ili9341_command(ILI9341_SWRST, spi, gpio);
    gpio_out(gpio, ILI9341_CE, 1);

    delay_ms(300);
}

void ili9341_set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, spi_t *spi, gpio_t *gpio) {
    gpio_out(gpio, ILI9341_CE, 0);

    ili9341_command(ILI9341_COLSET, spi, gpio); // Column addr set
    ili9341_data(((x0 & 0xff00) >> 8), spi, gpio);
    ili9341_data(x0 & 0xff, spi, gpio);     // XSTART
    ili9341_data(((x1 & 0xff00) >> 8), spi, gpio);
    ili9341_data(x1 & 0xff, spi, gpio);     // XEND

    ili9341_command(ILI9341_PAGESET, spi, gpio); // Row addr set
    ili9341_data(((y0 & 0xff00) >> 8), spi, gpio);
    ili9341_data(y0 & 0xff, spi, gpio);     // YSTART
    ili9341_data(((y1 & 0xff00) >> 8), spi, gpio);
    ili9341_data(y1 & 0xff, spi, gpio);     // YEND

    ili9341_command(ILI9341_MEMWR, spi, gpio); // write to RAM

    gpio_out(gpio, ILI9341_CE, 1);
}

void ili9341_streampixel(uint16_t color, spi_t *spi) {
    uint8_t data1 = color>>8;
    uint8_t data2 = color&0xff;
    spi_write(spi, data1);
    spi_write(spi, data2);
}

void ili9341_streampixel_bytes(uint16_t color, uint16_t count, spi_t *spi){
    uint8_t data1 = color>>8;
    uint8_t data2 = color&0xff;
    for(uint16_t i=0;i<count;i++){
        spi_write(spi, data1);
        spi_write(spi, data2);
    }
}

void ili9341_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color, spi_t *spi, gpio_t *gpio) {
    ili9341_set_addr_window(x, y, x+w-1, y+h-1, spi, gpio);

    gpio_out(gpio, ILI9341_CE, 0);
    gpio_out(gpio, ILI9341_DC, 1);

    for(y=h; y>0; y--) {
        //for(x=w; x>0; x--) {
            ili9341_streampixel_bytes(color, w, spi);
        //}
    }

    gpio_out(gpio, ILI9341_CE, 1);
}

void ili9341_fill_screen(uint16_t color, spi_t *spi, gpio_t *gpio) {
    ili9341_fill_rect(0, 0, g_ili9341_width, g_ili9341_width, color, spi, gpio);
}

void ili9341_draw_pixel(int16_t x, int16_t y, uint16_t color, spi_t *spi, gpio_t *gpio) {
    ili9341_set_addr_window(x, y, x+1, y+1, spi, gpio);
    gpio_out(gpio, ILI9341_CE, 0);

    gpio_out(gpio, ILI9341_DC, 1);
    ili9341_streampixel(color, spi);
    gpio_out(gpio, ILI9341_CE, 1);
}

void ili9341_hwreset(gpio_t *gpio) {
    gpio_out(gpio, ILI9341_RES, 1);
    gpio_out(gpio, ILI9341_CE, 0);

    delay_ms(100);

    gpio_out(gpio, ILI9341_RES, 0);
    delay_ms(100);
    gpio_out(gpio, ILI9341_RES, 1);
    delay_ms(200);

    gpio_out(gpio, ILI9341_CE, 1);
}

void ili9341_command(uint8_t cmd, spi_t *spi, gpio_t *gpio) {
    // Setting command mode -> D = 1, C = 0
    gpio_out(gpio, ILI9341_DC, 0);
    spi_write(spi, cmd);
    gpio_out(gpio, ILI9341_DC, 1);
}

void ili9341_data(uint8_t dta, spi_t *spi, gpio_t *gpio) {
    // Setting command mode -> D = 1, C = 0
    gpio_out(gpio, ILI9341_DC, 1);
    spi_write(spi, dta);
}

void ili9341_set_rotation(uint8_t m, spi_t *spi, gpio_t *gpio) {
    rotation = m % 4;
    gpio_out(gpio, ILI9341_CE, 0);

    ili9341_command(ILI9341_MEMACTRL, spi, gpio); // Column addr set
    switch(rotation){
        case 0:
            ili9341_data(ILI9341_MADCTL_MX | ILI9341_MADCTL_RGB, spi, gpio);
            g_ili9341_width = ILI9341_WIDTH;
            g_ili9341_height = ILI9341_HEIGHT;
        break;
        case 1:
            ili9341_data(ILI9341_MADCTL_MV | ILI9341_MADCTL_RGB, spi, gpio);
            g_ili9341_width = ILI9341_HEIGHT;
            g_ili9341_height = ILI9341_WIDTH;
        break;
        case 2:
            ili9341_data(ILI9341_MADCTL_MY | ILI9341_MADCTL_RGB, spi, gpio);
            g_ili9341_width = ILI9341_WIDTH;
            g_ili9341_height = ILI9341_HEIGHT;
        break;
        case 3:
            ili9341_data(ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_RGB, spi, gpio);
            g_ili9341_width = ILI9341_HEIGHT;
            g_ili9341_height = ILI9341_WIDTH;
        break;
    }
    gpio_out(gpio, ILI9341_CE, 1);
}

uint8_t ili9341_get_rotation() {
    return rotation;
}

uint16_t ili9341_get_width() {
    return g_ili9341_width;
}

uint16_t ili9341_get_height() {
    return g_ili9341_height;
}

void ili9341_tearing_off(spi_t *spi, gpio_t *gpio) {
    gpio_out(gpio, ILI9341_CE, 0);

    ili9341_command(ILI9341_TEOFF , spi, gpio);

    gpio_out(gpio, ILI9341_CE, 1);
}

void ili9341_tearing_on(spi_t *spi, gpio_t *gpio) {
    gpio_out(gpio, ILI9341_CE, 0);

    ili9341_command(ILI9341_TEON , spi, gpio); 

    gpio_out(gpio, ILI9341_CE, 1);
}