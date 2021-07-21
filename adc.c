#include "types.h"
#include "stmf103xxx.h"
#include "console_system.h"
#include "adc.h"

void adc_init(adc_t *adc) {
    gpio_init(GPIOA, RCC, 1, GPIO_MODE_INPUT | GPIO_CNF_IN_ANALOG);
    RCC->apb2enr |= (1 << 9);

    adc->cr2 = 1; //ADON
    adc->smpr[1] = 1 << 3; //SMP1 = 001, 7.5 clocks

    delay_ms(1);
}

uint32_t adc_get(adc_t *adc) {
    adc->sqr[2] = 1;
    adc->cr2 = 1; //ADON start conversion
    while((adc->sr & (1 << 1)) == 0);

    return adc->dr;
}

void adc_off(adc_t *adc) {
    adc->cr2 &= ~(1); //ADON
}