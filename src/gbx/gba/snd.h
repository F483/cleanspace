// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_SND_H
#define GBX_SND_H

#include <src/gbx/common.h>


////////////////////////////////////////////////////////////////////////////////
//                          SOUND CONTROL REGISTERS                           //
////////////////////////////////////////////////////////////////////////////////

// FM Channel 1: Square wave with sweep
#define snd_reg_c1_sweep    (*(VUint16*)0x04000060) // sweep
#define snd_reg_c1_ctrl     (*(VUint16*)0x04000062) // control
#define snd_reg_c1_freq     (*(VUint16*)0x04000064) // frequency

// FM Channel 2: Simple square wave
#define snd_reg_c2_ctrl     (*(VUint16*)0x04000068) // control
#define snd_reg_c2_freq     (*(VUint16*)0x0400006C) // frequency

// FM Channel 3: Wave player
#define snd_reg_c3_sel      (*(VUint16*)0x04000070) // wave select
#define snd_reg_c3_ctrl     (*(VUint16*)0x04000072) // control
#define snd_reg_c3_freq     (*(VUint16*)0x04000074) // frequency

// FM Channel 4: Noise generator
#define snd_reg_c4_ctrl     (*(VUint16*)0x04000078) // control
#define snd_reg_c4_freq     (*(VUint16*)0x0400007C) // frequency

// Sound setup/control
#define snd_reg_fm          (*(VUint16*)0x04000080) // Frequency modulation
#define snd_reg_ds          (*(VUint16*)0x04000082) // Direct Sound
#define snd_reg_status      (*(VUint16*)0x04000084) // Sound setup
#define snd_reg_bias        (*(VUint16*)0x04000088) // Sound bias      

// Sound buffers
#define snd_reg_wave_ram    ((VUint32*)0x04000090) // Channel 3 wave buffer
#define snd_reg_wave_ram0   (*(VUint32*)0x04000090)            
#define snd_reg_wave_ram1   (*(VUint32*)0x04000094)             
#define snd_reg_wave_ram2   (*(VUint32*)0x04000098)            
#define snd_reg_wave_ram3   (*(VUint32*)0x0400009C)           

#define snd_reg_fifo_a      (*(VUint32*)0x040000A0) // Direct Sound A FIFO
#define snd_reg_fifo_b      (*(VUint32*)0x040000A4) // Direct Sound B FIFO


////////////////////////////////////////////////////////////////////////////////
//                          SOUND STATUS CONTROL                              //
//                                                                            //
// This enables sound and channels.                                           //
////////////////////////////////////////////////////////////////////////////////

#define SND_STATUS_C1       0x0001  // Channel 1 status
#define SND_STATUS_C2       0x0002  // Channel 2 status
#define SND_STATUS_C3       0x0004  // Channel 3 status
#define SND_STATUS_C4       0x0008  // Channel 4 status
#define SND_STATUS_DISABLE  0       // Disable sound
#define SND_STATUS_ENABLE   0x0080  // Enable sound before using anything.


////////////////////////////////////////////////////////////////////////////////
//                          FREQUENCY MODULATION CONTROL                      //
// TODO document bits                                                         //
// This sets volume and channel mapping to L and R speakers.                  //
////////////////////////////////////////////////////////////////////////////////

#define SND_FM_C1_L         0x0100  // Channel 1 L
#define SND_FM_C2_L         0x0200  // Channel 2 L
#define SND_FM_C3_L         0x0400  // Channel 3 L
#define SND_FM_C4_L         0x0800  // Channel 4 L
#define SND_FM_C1_R         0x1000  // Channel 1 R
#define SND_FM_C2_R         0x2000  // Channel 2 R
#define SND_FM_C3_R         0x4000  // Channel 3 R
#define SND_FM_C4_R         0x8000  // Channel 4 R
                            
#define SND_FM_LVOL_MASK    0x0007
#define SND_FM_LVOL_SHIFT   0
#define SND_FM_LVOL(n)      ((n) << SND_FM_LVOL_SHIFT)

#define SND_FM_RVOL_MASK    0x0070
#define SND_FM_RVOL_SHIFT   4
#define SND_FM_RVOL(n)      ((n) << SND_FM_RVOL_SHIFT)

// Unshifted values
#define SND_FM_C1           0x01
#define SND_FM_C2           0x02
#define SND_FM_C3           0x04
#define SND_FM_C4           0x08

inline Uint16 snd_fm_build(Uint16 lmode, Uint16 rmode, 
                           Uint16 lvol, Uint16 rvol){
  return ((rmode) << 12) | ((lmode) << 8) | (((rvol)&7) << 4) | ((lvol)&7);
}

inline Uint16 snd_fm_buildlr(Uint16 mode, Uint16 vol){
  return snd_fm_build(mode, mode, vol, vol);
}


////////////////////////////////////////////////////////////////////////////////
//                      DIRCET SOUND CONTROL                                  //
//                                                                            //
// BITS  NAME           DESCRIPTION                                           //
// 0-1   SND_DS_VOL*    Tone generator volume                                 //
// 2     SND_DS_AVOL*   DMA Sound A Volume (50% | 100%)                       //
// 3     SND_DS_BVOL*   DMA Sound B Volume (50% | 100%)                       //
// 4-7                  Unused                                                //
// 8     SND_DS_AR      Enable Direct Sound A on right                        //
// 9     SND_DS_AL      Enable Direct Sound A on left                         //
// 10    SND_DS_ATMR*   Direct Sound A to use timer 0|1                       //
// 11    SND_DS_ARESET  Reset FIFO of Direct Sound A                          //
// 12    SND_DS_BR      Enable Direct Sound B on right                        //
// 13    SND_DS_BL      Enable Direct Sound B on left                         //
// 14    SND_DS_BTMR*   Direct Sound B to use timer 0|1                       //
// 15    SND_DS_BRESET  Reset FIFO of Direct Sound B                          //
////////////////////////////////////////////////////////////////////////////////

#define SND_DS_VOL_MASK 0x0003
#define SND_DS_VOL25    0x0000  // 25%
#define SND_DS_VOL50    0x0001  // 50%
#define SND_DS_VOL100   0x0002  // 100%
#define SND_DS_VOLXXX   0x0003  // forbidden

#define SND_DS_AVOL50   0x0000  // Direct Sound A at 50% volume
#define SND_DS_AVOL100  0x0004  // Direct Sound A at 100% volume
#define SND_DS_BVOL50   0x0000  // Direct Sound B at 50% volume
#define SND_DS_BVOL100  0x0008  // Direct Sound B at 100% volume
#define SND_DS_AR       0x0100  // Enable Direct Sound A on right
#define SND_DS_AL       0x0200  // Enable Direct Sound A on left
#define SND_DS_ATMR0    0x0000  // Direct Sound A to use timer 0
#define SND_DS_ATMR1    0x0400  // Direct Sound A to use timer 1
#define SND_DS_ARESET   0x0800  // Reset FIFO of Direct Sound A
#define SND_DS_BR       0x1000  // Enable Direct Sound B on right
#define SND_DS_BL       0x2000  // Enable Direct Sound B on left
#define SND_DS_BTMR0    0x0000  // Direct Sound B to use timer 0
#define SND_DS_BTMR1    0x4000  // Direct Sound B to use timer 1
#define SND_DS_BRESET   0x8000  // Reset FIFO of Direct Sound B


////////////////////////////////////////////////////////////////////////////////
//                            SWEEP (Channel 1)                               //
//                                                                            //
// BITS  NAME                 DESCRIPTION                                     //
// 0-2   SND_SWEEP_SHIFT      Sweep Shifts                                    //
// 3     SND_SWEEP_DIR_*      Sweep direction (addition/decrease)             //
// 4-6   SND_SWEEP_TIME       Sweep time                                      //
// 7-15                       Unused                                          //
////////////////////////////////////////////////////////////////////////////////

#define SND_SWEEP_DIR_INC     0x0000  // Increasing sweep rate
#define SND_SWEEP_DIR_DEC     0x0008  // Decreasing sweep rate

#define SND_SWEEP_SHIFT_MASK  0x0007
#define SND_SWEEP_SHIFT_SHIFT 0
#define SND_SWEEP_SHIFT(n)    ((n) << SND_SWEEP_SHIFT_SHIFT)

#define SND_SWEEP_TIME_MASK   0x0070
#define SND_SWEEP_TIME_SHIFT  4
#define SND_SWEEP_TIME(n)     ((n) << SND_SWEEP_TIME_SHIFT)

/**
 * @param shift 3bits 0-7
 * @param dir   1bits SND_SWEEP_DIR_INC | SND_SWEEP_DIR_DEC
 * @param time  3bits 0-7
 */
inline Uint16 snd_sweep_build(Uint16 shift, Uint16 dir, Uint16 time){
  return (shift bitand SND_SWEEP_SHIFT_MASK) bitor 
         (dir bitand SND_SWEEP_DIR_DEC) bitor 
         (SND_SWEEP_TIME(time) bitand SND_SWEEP_TIME_MASK);
}


////////////////////////////////////////////////////////////////////////////////
//                              CONTROL (Channel 1, 2, 4)                     //
//                                                                            //
// BITS  NAME                   DESCRIPTION                                   //
// 0-5   SND_SQR_LEN*           Sound Length                                  //
// 6-7   SND_SQR_DUTY*          Waveform type                                 //
// 8-10  SND_SQR_ENV_STEPS*     Time between envelope changes.                //
// 11  SND_SQR_ENV_DIR_*        Envelope Attenuate/Amplify                    //
// 12-15 SND_SQR_ENV_IVOL*      Envelope Initial Value                        //
////////////////////////////////////////////////////////////////////////////////

#define SND_SQR_LEN_MASK        0x003F
#define SND_SQR_LEN_SHIFT       0
#define SND_SQR_LEN(n)          ((n) << SND_SQR_LEN_SHIFT)

#define SND_SQR_DUTY_MASK       0x00C0
#define SND_SQR_DUTY_SHIFT      6
#define SND_SQR_DUTY(n)         ((n) << SND_SQR_DUTY_SHIFT)
#define SND_SQR_DUTY1_8         0 // 12.5% duty cycle (#-------)
#define SND_SQR_DUTY1_4         1 // 25.0% duty cycle (##------)
#define SND_SQR_DUTY1_2         2 // 50.0% duty cycle (####----)
#define SND_SQR_DUTY3_4         3 // 75.0% duty cycle (######--) == 25%

#define SND_SQR_ENV_STEPS_MASK  0x0700
#define SND_SQR_ENV_STEPS_SHIFT 8
#define SND_SQR_ENV_STEPS(n)    ((n) << SND_SQR_ENV_STEPS_SHIFT)

#define SND_SQR_ENV_DIR_DEC     0    // Increasing volume
#define SND_SQR_ENV_DIR_INC     0x0800 // Decreasing volume

#define SND_SQR_ENV_IVOL_MASK   0xF000
#define SND_SQR_ENV_IVOL_SHIFT  12
#define SND_SQR_ENV_IVOL(n)     ((n) << SND_SQR_ENV_IVOL_SHIFT)

/**
 * @param len    6bit 0-63
 * @param duty   2bit 0-3 (12.5%, 25%, 50%, 75%)
 * @param env_step 3bit 0-7
 * @param env_dir  1bit SND_SQR_ENV_DIR_DEC | SND_SQR_ENV_DIR_INC
 * @param env_ivol 4bit 0-15
 */
inline Uint16 snd_sqr_build(Uint16 len, Uint16 duty, 
              Uint16 env_step, Uint16 env_dir, Uint16 env_ivol){
  return (SND_SQR_ENV_IVOL(env_ivol) bitand SND_SQR_ENV_IVOL_MASK)   bitor
         (env_dir bitand SND_SQR_ENV_DIR_INC)                        bitor 
         (SND_SQR_ENV_STEPS(env_step) bitand SND_SQR_ENV_STEPS_MASK) bitor
         (SND_SQR_DUTY(duty) bitand SND_SQR_DUTY_MASK)               bitor
         (len bitand SND_SQR_LEN_MASK);
}


////////////////////////////////////////////////////////////////////////////////
//                            Frequency (Channel 1, 2, 3)                     //
//                                      *                                     //
// BITS  NAME                 DESCRIPTION                                     //
// 0-10  SND_FREQ_RATE        Frequency                                       //
// 11-13                      Unused                                          //
// 14  SND_FREQ_(HOLD|TIMED)  Timed / Continuous                              //
// 15  SND_FREQ_RESET         Reset / Initialization                          //
////////////////////////////////////////////////////////////////////////////////

#define SND_FREQ_RATE_MASK    0x07FF
#define SND_FREQ_RATE_SHIFT   0
#define SND_FREQ_RATE(n)      ((n) << SND_FREQ_RATE_SHIFT)
#define SND_FREQ_HOLD         0x0000  // Continuous play
#define SND_FREQ_TIMED        0x4000  // Timed play
#define SND_FREQ_RESET        0x8000  // Reset sound

/**
 * @param rate  11bits 0-4095
 * @param timed SND_FREQ_TIMED | SND_FREQ_HOLD
 * @param reset SND_FREQ_RESET | 0
 */
inline Uint16 snd_freq_build(Uint16 rate, Uint16 timed, Uint16 reset){
  return (rate bitand SND_FREQ_RATE_MASK) bitor 
         (timed bitand SND_FREQ_TIMED) bitor 
         (reset bitand SND_FREQ_RESET);
}


//////////////////////
// HELPER FUNCTIONS //
//////////////////////

inline void snd_reset(){
  snd_reg_c1_sweep  = 0;
  snd_reg_c1_ctrl   = 0;
  snd_reg_c1_freq   = 0;
  snd_reg_c2_ctrl   = 0;
  snd_reg_c2_freq   = 0;
  snd_reg_c3_sel    = 0;
  snd_reg_c3_ctrl   = 0;
  snd_reg_c3_freq   = 0;
  snd_reg_c4_ctrl   = 0;
  snd_reg_c4_freq   = 0;
  snd_reg_fm        = 0;
  snd_reg_ds        = 0;
  snd_reg_status    = 0;
  snd_reg_bias      = 0;
  // TODO clear buffer? snd_reg_wave_ram
  snd_reg_wave_ram0 = 0;
  snd_reg_wave_ram1 = 0;
  snd_reg_wave_ram2 = 0;
  snd_reg_wave_ram3 = 0;
  snd_reg_fifo_a    = 0;
  snd_reg_fifo_b    = 0;
}

#endif
