// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_SCENE_H
#define GBX_SCENE_H

#define SCENE(loadfunc, tickfunc, drawfunc) { loadfunc, tickfunc, drawfunc }

typedef void  (*SceneLoadFunc)();
typedef void  (*SceneTickFunc)();
typedef void  (*SceneDrawFunc)();

typedef struct {
  SceneLoadFunc loadfunc; // load scene data
  SceneTickFunc tickfunc; // update scene state
  SceneDrawFunc drawfunc; // draw scene state
} Scene;

void scene_set(const Scene* scene); // will be loaded during next vblank
void scene_run(); // main game loop

extern Uint32 scene_frame; // incremented every frame and reset before loading
extern Uint32 scene_total_frames; // incremented every frame

extern Uint32 scene_vcount_draw; // vcount at end of draw 
extern Uint32 scene_vcount_tick; // vcount at end of tick

extern Bool scene_reset_gfx_on_change;
extern Bool scene_reset_snd_on_change;

// TODO maybe change scene_*_frame fps to float for 0.5 fps etc

/**
 * returns true once every @param fps
 */
inline Bool scene_trigger_frame(Uint32 fps){
  return scene_frame % (60 / fps) == 0;
}

/**
 * Toggles at the rate of @param fps.
 */
inline Bool scene_toggle_frame(Uint32 fps){
  return (scene_frame / (60 / fps)) % 2 == 0;
}

inline Uint32 scene_animation_frame(Uint32 fps, Uint32 frames){
  return (scene_frame / (60 / fps)) % frames;
}

#endif
