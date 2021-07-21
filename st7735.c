#include "types.h"
#include "console_system.h"
#include "rcc.h"
#include "gpio.h"
#include "spi.h"
#include "st7735.h"

#if ST7735_ROTATION
#define ST7735_COL_OFFSET 1
#define ST7735_ROW_OFFSET 2
#else
#define ST7735_COL_OFFSET 2
#define ST7735_ROW_OFFSET 1
#endif
#define ST7735_RES 2 
#define ST7735_DC  4
#define ST7735_CE  3

void st7735_test(gpio_t *gpio, spi_t *spi)
{
  int x_vel = 1, y_vel = 1;
  uint8_t x_pos, y_pos, x_prev, y_prev;

  //st7735_init(gpio, spi);

  st7735_fill_screen(Color565(0,0,0), gpio, spi);

  st7735_fill_rect(0, 0, 42, 160, Color565(255,0,0), gpio, spi);
  st7735_fill_rect(42, 0, 42, 160, Color565(0,255,0), gpio, spi);
  st7735_fill_rect(84, 0, 44, 160, Color565(0,0,255), gpio, spi);
  delay_ms(200);

  x_pos = 0;
  y_pos = 0;
  x_prev = 0;
  y_prev = 0;

  st7735_tearing_off(gpio, spi);
  while (1)
  {
        st7735_fill_rect(x_prev, y_prev, 8, 8, Color565(255,255,0), gpio, spi);
        st7735_fill_rect(x_pos, y_pos, 8, 8, Color565(0,0,0), gpio, spi);
        delay_ms(50);
        //st7735_fill_rect(x_pos, y_pos, 8, 8, Color565(0,0,255));

        x_prev = x_pos;
        y_prev = y_pos;

        if(x_pos + x_vel + 8 > ST7735_TFTWIDTH)
        {
            //x_pos = ST7735_TFTWIDTH;
            x_vel = -1;
        }

        if(x_pos + x_vel < 0)
        {
            //x_pos = 0;
            x_vel = 1;
        }

        if(y_pos + y_vel + 8 > ST7735_TFTHEIGHT)
        {
            //y_pos = ST7735_TFTHEIGHT;
            y_vel = -1;
        }

        if(y_pos + y_vel < 0)
        {
            //y_pos = 0;
            y_vel = 1;
        }

        x_pos += x_vel;
        y_pos += y_vel;

  }
}

//------------- ST7735 functions -------------------------//
// Pin configuration
// RES -> PA2
// CE  -> PA3
// D/C -> PA4

void st7735_hwreset(gpio_t *gpio)
{
    // Issuing command
    gpio_out(gpio, ST7735_RES, 1);
    //ST7735_RST = 1;
    gpio_out(gpio, ST7735_CE, 0);
    //ST7735_CE = 0;

    delay_ms(200);
    //__delay_ms(200);

    gpio_out(gpio, ST7735_RES, 0);
    //ST7735_RST = 0;

    delay_ms(200);
    //__delay_ms(200);

    gpio_out(gpio, ST7735_RES, 1);
    //ST7735_RST = 1;

    delay_ms(600);
    //__delay_ms(200);

    // Device deselect
    gpio_out(gpio, ST7735_CE, 1);
    //ST7735_CE = 1;
}

void st7735_command(uint8_t cmd, gpio_t *gpio, spi_t *spi)
{
    // Device select
    //ST7735_CE = 0;
    //gpio_out(gpio, ST7735_CE, 0);
    // Setting command mode -> D = 1, C = 0
    gpio_out(gpio, ST7735_DC, 0);
    //ST7735_DC = 0;
    spi_write(spi, cmd);
    //WriteSPI(cmd);
    // Device deselect
    //gpio_out(gpio, ST7735_CE, 1);
    //ST7735_CE = 1;
}

void st7735_data(uint8_t dta, gpio_t *gpio, spi_t *spi)
{
    // Device select
    //ST7735_CE = 0;
    //gpio_out(gpio, ST7735_CE, 0);
    // Setting data mode -> D = 1, C = 0
    gpio_out(gpio, ST7735_DC, 1);
    //ST7735_DC = 0;
    spi_write(spi, dta);
    //WriteSPI(cmd);
    // Device deselect
    //gpio_out(gpio, ST7735_CE, 1);
    //ST7735_CE = 1;
}

void st7735_init(gpio_t *gpio, spi_t *spi)
{
    gpio_out(gpio, ST7735_CE, 1);
    gpio_out(gpio, ST7735_DC, 1);
    gpio_out(gpio, ST7735_RES, 1);

    st7735_hwreset(gpio);

    //Select display
    gpio_out(gpio, ST7735_CE, 0);

    st7735_command(ST7735_SWRESET, gpio, spi);
    delay_ms(150);

    st7735_command(ST7735_SLPOUT, gpio, spi);
    delay_ms(500);

    st7735_command(ST7735_COLMOD, gpio, spi);
    st7735_data(0x05, gpio, spi);
    delay_ms(10);

    st7735_command(ST7735_FRMCTR1, gpio, spi);
    st7735_data(0x01, gpio, spi);
    st7735_data(0x2C, gpio, spi);
    st7735_data(0x2D, gpio, spi);
    //__delay_ms(10);

    st7735_command(ST7735_FRMCTR2, gpio, spi);
    st7735_data(0x01, gpio, spi);
    st7735_data(0x2C, gpio, spi);
    st7735_data(0x2D, gpio, spi);

    st7735_command(ST7735_FRMCTR3, gpio, spi);
    st7735_data(0x01, gpio, spi);
    st7735_data(0x2C, gpio, spi);
    st7735_data(0x2D, gpio, spi);
    st7735_data(0x01, gpio, spi);
    st7735_data(0x2C, gpio, spi);
    st7735_data(0x2D, gpio, spi);

    st7735_command(ST7735_INVCTR, gpio, spi);
    st7735_data(0x00, gpio, spi);

    st7735_command(ST7735_PWCTR1, gpio, spi);
    st7735_data(0xA2, gpio, spi);
    st7735_data(0x02, gpio, spi);
    st7735_data(0x84, gpio, spi);

    st7735_command(ST7735_PWCTR2, gpio, spi);
    st7735_data(0xC5, gpio, spi);

    st7735_command(ST7735_PWCTR3, gpio, spi);
    st7735_data(0x0A, gpio, spi);
    st7735_data(0x00, gpio, spi);

    st7735_command(ST7735_PWCTR4, gpio, spi);
    st7735_data(0x8A, gpio, spi);
    st7735_data(0x2A, gpio, spi);

    st7735_command(ST7735_PWCTR5, gpio, spi);
    st7735_data(0x8A, gpio, spi);
    st7735_data(0xEE, gpio, spi);

    st7735_command(ST7735_VMCTR1, gpio, spi);
    st7735_data(0x0E, gpio, spi);

    st7735_command(ST7735_INVOFF, gpio, spi);

    st7735_command(ST7735_MADCTL, gpio, spi);
    st7735_data(0xC8, gpio, spi);

    st7735_command(ST7735_DISSET5, gpio, spi);
    st7735_data(0x15, gpio, spi);
    st7735_data(0x02, gpio, spi);

    st7735_command(ST7735_COLMOD, gpio, spi);
    st7735_data(0x05, gpio, spi);

    st7735_command(ST7735_CASET, gpio, spi);
    st7735_data(0x00, gpio, spi);
    st7735_data(0x02, gpio, spi);
    st7735_data(0x00, gpio, spi);
    st7735_data(0x81, gpio, spi);

    st7735_command(ST7735_RASET, gpio, spi);
    st7735_data(0x00, gpio, spi);
    st7735_data(0x02, gpio, spi);
    st7735_data(0x00, gpio, spi);
    st7735_data(0x81, gpio, spi);                      // For green tab lcd

    st7735_command(ST7735_GMCTRP1, gpio, spi);
    st7735_data(0x02, gpio, spi);
    st7735_data(0x1C, gpio, spi);
    st7735_data(0x07, gpio, spi);
    st7735_data(0x12, gpio, spi);

    st7735_data(0x37, gpio, spi);
    st7735_data(0x32, gpio, spi);
    st7735_data(0x29, gpio, spi);
    st7735_data(0x2d, gpio, spi);

    st7735_data(0x29, gpio, spi);
    st7735_data(0x25, gpio, spi);
    st7735_data(0x2B, gpio, spi);
    st7735_data(0x39, gpio, spi);

    st7735_data(0x00, gpio, spi);
    st7735_data(0x01, gpio, spi);
    st7735_data(0x03, gpio, spi);
    st7735_data(0x10, gpio, spi);

    st7735_command(ST7735_GMCTRN1, gpio, spi);
    st7735_data(0x03, gpio, spi);
    st7735_data(0x1d, gpio, spi);
    st7735_data(0x07, gpio, spi);
    st7735_data(0x06, gpio, spi);

    st7735_data(0x2E, gpio, spi);
    st7735_data(0x2C, gpio, spi);
    st7735_data(0x29, gpio, spi);
    st7735_data(0x2D, gpio, spi);

    st7735_data(0x2E, gpio, spi);
    st7735_data(0x2E, gpio, spi);
    st7735_data(0x37, gpio, spi);
    st7735_data(0x3F, gpio, spi);

    st7735_data(0x00, gpio, spi);
    st7735_data(0x00, gpio, spi);
    st7735_data(0x02, gpio, spi);
    st7735_data(0x10, gpio, spi);

    st7735_command(ST7735_NORON, gpio, spi);
    delay_ms(10);

    st7735_command(ST7735_DISPON, gpio, spi);
    delay_ms(500);

    //deselect display
    gpio_out(gpio, ST7735_CE, 1);
}

void st7735_streampixel(uint16_t color, spi_t *spi) {
    uint8_t data1 = color>>8;
    uint8_t data2 = color&0xff;
    spi_write(spi, data1);
    spi_write(spi, data2);
    //WriteSPI(color>>8);
    //WriteSPI(color&0xff);
}

void st7735_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, gpio_t *gpio, spi_t *spi) {
    gpio_out(gpio, ST7735_CE, 0);
    //ST7735_CE = 0;

    st7735_command(ST7735_CASET, gpio, spi); // Column addr set
    st7735_data(0x00, gpio, spi);
    st7735_data(x0 + ST7735_COL_OFFSET, gpio, spi);     // XSTART
    st7735_data(0x00, gpio, spi);
    st7735_data(x1 + ST7735_COL_OFFSET, gpio, spi);     // XEND

    st7735_command(ST7735_RASET, gpio, spi); // Row addr set
    st7735_data(0x00, gpio, spi);
    st7735_data(y0 + ST7735_ROW_OFFSET, gpio, spi);     // YSTART
    st7735_data(0x00, gpio, spi);
    st7735_data(y1 + ST7735_ROW_OFFSET, gpio, spi);     // YEND

    st7735_command(ST7735_RAMWR, gpio, spi); // write to RAM

    gpio_out(gpio, ST7735_CE, 1);
    //ST7735_CE = 1;
}

void st7735_draw_pixel(int16_t x, int16_t y, uint16_t color, gpio_t *gpio, spi_t *spi) {

    st7735_set_addr_window(x, y, x+1, y+1, gpio, spi);

    gpio_out(gpio, ST7735_CE, 0);
    //ST7735_CE = 0;
    gpio_out(gpio, ST7735_DC, 1);
    //ST7735_DC = 1;
    st7735_streampixel(color, spi);
    gpio_out(gpio, ST7735_CE, 1);
    //ST7735_CE = 1;
}

void st7735_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color, gpio_t *gpio, spi_t *spi) {
    st7735_set_addr_window(x, y, x+w-1, y+h-1, gpio, spi);

    gpio_out(gpio, ST7735_CE, 0);
    //ST7735_CE = 0;
    gpio_out(gpio, ST7735_DC, 1);
    //ST7735_DC = 1;

    for(y=h; y>0; y--) {
        for(x=w; x>0; x--) {
            st7735_streampixel(color, spi);
        }
    }

    gpio_out(gpio, ST7735_CE, 1);
    //ST7735_CE = 1;
}

void st7735_fill_screen(uint16_t color, gpio_t *gpio, spi_t *spi) {
    st7735_fill_rect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, color, gpio, spi);
}

void st7735_tearing_off(gpio_t *gpio, spi_t *spi){
    gpio_out(gpio, ST7735_CE, 0);
    st7735_command(ST7735_TEOFF, gpio, spi);
    gpio_out(gpio, ST7735_CE, 1);
}

void st7735_tearing_on(gpio_t *gpio, spi_t *spi){
    gpio_out(gpio, ST7735_CE, 0);
    st7735_command(ST7735_TEON, gpio, spi);
    gpio_out(gpio, ST7735_CE, 1);
}

void st7735_set_rotation(uint8_t rotation, gpio_t *gpio, spi_t *spi) {

    gpio_out(gpio, ST7735_CE, 0);
    st7735_command(ST7735_MADCTL, gpio, spi);
    st7735_data(rotation, gpio, spi);
    gpio_out(gpio, ST7735_CE, 1);
}
