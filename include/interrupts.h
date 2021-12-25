#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define MAX_IRQ 68

#define EXTI0_IRQ 6
#define EXTI1_IRQ 7
#define EXTI2_IRQ 8
#define EXTI3_IRQ 9
#define EXTI4_IRQ 10 
#define EXTI9_5_IRQ 23

typedef struct exti_t {
    volatile uint32_t imr;
    volatile uint32_t emr;
    volatile uint32_t rtsr;
    volatile uint32_t ftsr;
    volatile uint32_t swier;
    volatile uint32_t pr;
} exti_t;

struct scb {
    volatile uint32_t actlr;
    volatile uint32_t cpuid;
    volatile uint32_t icsr;
    volatile uint32_t vtor;
    volatile uint32_t aircr;
} scb;

struct nvic {
    volatile uint32_t iser[3];
    volatile uint32_t icer[3];
    volatile uint32_t ispr[3];
    volatile uint32_t icpr[3];
    volatile uint32_t iabr[3];
    volatile uint32_t ipr[21];
    volatile uint32_t stir;
} nvic;

typedef struct afio_t {
    volatile uint32_t evcr;
    volatile uint32_t mapr;
    volatile uint32_t exticr[4];
    volatile uint32_t mapr2;
} afio_t;


void nvic_enable_irq(uint32_t irq);
void nvic_disable_irq(uint32_t irq);
void _disable_irq();
void _enable_irq();

#endif
