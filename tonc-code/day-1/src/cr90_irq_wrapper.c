// This wrapper exists due to the `IRQ_*` name clash between tonc and ugba.
// Dang, I miss the namespace so much.

#include "cr90_irq_wrapper.h"

#include <ugba/interrupts.h>

void cr90_irq_init(void)
{
    IRQ_Init();
}

void cr90_irq_set_handler(cr90_IrqIndex index, cr90_IsrFunc function)
{
    IRQ_SetHandler((irq_index)index, function);
}

void cr90_irq_enable(cr90_IrqIndex index)
{
    IRQ_Enable((irq_index)index);
}

void cr90_irq_disable(cr90_IrqIndex index)
{
    IRQ_Disable((irq_index)index);
}

void cr90_irq_set_reference_vcount(uint32_t y)
{
    IRQ_SetReferenceVCOUNT(y);
}
