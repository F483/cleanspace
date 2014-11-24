// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>
#include <src/scores.h>
#include <src/scenes.h>

void init_snd(){
  scene_reset_snd_on_change = false; // settings for all scenes and sounds

  // setup sound
  snd_reg_status = SND_STATUS_ENABLE;
  snd_reg_fm = snd_fm_buildlr(SND_FM_C1 | SND_FM_C2 | SND_FM_C4, 7);
//  snd_reg_ds = SND_DS_VOL100 bitor SND_DS_AVOL100 bitor SND_DS_BVOL100 bitor 
//         SND_DS_AR bitor SND_DS_ARESET bitor 
//         SND_DS_BL bitor SND_DS_BRESET;

  // setup channel 1 (game collect powercell and death)
  snd_reg_c1_freq = 0;
  snd_reg_c1_sweep = 0; // depends on effect
  snd_reg_c1_ctrl = snd_sqr_build(0, SND_SQR_DUTY1_4, 
                  7, SND_SQR_ENV_DIR_DEC, 6);
  // setup channel 2 (menu navigation sounds)
  snd_reg_c2_freq = 0;
  snd_reg_c2_ctrl = snd_sqr_build(0, SND_SQR_DUTY1_4, 
                  1, SND_SQR_ENV_DIR_DEC, 6);
  // setup channel 4 (game avatar boost sounds)
  snd_reg_c4_freq = 0;
  snd_reg_c4_ctrl = snd_sqr_build(0, SND_SQR_DUTY1_2, 
                  7, SND_SQR_ENV_DIR_DEC, 6);
}

int main(){
  init_snd();
  sav_init(0xFABE0001); // init with nonce for cleanspace
  scene_set(&company);
  scene_run();
  return 0;
}
