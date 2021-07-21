#ifndef ADC_H
#define ADC_H

typedef struct adc_t {
    volatile uint32_t sr;
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t smpr[2];
    volatile uint32_t jofr[4];
    volatile uint32_t htr;
    volatile uint32_t ltr;
    volatile uint32_t sqr[3];
    volatile uint32_t jsqr;
    volatile uint32_t jdr[4];
    volatile uint32_t dr;
} adc_t;

void adc_init(adc_t *adc);
uint32_t adc_get(adc_t *adc);
void adc_off(adc_t *adc);

#endif
