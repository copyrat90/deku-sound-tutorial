// This wrapper exists due to the `IRQ_*` name clash between tonc and ugba.
// Dang, I miss the namespace so much.

#pragma once

#include <stdint.h>

typedef enum
{
    CR90_IRQ_VBLANK = 0,
    CR90_IRQ_HBLANK = 1,
    CR90_IRQ_VCOUNT = 2,
    CR90_IRQ_TIMER0 = 3,
    CR90_IRQ_TIMER1 = 4,
    CR90_IRQ_TIMER2 = 5,
    CR90_IRQ_TIMER3 = 6,
    CR90_IRQ_SERIAL = 7,
    CR90_IRQ_DMA0 = 8,
    CR90_IRQ_DMA1 = 9,
    CR90_IRQ_DMA2 = 10,
    CR90_IRQ_DMA3 = 11,
    CR90_IRQ_KEYPAD = 12,
    CR90_IRQ_GAMEPAK = 13,
} cr90_IrqIndex;

typedef void (*cr90_IsrFunc)(void);

void cr90_irq_init(void);
void cr90_irq_set_handler(cr90_IrqIndex index, cr90_IsrFunc function);
void cr90_irq_enable(cr90_IrqIndex index);
void cr90_irq_disable(cr90_IrqIndex index);
void cr90_irq_set_reference_vcount(uint32_t y);
