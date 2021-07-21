#ifndef RCC_H
#define RCC_H

#define PLL_HSE		0x10000	/* 1 is HSE, 0 is HSI/2 */
#define PLL_HSI		0x00000

#define	SYS_PLL		0x02	/* HSE multiplied by PLL */
#define APB1_DIV2	(4<<8)	/* 36 Mhz max */
#define	SYS_HSI		0x00	/* reset value - 8 Mhz internal RC */
#define	SYS_PLL		0x02

#define PLL_2		0
#define PLL_3		(1<<18)
#define PLL_4		(2<<18)
#define PLL_5		(3<<18)
#define PLL_6		(4<<18)
#define PLL_7		(5<<18)
#define PLL_8		(6<<18)
#define PLL_9		(7<<18)
#define PLL_10		(8<<18)
#define PLL_11		(9<<18)
#define PLL_12		(10<<18)
#define PLL_13		(11<<18)
#define PLL_14		(12<<18)
#define PLL_15		(13<<18)
#define PLL_16		(15<<18)

#define PPRE2_0         0
#define PPRE2_2         (4<<11)
#define PPRE2_4         (5<<11)
#define PPRE2_8         (6<<11)
#define PPRE2_16        (7<<11)

#define PPRE1_0         0
#define PPRE1_2         (4<<8)
#define PPRE1_4         (5<<8)
#define PPRE1_8         (6<<8)
#define PPRE1_16        (7<<8)

#define APB1_DIV2	(4<<8)

#define PLL_HSE		0x10000

#define HSI_ON		1
#define HSE_ON		0x10000
#define HSE_TRIM	0x80

#define CCR_NORM	(HSI_ON | HSE_ON | HSE_TRIM)

/* Bits in the clock control register CCR */
#define PLL_ENABLE	0x01000000
#define FLASH_ACR      ((volatile uint32_t *) 0x40022000)

#define SYT_COUNTF   (1 << 16)
#define SYT_SRC      (1 << 2)
#define SYT_ENABLE   (1)
#define SYT_INT      (1 << 1)


typedef struct rcc_t {
    volatile uint32_t cr;
    volatile uint32_t cfgr;
    volatile uint32_t cir;
    volatile uint32_t apb2rstr;
    volatile uint32_t abp1rstr;
    volatile uint32_t ahbenr;
    volatile uint32_t apb2enr;
    volatile uint32_t apb1enr;
    volatile uint32_t bdcr;
    volatile uint32_t csr;
    volatile uint32_t ahbrstr;
    volatile uint32_t cfgr2;
} rcc_t;

typedef struct systick_t {
    volatile uint32_t ctrl;
    volatile uint32_t load;
    volatile uint32_t val;
    volatile uint32_t calib;
} systick_t;

void rcc_init(rcc_t *rcc);
void systick_init(systick_t *syt);
//void delay_ms(systick_t *syt, uint32_t count);
void _delay_ms(timer_t *tim, uint32_t count);
void _delay_us(timer_t *tim, uint32_t count);
void rcc_setup_cpu(rcc_t *rcc, uint32_t pll_clk, uint32_t apb_clk);
void systick_interrupt_start(systick_t *syt);
void systick_counter_set(uint32_t val);
uint32_t systick_counter_get();
void srand(uint16_t seed);
uint16_t rand();

//delay routines
#define delay_ms(y) _delay_ms(TIM3, y)
#define delay_us(y) _delay_us(TIM3, y)

#endif
