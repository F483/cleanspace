// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_SYNTH_H
#define GBX_SYNTH_H

#include <src/gbx/gba/snd.h>

#define SYNTH_NOTE_COUNT  12
typedef enum {
  SYNTH_NOTE_C   = 0,  // C
  SYNTH_NOTE_CIS = 1,  // C#
  SYNTH_NOTE_D   = 2,  // D
  SYNTH_NOTE_DIS = 3,  // D#
  SYNTH_NOTE_E   = 4,  // E
  SYNTH_NOTE_F   = 5,  // F
  SYNTH_NOTE_FIS = 6,  // F#
  SYNTH_NOTE_G   = 7,  // G
  SYNTH_NOTE_GIS = 8,  // G#
  SYNTH_NOTE_A   = 9,  // A
  SYNTH_NOTE_AIS = 10, // A#
  SYNTH_NOTE_B   = 11  // B
} SynthNote;

#define SYNTH_OCTAVE_COUNT  10
typedef enum {
  SYNTH_OCTAVE_C0 = 0,
  SYNTH_OCTAVE_C1 = 1,
  SYNTH_OCTAVE_C2 = 2,
  SYNTH_OCTAVE_C3 = 3,
  SYNTH_OCTAVE_C4 = 4, // Middle
  SYNTH_OCTAVE_C5 = 5,
  SYNTH_OCTAVE_C6 = 6,
  SYNTH_OCTAVE_C7 = 7,
  SYNTH_OCTAVE_C8 = 8,
  SYNTH_OCTAVE_C9 = 9
} SynthOctave;

extern const Uint32 synth_note_rate_map[SYNTH_NOTE_COUNT];

inline Uint32 synth_rate(SynthNote note, SynthOctave octave){
  assert(note < SYNTH_NOTE_COUNT);
  assert(octave < SYNTH_OCTAVE_COUNT);
  return 2048 - (synth_note_rate_map[note] >> octave);
}

inline Uint32 synth_freq_to_rate(Uint32 freq){
  return 2048 - 131072 / freq;
}

inline Uint32 synth_rate_to_freq(Uint32 rate){
  return 131072 / (2048 - rate);
}

inline void synth_play_channel1(SynthNote note, SynthOctave octave){
  Uint16 rate = synth_rate(note, octave);
  snd_reg_c1_freq = SND_FREQ_RESET | SND_FREQ_TIMED | rate;
}

inline void synth_play_channel2(SynthNote note, SynthOctave octave){
  Uint16 rate = synth_rate(note, octave);
  snd_reg_c2_freq = SND_FREQ_RESET | SND_FREQ_TIMED | rate;
}

inline void synth_play_channel4(SynthNote note, SynthOctave octave){
  Uint16 rate = synth_rate(note, octave);
  snd_reg_c4_freq = SND_FREQ_RESET | SND_FREQ_TIMED | rate;
}

#endif
