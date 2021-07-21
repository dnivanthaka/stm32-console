#include "types.h"
#include "stmf103xxx.h"
#include "rcc.h"
#include "gpio.h"
#include "spi.h"
#include "st7565r.h"

//CS  A0
//RSE A1
//RS  A2 - Register select

#define ST7565R_CS  8
#define ST7565R_RSE 9
#define ST7565R_RS  2

#define ST7565R_CS_PORT  GPIOB
#define ST7565R_RSE_PORT GPIOB
#define ST7565R_RS_PORT  GPIOA

volatile uint8_t buffer[(ST7565R_WIDTH * ST7565R_HEIGHT) / 8] = {
    0x7C, 0x12, 0x11, 0x12, 0x7C, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0, 0,
    0x20, 0x54, 0x54, 0x78, 0x40, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0, 0,// a
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x61, 0x59, 0x49, 0x4D, 0x43, 
};

void st7565r_init() {
    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 1);
    gpio_out(ST7565R_RSE_PORT, ST7565R_RSE, 1);
    gpio_out(ST7565R_RS_PORT, ST7565R_RS, 1);

    delay_ms(100);

    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 0);
    gpio_out(ST7565R_RSE_PORT, ST7565R_RSE, 0);
    delay_ms(100);

    gpio_out(ST7565R_RSE_PORT, ST7565R_RSE, 1);
    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 1);

    delay_ms(100);

    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 0);
    st7565r_command(ST7565R_BIAS_1_7);
    st7565r_command(ST7565R_ADC_NORMAL);
    st7565r_command(ST7565R_OUTPUT_MODE_NORM);
    st7565r_command(ST7565R_SET_START_LINE | 0);

    st7565r_command(ST7565R_POWER_CONTROL_SET | 0x4);
    delay_ms(100);
    st7565r_command(ST7565R_POWER_CONTROL_SET | 0x6);
    delay_ms(100);
    st7565r_command(ST7565R_POWER_CONTROL_SET | 0x7);
    delay_ms(20);

    st7565r_command(ST7565R_RESISTOR_RATIO_SET | 0x6);

    delay_ms(10);  
    /*st7565r_command(0x81);    //Electr0x2C onic Volume   
    st7565r_command(0x20);    //Reference Register selection  Vo=(1+Rb/Ra)(1+a)*2.1=10   
    st7565r_command(0x2c);    //Power Control(Vc=1;Vr=0;Vf=0)   
    delay_ms(10);  
    st7565r_command(0x2c|0x2a);  
    delay_ms(10);  
    st7565r_command(0x2c|0x2a|0x29);  
    delay_ms(10);  
    st7565r_command(0xF8);  
    st7565r_command(0x01);  
  
    delay_ms(5);  
    st7565r_command(0xAF);    //Display on   
    st7565r_command(0xA6);
    */

    //st7565r_command(ST7565R_SET_PAGE_ADDR | 0x07);

    st7565r_command(0x81);    //Electronic Volume   
    st7565r_command(0x09);    //Reference Register selection  Vo=(1+Rb/Ra)(1+a)*2.1=10   

    st7565r_command(ST7565R_OUTPUT_MODE_NORM);

    st7565r_command(ST7565R_DISPLAYON);
    st7565r_command(ST7565R_DISPLAY_ALL_POINTS_NORM);
    st7565r_command(ST7565R_SET_PAGE_ADDR | 0);
    st7565r_command(ST7565R_SET_START_LINE | 0);
    
    st7565r_command(0x19);   //constrast
   
    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 1);

    st7565r_clear();
}

void st7565r_command(uint8_t c) {
    gpio_out(ST7565R_RS_PORT, ST7565R_RS, 0);

    spi_write(SPI1, c);
}

void st7565r_data(uint8_t d) {
    gpio_out(ST7565R_RS_PORT, ST7565R_RS, 1);

    spi_write(SPI1, d);
}

void st7565r_clear() {
    st7565r_fill_screen(0);
}

void st7565r_fill_screen(uint8_t color) {
    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 0);
    for(int p=0;p<8;p++){
        st7565r_command(ST7565R_SET_PAGE_ADDR | p);

        for(int c=0;c<128;c++){
            st7565r_command(ST7565R_COL_ADDR_SET_LOWER | (c & 0xf));
            st7565r_command(ST7565R_COL_ADDR_SET_UPPER | ((c >> 4) & 0xf));

            st7565r_data(color);
        }
    }
    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 1);
}

void st7565r_set_location(uint8_t page, uint16_t col) {
    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 0);

    st7565r_command(ST7565R_SET_PAGE_ADDR | page);
    st7565r_command(ST7565R_COL_ADDR_SET_LOWER | (col & 0xf));
    st7565r_command(ST7565R_COL_ADDR_SET_UPPER | ((col >> 4) & 0xf));
    
    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 1);
}

void st7565r_update() {
    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 0);
    
    //st7565r_set_location(0, 0);

    for(int p=0;p<8;p++){
        st7565r_command(ST7565R_SET_PAGE_ADDR | p); //Start from the top

        st7565r_command(ST7565R_COL_ADDR_SET_UPPER | 0); 
        st7565r_command(ST7565R_COL_ADDR_SET_LOWER | 0);
        //st7565r_command(ST7565R_RMW_WRITE);

        for(int c=127;c>=0;c--){

            st7565r_data(buffer[((p) * ST7565R_WIDTH) + c]);
        }
    }
    gpio_out(ST7565R_CS_PORT, ST7565R_CS, 1);
}
