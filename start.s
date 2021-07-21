.section .vectors
.cpu cortex-m3
.thumb

/*---------------- Setup vector table entries -----------------*/

.word 0x20005000 /*SP 20480bytes = 0x5000*/
.word _reset     /*Reset*/
.word hang       /*NMI*/
.word hang       /*Hard fault*/
.word hang       /*MM fault*/
.word hang       /*Bus fault*/
.word hang       /*Usage fault*/
.word hang       /*Reserved*/
.word hang       /*Reserved*/
.word hang       /*Reserved*/
.word hang       /*Reserved*/
.word hang       /*SVCall*/
.word hang       /*Debug - reserved*/
.word hang       /*Reserved*/
.word hang       /*Pend SV*/
.word systick_handler       /*Systick*/

.word hang       /*Watchdog window*/
.word hang       /*PVD*/
.word hang       /*TAMPER*/
.word hang       /*RTC global*/
.word hang       /*Flash global*/
.word hang       /*RCC global*/
.word hang       /*EXTI0*/
.word hang       /*EXTI1*/
.word hang       /*EXTI2*/
.word exti3_irq_handler       /*EXTI3*/
.word exti4_irq_handler       /*EXTI4*/
.word hang       /*DMA 1*/
.word hang       /*DMA 2*/
.word hang       /*DMA 3*/
.word hang       /*DMA 4*/
.word hang       /*DMA 5*/
.word hang       /*DMA 6*/
.word hang       /*DMA 7*/
.word hang       /*ADC1_2*/
.word hang       /*CAN1_TX*/
.word hang       /*CAN1_RX0*/
.word hang       /*CAN_RX1*/
.word hang       /*CAN_SCE*/
.word exti9_5_irq_handler  /*EXTI 9 -5*/

.section .text

.thumb_func
hang: b hang

.thumb_func
_reset:
 bl startup
 b .             /*Should not reach here*/

.align

.thumb_func
.globl PUT16
PUT16:
 strh r1, [r0]
 bx lr

.thumb_func
.globl PUT32
PUT32:
 str r1, [r0]
 bx lr

.thumb_func
.globl GET32
GET32:
 ldr r0, [r0]
 bx lr

.end
