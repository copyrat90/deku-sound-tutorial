#include <tonc.h>
#include "cr90_irq_wrapper.h"

#include "Piano.raw.h"

void isr_timer1_stop_playback()
{
    // stop the DMA 1
    REG_DMA1CNT_H = 0;
}

void isr_timer1_loop_playback()
{
    // loop `Piano.raw` sample from pos=9176
    const int loop_pos = 9176;

    REG_DMA1CNT_H = 0;
    REG_DMA1SAD = (u32)Piano_raw + loop_pos;
    REG_DMA1CNT = DMA_DST_FIXED | DMA_REPEAT | DMA_32 | DMA_AT_FIFO | DMA_ENABLE;

    REG_TM1CNT = 0;
    REG_TM1D = 65536 - (Piano_raw_size - loop_pos);
    REG_TM1CNT = TM_CASCADE | TM_IRQ | TM_ENABLE;
}

int main(void)
{
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

    cr90_irq_init();

    cr90_irq_set_handler(CR90_IRQ_TIMER1, isr_timer1_stop_playback);
    // cr90_irq_set_handler(CR90_IRQ_TIMER1, isr_timer1_loop_playback);

    cr90_irq_enable(CR90_IRQ_VBLANK);
    cr90_irq_enable(CR90_IRQ_TIMER1);

    // init Direct Sound A w/ Timer 0
    REG_SNDSTAT = SSTAT_ENABLE;
    REG_SNDDSCNT = SDS_A100 | SDS_AL | SDS_AR | SDS_ATMR0 | SDS_ARESET;

    // enable Timer 0 (FIFO)
    const int sound_freq = 8363;
    REG_TM0D = 65536 - ((1 << 24) / sound_freq);
    REG_TM0CNT = TM_ENABLE;

    // enable DMA 1
    REG_DMA1SAD = (u32)Piano_raw;
    REG_DMA1DAD = (u32)&REG_FIFO_A;
    REG_DMA1CNT = DMA_DST_FIXED | DMA_REPEAT | DMA_32 | DMA_AT_FIFO | DMA_ENABLE;

    // enable Timer 1 (stop playback)
    REG_TM1D = 65536 - Piano_raw_size;
    REG_TM1CNT = TM_CASCADE | TM_IRQ | TM_ENABLE;

    // text
    tte_init_chr4c_default(0, BG_CBB(0) | BG_SBB(31));
    tte_set_pos(90, 70);
    tte_write("single-sound");

    while (1)
    {
        VBlankIntrWait();
    }
}
