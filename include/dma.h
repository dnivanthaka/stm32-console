#ifndef DMA_H
#define DMA_H

#include "types.h"

typedef struct dma_channel_t {
    volatile uint32_t ccr;
    volatile uint32_t cndtr;
    volatile uint32_t cpar;
    volatile uint32_t cmar;
} dma_channel_t;

typedef struct dma_t {
    volatile uint32_t isr;
    volatile uint32_t ifcr;
    dma_channel_t *chan;
} dma_t;

#define DMA

inline uint32_t dma_mem_calc(uint32_t base, uint8_t chan){
    return base + 0x08 + (0x0d20 * (chan - 1));
}

#endif