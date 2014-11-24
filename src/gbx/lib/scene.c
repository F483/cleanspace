// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>

Scene* scene_curr = NULL;
Scene* scene_next = NULL;
Uint32 scene_frame = 0; // TODO rename to scene_scene_frames for clarity
Uint32 scene_total_frames = 0;

Uint32 scene_vcount_draw = 0;
Uint32 scene_vcount_tick = 0;

Bool scene_reset_gfx_on_change = true;
Bool scene_reset_snd_on_change = true;

void scene_set(const Scene* scene){
  scene_next = (Scene*)scene; // load in main loop to use full vblank time
}

void scene_change(){
  scene_curr = scene_next;

  if (scene_reset_snd_on_change) {
    snd_reset();
  }
  if (scene_reset_gfx_on_change) {
    gfx_reset();
    pal_reset();
    obj_reset();
  }
  scene_frame = 0;
  scene_vcount_draw = 0;
  scene_vcount_tick = 0;

  // load scene
  if (scene_curr and scene_curr->loadfunc){
    scene_curr->loadfunc();
  }
}

void scene_run(){
  while(scene_curr or scene_next){
    gfx_vsync_busywait(); // TODO use interupts instead

    // change scene if needed
    if (scene_curr != scene_next){
      scene_change();
      continue; // skip frame on change to give drawfunc full vblank time
    }

    // drawing must be done in vblank phase
    if (scene_curr and scene_curr->drawfunc){
      scene_curr->drawfunc();
      scene_vcount_draw = gfx_reg_vcount;
    }

    // updateing state can be done in vdraw phase as well
    input_poll();
    if (scene_curr and scene_curr->tickfunc){
      scene_curr->tickfunc();
      scene_vcount_tick = gfx_reg_vcount;
    }

    scene_frame++;
    scene_total_frames++;
  }
}
