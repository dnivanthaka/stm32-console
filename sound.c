#include "types.h"
#include "sound.h"
#include "stmf103xxx.h"

#define SOUNDQ_SIZE 8

static volatile uint32_t sound_ticks = 0;
static uint8_t start = 0;
static uint8_t end   = 0;
static uint8_t sound_playing = 0;

typedef struct soundq_t {
    uint8_t tone;
    uint16_t duration;
} soundq_t;

soundq_t sq[SOUNDQ_SIZE];

void soundq_init() {
    sound_off();
    start = 0;
    end = 0;
    sound_playing = 0;
    sound_ticks = 0;
}

void soundq_push(uint8_t tone, uint16_t dur) {
    sq[end].tone = tone;
    sq[end].duration = dur;

    end = ((end + 1) < SOUNDQ_SIZE) ? (end + 1) : SOUNDQ_SIZE - 1;

    sound_playing = 1;
}

void soundq_process() {
    if(sound_playing == 1){
        //check tone here
        sound_on();

        //update duration
        if(sound_ticks > 0){
            int temp = sq[start].duration;
            sq[start].duration = (temp - ABS(systick_counter_get() - sound_ticks) <= 0) ? 0 : 
                sq[start].duration - ABS(systick_counter_get() - sound_ticks);
            if(sq[start].duration == 0){
                if(start == (SOUNDQ_SIZE - 1) || start == end){
                    //terminating condition
                    soundq_init();
                }else{
                    //play the next tone
                    start++;
                    sound_ticks = systick_counter_get();
                }
            }else{
                sound_ticks = systick_counter_get();
            }
        }else{
            sound_ticks = systick_counter_get();
        }

    }
}

void sound_init() {
   //enable TIM2 clock
   RCC->apb2enr |= 0xFC;
   RCC->apb1enr |= (1 << 0);

   TIM2->ccer = 1 << 0; //CC1P = 0, CC1E = 1
   TIM2->ccmr[0] = 0x0068; //PWM1, OC1PE = 1
   TIM2->cr1 = 0x80;

   TIM2->psc = 720 - 1; //prescalar 72
   TIM2->arr = 1010 - 1; //ARR = 999
   TIM2->ccr[0] = 1000; //duty cycle (300 / 1000) * 100
   TIM2->egr = 1; // UG = 1 generate update
   TIM2->cr1 |= 0x01; //timer enable

   soundq_init();
}

void sound_on() {
   TIM2->cr1 |= 0x01; //timer enable
}

void sound_off() {
   TIM2->cr1 &= ~(0x01); //timer disable
}

void sound_tick_update() {
    sound_ticks--;
}