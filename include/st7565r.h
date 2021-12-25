#ifndef ST7565R_H
#define ST7565R_H

#define ST7565R_DISPLAYON            0b10101111
#define ST7565R_DISPLAYOFF           0b10101110
#define ST7565R_SET_START_LINE       0b01000000
#define ST7565R_SET_PAGE_ADDR        0b10110000
#define ST7565R_COL_ADDR_SET_UPPER   0b00010000
#define ST7565R_COL_ADDR_SET_LOWER   0b00000000
#define ST7565R_STATUS_READ          0b00000000

#define ST7565R_ADC_NORMAL           0b10100000
#define ST7565R_ADC_REVERSE          0b10100001

#define ST7565R_DISPLAY_NORMAL       0b10100110
#define ST7565R_DISPLAY_REVERSE      0b10100111

#define ST7565R_DISPLAY_ALL_POINTS_NORM  0b10100100
#define ST7565R_DISPLAY_ALL_POINTS_ON    0b10100101

#define ST7565R_BIAS_1_9             0b10100010
#define ST7565R_BIAS_1_7             0b10100011

#define ST7565R_RMW_READ             0b11100000
#define ST7565R_RMW_WRITE            0b11100001
#define ST7565R_RMW_END              0b11101110

#define ST7565R_RESET                0b11100010

#define ST7565R_OUTPUT_MODE_NORM     0b11000000
#define ST7565R_OUTPUT_MODE_REVERSE  0b11001000

#define ST7565R_POWER_CONTROL_SET    0b00101000

#define ST7565R_RESISTOR_RATIO_SET   0b00100000

#define ST7565R_VOLUME_MODE_SET      0b10000001
#define ST7565R_VOLUME_REGISTER_SET  0b00000000

#define ST7565R_STATIC_INDICATOR_OFF 0b10101100
#define ST7565R_STATIC_INDICATOR_ON  0b10101101

#define ST7565R_STATIC_REGISTER_SET  0b00000000

#define ST7565R_BOOSTER_RATIO_SET    0b11111000

#define ST7565R_NOP                  0b11100011

#define ST7565R_WIDTH  128
#define ST7565R_HEIGHT 64
#define ST7565R_PAGE_COUNT 8

void st7565r_init();
void st7565r_command(uint8_t c);
void st7565r_data(uint8_t d);
void st7565r_clear();
void st7565r_fill_screen(uint8_t color);
void st7565r_set_location(uint8_t page, uint16_t col);
void st7565r_update();
void st7565r_put(uint8_t loc, const uint8_t *buff, uint8_t count);
void st7565r_putpixel(uint8_t x, uint8_t y, uint8_t color);
uint8_t st7565r_getpixel(uint8_t x, uint8_t y);

#endif