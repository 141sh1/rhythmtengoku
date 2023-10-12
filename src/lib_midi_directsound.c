#include "global.h"
#include "sound.h"
#include "lib_midi.h"

asm(".include \"include/gba.inc\"");//Temporary

extern u32 __udivmoddi4(u64, u64);


/* AUDIO LIBRARY - DIRECTSOUND */


// INTERRUPT_DMA2
void midi_interrupt_dma2(void) {
    volatile u32 dummy;
    u32 temp;
    u32 flag;

    if (!D_030064c4) {
        return;
    }

    flag = FALSE;
    D_030064a0 += 4;

    if (D_030064a0 >= D_03005b24) {
        D_030064a0 -= D_03005b24;
    }

    if (D_030064a0 == D_03005b40) {
        D_030064a0 = (D_030064a0 != 0) ? D_030064a0 - 4 : D_03005b24 - 4;

        temp = D_0300563c[D_030064a0 + 3] >> 24;
        temp |= (temp << 8);
        temp |= (temp << 16);
        D_0300563c[D_030064a0 + 0] = D_0300563c[D_030064a0 + 1] = D_0300563c[D_030064a0 + 2] = D_0300563c[D_030064a0 + 3] = temp;

        temp = D_030064b8[D_030064a0 + 3] >> 24;
        temp |= (temp << 8);
        temp |= (temp << 16);
        D_030064b8[D_030064a0 + 0] = D_030064b8[D_030064a0 + 1] = D_030064b8[D_030064a0 + 2] = D_030064b8[D_030064a0 + 3] = temp;
        flag = TRUE;
    }

    if (D_030064a0 == 0) {
        flag = TRUE;
    }

    if (!flag) {
        return;
    }

    switch (D_030055f4) {
        case DIRECTSOUND_MODE_STEREO:
            REG_DMA1CNT_H = 0;
            REG_DMA2CNT_H = 0;
            REG_DMA1SAD = (u32)&D_0300563c[D_030064a0];
            REG_DMA2SAD = (u32)&D_030064b8[D_030064a0];
            REG_DMA1CNT_H = (DMACNT_DEST_INC_TYPE_INCREMENT | DMACNT_SRC_INC_TYPE_INCREMENT
                                | DMACNT_START_MODE_VBLANK | DMACNT_START_MODE_FIFO_EMPTY
                                | DMACNT_REPEAT | DMACNT_SIZE | DMACNT_ENABLE);
            dummy = 0;
            dummy = 0;
            REG_DMA2CNT_H = (DMACNT_DEST_INC_TYPE_INCREMENT | DMACNT_SRC_INC_TYPE_INCREMENT
                                | DMACNT_START_MODE_VBLANK | DMACNT_START_MODE_FIFO_EMPTY
                                | DMACNT_REPEAT | DMACNT_SIZE | DMACNT_IRQ | DMACNT_ENABLE);
            dummy = 0;
            dummy = 0;
            break;

        case DIRECTSOUND_MODE_MONO1:
            REG_DMA2CNT_H = 0;
            REG_DMA2SAD = (u32)&D_0300563c[D_030064a0];
            REG_DMA2CNT_H = (DMACNT_DEST_INC_TYPE_INCREMENT | DMACNT_SRC_INC_TYPE_INCREMENT
                                | DMACNT_START_MODE_VBLANK | DMACNT_START_MODE_FIFO_EMPTY
                                | DMACNT_REPEAT | DMACNT_SIZE | DMACNT_IRQ | DMACNT_ENABLE);
            dummy = 0;
            dummy = 0;
            break;

        case DIRECTSOUND_MODE_MONO2:
            REG_DMA1CNT_H = 0;
            REG_DMA2CNT_H = 0;
            REG_DMA1SAD = (u32)&D_0300563c[D_030064a0];
            REG_DMA2SAD = (u32)&D_0300563c[D_030064a0];
            REG_DMA1CNT_H = (DMACNT_DEST_INC_TYPE_INCREMENT | DMACNT_SRC_INC_TYPE_INCREMENT
                                | DMACNT_START_MODE_VBLANK | DMACNT_START_MODE_FIFO_EMPTY
                                | DMACNT_REPEAT | DMACNT_SIZE | DMACNT_ENABLE);
            dummy = 0;
            dummy = 0;
            REG_DMA2CNT_H = (DMACNT_DEST_INC_TYPE_INCREMENT | DMACNT_SRC_INC_TYPE_INCREMENT
                                | DMACNT_START_MODE_VBLANK | DMACNT_START_MODE_FIFO_EMPTY
                                | DMACNT_REPEAT | DMACNT_SIZE | DMACNT_IRQ | DMACNT_ENABLE);
            dummy = 0;
            dummy = 0;
            break;
    }
}


// Load Sample
void midi_sampler_load(u32 id, struct SampleData *sample) {
    struct SampleStream *stream = &D_03005b88[id];
    u32 baseKeyFrequency, baseKeySampleRate;
    u64 sampleRate;

    stream->active = FALSE;
    stream->sample = sample->waveform;
    stream->length = sample->length / 4;

    if ((sample->loopStart | sample->loopEnd) != 0) {
        stream->loopStart = sample->loopStart << 14;
        stream->loopEnd = sample->loopEnd << 14;
    } else {
        stream->loopStart = sample->length << 14;
        stream->loopEnd = sample->length << 14;
    }

    baseKeyFrequency = midi_tuning_table[sample->pitch];
    baseKeySampleRate = D_03005b94 * baseKeyFrequency;
    sampleRate = (u64)sample->sampleRate << 28;
    stream->rate = __udivmoddi4(sampleRate + baseKeySampleRate - 1, baseKeySampleRate);
}


// Start Sampler
void midi_sampler_start(u32 id) {
    D_03005b88[id].position = 0;
    D_03005b88[id].active = TRUE;
}


// Stop Sampler
void midi_sampler_stop(u32 id) {
    D_03005b88[id].active = FALSE;
}


// Set Sampler Stereo Bias
void midi_sampler_set_stereo_bias(u32 id, u32 left, u32 right) {
    D_03005b88[id].leftBias = left;
    D_03005b88[id].rightBias = right;
}


// Set Sampler Volume
void midi_sampler_set_volume(u32 id, u32 volume) {
    D_03005b88[id].volume = volume;
}


// Set Sampler Frequency
void midi_sampler_set_frequency(u32 id, u32 frequency) {
    struct SampleStream *stream = &D_03005b88[id];

    if (frequency == 0) {
        stream->frequency = 0x4000;
        stream->hasFrequency = FALSE;
    } else {
        stream->frequency = ((u64)stream->rate * frequency) >> 14;
        stream->hasFrequency = (stream->frequency != 0x4000);
    }
}


// Set Sampler Enable Distortion(?)
void midi_sampler_set_enable_distort(u32 id, u32 enable) {
    D_03005b88[id].distort = enable;
}


// Set Sampler Enable Equalizer
void midi_sampler_set_enable_eq(u32 id, u32 enable) {
    D_03005b88[id].equalize = enable;
}


/* DIRECTSOUND OPERATIONS */


// Initialise DirectSound (https://decomp.me/scratch/ohUd1)
#include "asm/lib_08049144/asm_08049490.s"


// Update DirectSound (https://decomp.me/scratch/jRyYQ)
void midi_directsound_update(void) {
    ThumbFunc fastProcessBaseFreq, fastProcessShiftedFreq, fastProcessDistort;
    ThumbFunc fastProcessEQ;
    u32 noSamplesProcessed;
    struct SampleStream *streams;
    s32 wordsPerFrame, unreadProcessedWords;
    u32 totalWordsToProcess, wordBatchSize;
    u32 sampleBufferPos;
    u32 eqWasUsed;
    u8 eqPosition;
    u16 eqHighGain;
    u32 eqSmoothing;
    u32 i;

    if (!D_030064c4) {
        return;
    }

    fastProcessBaseFreq = ALIGN_THUMB_FUNC(func_08048b9c);
    fastProcessShiftedFreq = ALIGN_THUMB_FUNC(func_080483b8);
    fastProcessDistort = ALIGN_THUMB_FUNC(func_08048d58);
    fastProcessEQ = ALIGN_THUMB_FUNC(func_08048fc0);
    streams = D_03005b88;

    wordsPerFrame = (D_030064a8 + 259) / 4;
    if (wordsPerFrame > D_03005b24) {
        wordsPerFrame = D_03005b24;
    }

    unreadProcessedWords = D_03005b40 - D_030064a0;
    if (unreadProcessedWords < 0) {
        unreadProcessedWords += D_03005b24;
    }

    totalWordsToProcess = (wordsPerFrame > unreadProcessedWords) ? (wordsPerFrame - unreadProcessedWords) : 0;

    eqPosition = D_03005620[0];
    if ((D_030064c0 >= 0) && (eqPosition > 127)) { // High-Pass
        D_03005620[1] = 0;
        D_03005620[2] = 0;
    } else if ((D_030064c0 < 0) && (eqPosition <= 127)) { // Low-Pass
        D_03005620[1] = D_030064b0[D_03005638 - 2];
        D_03005620[2] = D_030064b0[D_03005638 - 1];
    }
    D_030064c0 = eqPosition;

    while (totalWordsToProcess != 0) {
        wordBatchSize = D_03005638 / 4;
        if (wordBatchSize > totalWordsToProcess) {
            wordBatchSize = totalWordsToProcess;
        }

        (ALIGN_THUMB_FUNC(func_08048758))(wordBatchSize);

        noSamplesProcessed = TRUE;
        eqWasUsed = FALSE;
        eqPosition = D_03005620[0];
        eqHighGain = 0;

        for (i = 0; i < D_03005610; i++) {
            eqSmoothing = 256 - eqPosition;

            if (streams[i].active) {
                if (streams[i].equalize || D_03005b44) {
                    noSamplesProcessed = FALSE;
                    eqWasUsed = TRUE;
                    if (eqPosition > 127) {
                        eqHighGain = (streams[i].volume * eqSmoothing * D_03005b28) >> 7;
                    }
                    D_030064ac = eqHighGain;

                    if (!streams[i].hasFrequency) {
                        fastProcessBaseFreq(wordBatchSize, &streams[i]);
                    } else if (streams[i].distort) {
                        fastProcessDistort(wordBatchSize, &streams[i]);
                    } else {
                        fastProcessShiftedFreq(wordBatchSize, &streams[i]);
                    }
                }
            }
        }

        if (eqWasUsed) {
            fastProcessEQ(wordBatchSize, D_03005620);
        }

        D_030064ac = 0;
        for (i = 0; i < D_03005610; i++) {
            if (streams[i].active) {
                if (!streams[i].equalize && !D_03005b44) {
                    noSamplesProcessed = FALSE;

                    if (!streams[i].hasFrequency) {
                        fastProcessBaseFreq(wordBatchSize, &streams[i]);
                    } else if (streams[i].distort) {
                        fastProcessDistort(wordBatchSize, &streams[i]);
                    } else {
                        fastProcessShiftedFreq(wordBatchSize, &streams[i]);
                    }
                }
            }
        }

        D_03005720[0x3FF] = (noSamplesProcessed) ? 0 : -1;
        (ALIGN_THUMB_FUNC(func_08048a00))(wordBatchSize);

        sampleBufferPos = D_03005b40 + wordBatchSize;
        while (sampleBufferPos >= D_03005b24) {
            sampleBufferPos -= D_03005b24;
        }
        D_03005b40 = sampleBufferPos;

        totalWordsToProcess -= wordBatchSize;
        D_03005b84 = REG_VCOUNT;
    }
}


// Flush(?) DirectSound (Unused)
void midi_directsound_flush(void) {
    volatile u32 dummy;

    D_030064c4 = 0;
    if (D_030055f4 != DIRECTSOUND_MODE_MONO1) {
        REG_DMA1CNT = ((DMACNT_ENABLE | DMACNT_SIZE | DMACNT_DEST_INC_TYPE_UNCHANGED) << 16) + (16 / 4);
        dummy = 0;
        dummy = 1;
        REG_DMA1CNT_H = DMACNT_SIZE;
    }
    REG_DMA2CNT = ((DMACNT_ENABLE | DMACNT_SIZE | DMACNT_DEST_INC_TYPE_UNCHANGED) << 16) + (16 / 4);
    dummy = 0;
    dummy = 1;
    REG_DMA2CNT_H = DMACNT_SIZE;
}


// Set Reverb Controllers
void midi_directsound_set_reverb(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    D_030064a4 = arg0;
    D_03005630 = arg1;
    D_03005b48 = arg2;
    D_03005634 = arg3;
}


// Check if Sampler is Active
u32 midi_sampler_is_active(u32 id) {
    return D_03005b88[id].active;
}


// Set Equalizer Position
void midi_equalizer_set_position(u32 position) {
    if (!D_03005b44) {
        D_03005620[0] = position;
    }
}


// Set Equalizer High Gain
void midi_equalizer_set_high_gain(u8 gain) {
    if (!D_03005b44) {
        D_03005b28 = gain;
    }
}


// Initialise Equalizer
void midi_equalizer_init(void) {
    D_03005620[2] = 0;
    D_03005620[1] = 0;
    D_03005620[0] = 0;
    D_030064c0 = 0;
    D_030064b0[D_03005638 - 2] = D_030064b0[D_03005638 - 1] = 0;
}


// Reset Equalizer
void midi_equalizer_reset(void) {
    if (!D_03005b44) {
        midi_equalizer_init();
    }
}


// Set Equalizer
void midi_equalizer_set(u32 enable, u32 position, u32 gain) {
    enable = (enable != FALSE);

    if (D_03005b44 != enable) {
        midi_equalizer_init();
        D_03005b44 = enable;
        D_03005620[0] = position;
        D_03005b28 = gain;
    }
}
