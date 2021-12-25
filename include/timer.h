#ifndef TIMER_H
#define TIMER_H

typedef struct timer_t {
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t smcr;
    volatile uint32_t dier;
    volatile uint32_t sr;
    volatile uint32_t egr;
    volatile uint32_t ccmr[2];
    volatile uint32_t ccer;
    volatile uint32_t cnt;
    volatile uint32_t psc;
    volatile uint32_t arr;
    volatile uint32_t rcr;
    volatile uint32_t ccr[4];
    volatile uint32_t bdtr;
    volatile uint32_t dcr;
    volatile uint32_t dmar;

} timer_t;

#endif
