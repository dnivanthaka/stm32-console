#include "types.h"
#include "stm32f103xxx.h"
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
#include "font.h"
#include "eventq.h"
#include "dma.h"
#include "system.h"

//#define FPSCOUNT 1000/60 //(60fps) in ms

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

uint8_t ledVal = 0;
volatile uint16_t keypadkeys = 0xffff;

typedef struct coord_t {
    int x, y;
} coord_t;

typedef struct entity_t {
    uint8_t w, h;
    int x_vel, y_vel;
    coord_t pos, prev1, prev2;
} entity_t;

typedef struct menuitem_t {
    char *str;
    uint8_t (*func_ptr)();
} menuitem_t;


eventq_t events[8];

void exti3_irq_handler(){
    //keypad_read();

    EXTI->pr = (1 << 3); //Clearing the pending flag
}

void exti4_irq_handler(){
    keypad_read();

    EXTI->pr = (1 << 4); //Clearing the pending flag
}

void exti9_5_irq_handler(){
    //keypadkeys = keypad_read();

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

void draw_entity(entity_t *entity, uint8_t color){
    for(uint8_t i=0;i<entity->h;i++){
        for(uint8_t j=0;j<entity->w;j++){
            st7565r_putpixel(entity->pos.x + j, entity->pos.y + i, color);
        }
    }
}

void draw_char(uint8_t x, uint8_t y,uint8_t c){
    uint16_t idex = c * 5;
    if(c != '\n'){
        //st7565r_put(loc, &Font[c * 5], 5);
        for(uint8_t h = 0;h < 7;h++){
            for(uint8_t w=0;w < 5;w++){
                st7565r_putpixel(x + w, y + h, Font[idex + w] & (1 << h));
            }
        }
    }
}

uint8_t func1(){
    st7565r_putpixel(127, 10, 1);
}

uint8_t func2(){
    st7565r_putpixel(0, 10, 1);
}

void draw_text(uint8_t x, uint8_t y, uint8_t *str){
    uint8_t i = 0;

    while(*str){
        draw_char(x + i, y, *str);
        str++;
        i += 6;
    }
}

int main(){
    uint16_t mcp_data = 0;
    uint32_t start_ticks = 0;
    uint32_t *tmp = (uint32_t *)0x20000100;
    uint8_t mid_x = SCREEN_WIDTH_MID;
    uint8_t mid_y = SCREEN_HEIGHT_MID;
    uint16_t frame = 0;

    system_init();
    adc_t *ADC1 = (adc_t *)ADC1BASE; 
    adc_init(ADC1);

    srand(systick_counter_get() + (*tmp) + (adc_get(ADC1) & 0x001f) + (adc_get(ADC1) & 0x001f) + ((adc_get(ADC1)) << 4));

    adc_off(ADC1);

    setup();

    /*
    for(uint16_t i=0;i<(255 * 5);i+=5){
        st7565r_put(i, &Font[i], 5);
    }*/
    //st7565r_update();
    uint8_t x = 0, y = 0;
    int x_vel = 0, y_vel = 0;
    //uint8_t ball[] = {0x0f, 0x0f, 0x0f, 0x0f};
    //uint8_t ball_bk[] = {0, 0, 0, 0};

    menuitem_t item1;
    item1.str = "Left Dot";
    item1.func_ptr = &func2;

    menuitem_t item2;
    item2.str = "Right Dot";
    item2.func_ptr = &func1;

    entity_t paddle;
    paddle.w = 13;
    paddle.h = 2;
    paddle.pos.x = mid_x - 6;
    paddle.pos.y = SCREEN_HEIGHT - 4;
    paddle.prev1.x = paddle.pos.x;
    paddle.prev1.y = paddle.pos.y;
    paddle.prev2.x = paddle.pos.x;
    paddle.prev2.y = paddle.pos.y;
    paddle.x_vel = 0;
    paddle.y_vel = 0;

    entity_t ball;
    ball.w = 4;
    ball.h = 4;
    //ball.pos.x = SCREEN_WIDTH_MID - 1;
    ball.pos.x = rand() % SCREEN_WIDTH;
    ball.pos.y = 0;
    ball.prev1.x = ball.pos.x;
    ball.prev1.y = ball.pos.y;
    ball.prev2.x = ball.pos.x;
    ball.prev2.y = ball.pos.y;
    ball.x_vel = 2;
    ball.y_vel = 2;

    entity_t old_paddle, old_ball;

    old_paddle = paddle;
    old_ball = ball;

    draw_entity(&paddle, 1);
    //st7565r_update();

    for(;;){

        rcc_vsync_wait(1);

        paddle.x_vel = 0;
        paddle.y_vel = 0;
        
        //keypad_read();
        uint8_t tmp = keypad_getstate();
        //wait here until key press
        /*if(tmp == KEYPAD_DEFAULT_VAL){
            continue;
        }*/
        
        //start_ticks = systick_counter_get();

        if(KEYPAD_UP(tmp)) {
            //gpio_out(GPIOA, 0, 1);
            //y_vel = -1;
        }else if(KEYPAD_DOWN(tmp)){
            //gpio_out(GPIOA, 1, 1);
            //y_vel = 1;
        }else if(KEYPAD_LEFT(tmp)){
            //gpio_out(GPIOA, 0, 1);
            //x_vel = -1;
            paddle.x_vel = -4;
        }else if(KEYPAD_RIGHT(tmp)){
            //gpio_out(GPIOA, 1, 1);
            //x_vel = 1;
            paddle.x_vel = 4;
        }else if(KEYPAD_A(tmp)){
            //gpio_out(GPIOA, 0, 1);
        }else if(KEYPAD_B(tmp)){
            //gpio_out(GPIOA, 1, 1);
        }else if(KEYPAD_SELECT(tmp)){
            //gpio_out(GPIOA, 0, 1);
        }else if(KEYPAD_START(tmp)){
            //gpio_out(GPIOA, 1, 1);
        }else{
            //gpio_out(GPIOA, 0, 0);
            //gpio_out(GPIOA, 1, 0);
            //x_vel = 0;
            //y_vel = 0;
        }

        //st7565r_putpixel(x, y, 0);
        /*if(frame == 4){
            draw_entity(&paddle, 0);
            draw_entity(&ball, 0);
        }*/

        if(paddle.pos.x + paddle.x_vel + paddle.w >= SCREEN_WIDTH){
            //x_vel = -1;
            paddle.x_vel = 0;
            paddle.pos.x = SCREEN_WIDTH - paddle.w;
        }else if(paddle.pos.x + paddle.x_vel < 0){
            //x_vel = 1;
            paddle.x_vel = 0;
            paddle.pos.x = 0;
        }

        //if(ball.pos.x + ball.x_vel + ball.w > SCREEN_WIDTH){
        if(ball.pos.x + ball.w > SCREEN_WIDTH){
            //x_vel = -1;
            ball.x_vel = -2;
            //ball.pos.x = SCREEN_WIDTH;
        }
        if(ball.pos.x + ball.x_vel < 0){
            //x_vel = 1;
            ball.x_vel = 2;
            //ball.pos.x = -1;
        }
        //if(ball.pos.y + ball.y_vel + ball.h > SCREEN_HEIGHT){
        if(ball.pos.y + ball.h > SCREEN_HEIGHT){
            //x_vel = -1;
            ball.y_vel = -2;
            //ball.pos.y = SCREEN_HEIGHT;
        }
        if(ball.pos.y + ball.y_vel < 0){
            //x_vel = 1;
            ball.y_vel = 2;
            //ball.pos.y = -1;
        }


        if(frame % 4 == 0){
        ball.prev2.x = ball.prev1.x;
        ball.prev2.y = ball.prev1.y;

        ball.prev1.x = ball.pos.x;
        ball.prev1.y = ball.pos.y;

        ball.pos.x += ball.x_vel;
        ball.pos.y += ball.y_vel;
            //old_ball.pos.x = old_ball.prev1.x;
            //old_ball.pos.y = old_ball.prev1.y;

            //remove last object
            draw_entity(&old_ball, 0);

            /*ball.prev2.x = ball.prev1.x;
            ball.prev2.y = ball.prev1.y;

            ball.prev1.x = ball.pos.x;
            ball.prev1.y = ball.pos.y;

            ball.pos.x += ball.x_vel;
            ball.pos.y += ball.y_vel;*/
            
            draw_entity(&ball, 1);
            
            old_ball = ball;
            //delay_ms(25);
            //rcc_vsync_wait(2);
        }


        if(frame % 1 == 0){
        paddle.prev2.x = paddle.prev1.x;
        paddle.prev2.y = paddle.prev1.y;

        paddle.prev1.x = paddle.pos.x;
        paddle.prev1.y = paddle.pos.y;

        paddle.pos.x += paddle.x_vel;
        paddle.pos.y += paddle.y_vel;
            //paddle.pos.x += paddle.x_vel;
            //paddle.pos.y += paddle.y_vel;

        //st7565r_putpixel(x, y, 1);
            draw_entity(&old_paddle, 0);
            draw_entity(&paddle, 1);
            old_paddle = paddle;
            //rcc_vsync_wait(4);
            //frame = 0;
            //continue;
        }
        //draw_entity(&paddle, 1);
        //rcc_vsync_wait(4);
        //draw_entity(&ball, 1);

        //gpio_out(GPIOA, 0, 1);
        //st7565r_clear();
        //screen_fill(0x0f);
        //delay_ms(500);
        //screen_fill(0xf0);
        //gpio_out(GPIOA, 0, 0);
        //st7565r_put(0, &Font[48 * 5], 5);
        //draw_char(0, 0, 'd');
        //draw_text(50, 0, "Item 1");
        //draw_text(50, 8, "Item 2");
        //draw_text(50, 16, "Item 3");
        //item2.func_ptr();
        //st7565r_update();
        /*while(ABS(systick_counter_get() - start_ticks) < FPSCOUNT){

        }*/
        frame++;
        //rcc_vsync_wait(1);
    }

    //Turn off led
    //gpio_out(gpio_c, 13, 0);
    ledVal = 0;

    //screen_fill(Color565(0,0,0));
 return 0;
}
