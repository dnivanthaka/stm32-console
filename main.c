#include "types.h"
#include "stmf103xxx.h"
#include "timer.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "i2c.h"
#include "interrupts.h"
#include "keypad.h"
#include "sound.h"
#include "console_system.h"
#include "adc.h"
#include "bitmap.h"

#define FPSCOUNT 1000/30 //(30fps) in ms

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

uint8_t ledVal = 0;
volatile uint16_t keypadkeys = 0xffff;

typedef struct coord_t {
    uint16_t x, y;
} coord_t;

void exti3_irq_handler(){
    //keypadkeys = keypad_read();

    EXTI->pr = (1 << 3); //Clearing the pending flag
}

void exti4_irq_handler(){
    keypadkeys = keypad_read();

    EXTI->pr = (1 << 4); //Clearing the pending flag
}

void exti9_5_irq_handler(){
    keypadkeys = keypad_read();

    EXTI->pr = (1 << 5); //Clearing the pending flag
}

void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    int xdiff = x2 - x1;
    int ydiff = y2 - y1;
    uint16_t error_term = 0;
    int x_inc = 1, y_inc = 1;

    if(ydiff < 0){
        ydiff = ABS(ydiff);
        y_inc = -1;
    }

    if(xdiff < 0){
        xdiff = ABS(xdiff);
        x_inc = -1;
    }

    //y  = mx
    if(xdiff > ydiff){
       for(uint16_t i=0;i < xdiff;i++){
          screen_putpixel(x1, y1, color);
	      x1 += x_inc;
          error_term += ydiff;

          if(error_term > xdiff){
	        y1 += y_inc;
	        error_term -= xdiff;
          } 
        }
    }else if (xdiff < ydiff){
       for(uint16_t i=0;i < ydiff;i++){
          screen_putpixel(x1, y1, color);
	      y1 += y_inc;
          error_term += xdiff;

          if(error_term > ydiff){
	        x1 += x_inc;
	        error_term -= ydiff;
          } 
        }
    }else{
    //xdiff = ydiff
       for(uint16_t i=0;i < xdiff;i++){
          screen_putpixel(x1, y1, color);
	      x1 += x_inc;
	      y1 += y_inc;
        }
    }
}

void draw_polygon(uint16_t count, coord_t *vertices, uint16_t color) { 
    for(int i=0;i<count-1;i++){
        draw_line((vertices + i)->x, (vertices + i)->y, (vertices + i + 1)->x, (vertices + i + 1)->y, color);
    }

    //draw the last connecting line
    draw_line(vertices->x, vertices->y, (vertices + (count-1))->x, (vertices + (count-1))->y, color);
}

void draw_vline(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color) {
    int xdiff = x2 - x1;
    int x_inc = 1;

    if(xdiff < 0){
        xdiff = ABS(xdiff);
        x_inc = -1;
    }
    for(uint16_t i=0;i < xdiff;i++){
       screen_putpixel(x1, y, color);
       x1 += x_inc;
    }
}

void draw_hline(uint16_t x, uint16_t y1, uint16_t y2,  uint16_t color) {
    int ydiff = y2 - y1;
    int y_inc = 1;

    if(ydiff < 0){
        ydiff = ABS(ydiff);
        y_inc = -1;
    }
    for(uint16_t i=0;i < ydiff;i++){
       screen_putpixel(x, y1, color);
       y1 += y_inc;
    }
}

void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    draw_vline(x, x+h, y, color);
    draw_hline(x+h, y, y+w, color);
    draw_vline(x+h, x, y+w, color);
    draw_hline(x, y+w, y, color);
}

void draw_triangle(coord_t *vertices, uint16_t color) {
    draw_polygon(3, vertices, color);
}

void draw_filled_triangle(coord_t *vertices, uint16_t color) {
    draw_polygon(3, vertices, color);

    /*for(){
    
    }*/
}

void draw_circle(int8_t x0, int8_t y0, int8_t r, uint16_t color) {
    int8_t f = 1 - r;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * r;
    int8_t x = 0;
    int8_t y = r;

    screen_putpixel(x0, y0 + r, color);
    screen_putpixel(x0, y0 - r, color);
    screen_putpixel(x0 + r, y0, color);
    screen_putpixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {

            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        screen_putpixel(x0 + x, y0 + y, color);
        screen_putpixel(x0 - x, y0 + y, color);
        screen_putpixel(x0 + x, y0 - y, color);
        screen_putpixel(x0 - x, y0 - y, color);
        screen_putpixel(x0 + y, y0 + x, color);
        screen_putpixel(x0 - y, y0 + x, color);
        screen_putpixel(x0 + y, y0 - x, color);
        screen_putpixel(x0 - y, y0 - x, color);

    }
}

void run_demo() {
    coord_t vertices[3];

    //Triangle
    vertices[0].x = 100, vertices[0].y = 10;
    vertices[1].x = 30,  vertices[1].y = 50;
    vertices[2].x = 100, vertices[2].y = 100;

    //draw_polygon(3, vertices, Color565(255,0,0));
    //draw_hline(50, 10, 60, Color565(0,255,0));
    //draw_vline(50, 10, 60, Color565(0,255,255));
    //draw_rect(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 50), rand() % 100, rand() % 50, Color565(255,255,0));
    //draw_rect(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 50), rand() % 100, rand() % 50, Color565(255,0,255));
    //draw_rect(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 50), rand() % 100, rand() % 50, Color565(0,255,255));
    //draw_circle(50, 50, 30, Color565(0, 255, 0));

    for(int i=0;i<1000;i++){
        //draw_line(rand() % (SCREEN_WIDTH - 1), rand() % (SCREEN_HEIGHT - 1), rand() % (SCREEN_WIDTH - 1), rand() % (SCREEN_HEIGHT - 1), Color565(rand() % 255, rand() % 255, rand() % 255));
    }
    delay_ms(100);
    //Clear screen
    //screen_fill(Color565(0,0,0));

    for(int i=0;i<100;i++){
        //draw_rect(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 50), rand() % 100, rand() % 50, Color565(rand() % 255, rand() % 255, rand() % 255));
    }

    //draw bitmap
    /*
    for(int y=0;y<100;y++){
        for(int x=0;x<100;x++){
            ili9341_draw_pixel(x, y, demo_image[x * y + 100], SPI1, GPIOA);
        }
    }
    */
}

void beep() {
    sound_on();
    //sound_ticks = systick_counter - sound_ticks;
    sound_off();
}

void test_timers() {
   //enable TIM2 clock
   RCC->apb2enr |= 0xFC;
   RCC->apb1enr |= (1 << 0);

   TIM2->ccer = 1 << 0; //CC1P = 0, CC1E = 1
   TIM2->ccmr[0] = 0x0068; //PWM1, OC1PE = 1
   TIM2->cr1 = 0x80;

   TIM2->psc = 720 - 1; //prescalar 72
   TIM2->arr = 1500 - 1; //ARR = 999
   TIM2->ccr[0] = 1000; //duty cycle (300 / 1000) * 100
   TIM2->egr = 1; // UG = 1 generate update
   TIM2->cr1 |= 0x01; //timer enable
}


int main(){
    uint16_t mcp_data = 0;
    uint32_t start_ticks = 0;
    uint32_t *tmp = (uint32_t *)0x20000100;

    system_init();
    adc_t *ADC1 = (adc_t *)ADC1BASE; 
    adc_init(ADC1);

    srand(systick_counter_get() + (*tmp) + (adc_get(ADC1) & 0x001f) + (adc_get(ADC1) & 0x001f) + ((adc_get(ADC1)) << 4));

    adc_off(ADC1);

    //Turn off led
    //gpio_out(gpio_c, 13, 0);
    ledVal = 0;

    //screen_fill(Color565(0,0,0));

    int x_vel = 0, y_vel = 0;
    uint16_t x_pos = 0, y_pos = 0, x_prev = 0, y_prev = 0;
	uint8_t inp;
    
    //draw_line(0, 0, 100, 100, Color565(255,0,0));
    //
    //sound_init();
    //beep();
    //run_demo();

    //screen_fill_rect(x_pos, y_pos, 8, 8, Color565(0,0,255));


    soundq_push(1, 200);

    while(1){

    soundq_process();
	//if(keypadkeys == 0xffff){
	    //continue;
	//}
	
	uint8_t inp = (keypad_read() & 0x00ff);
	//uint8_t inp = (keypadkeys & 0x00ff);
    //TODO need to solve the sticky key issue
	//inp = keypadkeys & 0x00ff;

	//uint8_t inp = gpio_in(gpio_b, 5);
	x_vel = 0;
	y_vel = 0;

	inp = ~inp;
        if(inp){
        soundq_push(1, 50);
	    //NB display is horizontal
	    //up	
	    if(KEYPAD_UP(inp)){
	     y_vel = -8;
	    }
	    //down	
	    if(KEYPAD_DOWN(inp)){
	     y_vel = 8;
	    }
	    //left
	    if(KEYPAD_LEFT(inp)){
	     x_vel = -8;
	    }
	    //right
	    if(KEYPAD_RIGHT(inp)){
	     x_vel = 8;
	    }
	}else{
	    continue;
	}

    start_ticks = systick_counter_get();
    //screen_fill_rect(x_pos, y_pos, 8, 8, Color565(0,0,0));

        x_prev = x_pos;
        y_prev = y_pos;
	/*
	inp = ((keypad_read(i2c1) & 0xff00) >> 8);
	inp = ~inp;
        if(inp){	
	    gpio_out(gpio_c, 13, 0);
	    continue;
	}
	gpio_out(gpio_c, 13, 1);
	*/
        if(x_pos + x_vel + 8 > SCREEN_WIDTH)
        {
            x_vel = 0;
        }
        if(x_pos + x_vel < 0)
        {
            x_vel = 0;
        }

        if(y_pos + y_vel + 8 > SCREEN_HEIGHT)
        {
            y_vel = 0;
        }

        if(y_pos + y_vel < 0)
        {
            y_vel = 0;
        }

        x_pos += x_vel;
        y_pos += y_vel;

        //screen_fill_rect(x_prev, y_prev, 8, 8, Color565(0,0,0));
        //screen_fill_rect(x_pos, y_pos, 8, 8, Color565(0,0,255));


    while(ABS(systick_counter_get() - start_ticks) < FPSCOUNT){

    }

	//reaches here whenever a button is pressed
	//beep();
    //delay_ms(1, 10);

    }
 return 0;
}
