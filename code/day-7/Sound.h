// ----- Change log -----
// 10/08/04 - Changed MOD_NO_SAMPLE from 0 to 31
//          - Added structures for MOD player
//          - Modified SndMix to take a number of samples, added MODUpdate
//            to update the music, and SndUpdate to do the deciding how many
//            samples to mix at a time, and call MODUpdate as needed
// 10/21/04 - Moved MOD_NO_NOTE/MOD_NO_SAMPLE defines to Sound.c,
//            since nothing outside player needs to know them
// ----------------------

#ifndef SOUND_H
#define SOUND_H

#include "Gba.h"

// ----- Constants -----

#define SND_MAX_CHANNELS 4
#define SND_CHN_INVALID 0xff

typedef enum _SND_FREQ {
  SND_FREQ_5734,
  SND_FREQ_10512,
  SND_FREQ_13379,
  SND_FREQ_18157,
  SND_FREQ_21024,
  SND_FREQ_26758,
  SND_FREQ_31536,
  SND_FREQ_36314,
  SND_FREQ_40137,
  SND_FREQ_42048,
  SND_FREQ_43959,

  SND_FREQ_NUM

} SND_FREQ;

typedef enum _MOD_STATE {
  MOD_STATE_STOP,
  MOD_STATE_PLAY,
  MOD_STATE_PAUSE,

} MOD_STATE;

// ----- Structures -----

typedef struct _SOUND_CHANNEL {
  const s8 *data;
  u32 pos;
  u32 inc;
  u32 vol;
  u32 length;
  u32 loopLength;

} SOUND_CHANNEL;

typedef struct _SOUND_VARS {
  s8 *mixBufferBase;
  s8 *curMixBuffer;
  u32 mixBufferSize;
  u16 mixFreq;
  u32 mixFreqPeriod;       // 12-bit fixed-point
  s32 samplesUntilMODTick; // 12-bit fixed-point
  u32 samplesPerMODTick;   // 12-bit fixed-point
  u8 channelBlocked;       // One bit per mixer channel
  u8 activeBuffer;

} SOUND_VARS;

// This is the layout of the sample info in ROM
typedef struct _SAMPLE_HEADER {
  u16 length;
  u8 finetune;
  u8 vol;
  u16 loopStart;
  u16 loopLength;

  const s8 *smpData; // Pointer to sample data in ROM

} SAMPLE_HEADER;

// This is the MOD data layout in ROM
typedef struct _MOD_HEADER {
  const SAMPLE_HEADER *sample;
  const u8 *order;
  const u8 **pattern;

  u8 orderCount;
  u8 pad[3];

} MOD_HEADER;

// This is the data we need to keep track of for each channel
// while playing the song, stored in RAM
typedef struct _MOD_CHANNEL {
  u16 period; // Current period of note being played

  u8 sample; // Last sample used on this channel
  u8 vol;    // Current volume

  u8 effect; // Current effect running (set to 0 row tick if no
             // effect/parameter)
  u8 param;  // Current parameter (set to 0 row tick if no effect/parameter)

} MOD_CHANNEL;

// This is all the data for the currently playing song, in RAM
typedef struct _MOD {
  const SAMPLE_HEADER *sample; // Pointer to table of samples in ROM
  const u8 **pattern;          // Pointer to table of pointers to patterns
  const u8 *order;             // Array of pattern numbers to play

  const u8 *rowPtr; // Current position in current pattern, for quick access

  u8 state; // MOD_STATE enum (stopped/playing/paused)

  u8 speed;
  u8 tick; // When this gets to speed, process a new row and reset it to 0

  u8 curRow; // When this gets to 64, move to the next order and reset to 0

  u8 orderCount;
  u8 curOrder; // When this gets to orderCount, stop the song

  u8 tempo; // In BPM (Hz = BPM*2/5)

  u8 pad;

  MOD_CHANNEL channel[SND_MAX_CHANNELS]; // Current state of each channel

} MOD;

// ----- Global vars -----

extern SOUND_CHANNEL sndChannel[SND_MAX_CHANNELS];
extern SOUND_VARS sndVars;
extern MOD sndMod;

// ----- Tables generated by converter (in SndData.c) -----

extern const MOD_HEADER dModTable[];
extern const SAMPLE_HEADER dSfxTable[];

// ----- Global functions -----

extern void SndInit(SND_FREQ freq);
extern void SndVSync() IN_IWRAM; // in Irq.c
extern void SndMix(u32 samplesToMix) IN_IWRAM;
extern void SndUpdate();

extern void SndPlayMOD(u32 modIdx);
extern void SndStopMOD();
extern void SndPauseMOD();
extern void SndUnpauseMOD();
extern u32 SndPlaySFX(u32 sfxIdx);
extern void SndPlaySFXChn(u32 sfxIdx, u32 channel);
extern void SndStopSFX(u32 channel);
extern void SndStopAllSFX();

extern void MODUpdate();

#endif
