// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>
#include <src/game/tick.h>
#include <src/game/load.h>
#include <src/game/draw.h>

Uint32 draw_planet_bg_x; // init in src/game/load.c
Uint32 draw_planet_bg_y; // init in src/game/load.c
Sint32 draw_fadein_ttl;  // init in src/game/load.c

Sint32 game_to_screen_x(Sint32 game_x, Uint32 radius){
  return game_x - vec_get_x_int(tick_avatar.pos) + 32 - radius;
}

Sint32 game_to_screen_y(Sint32 game_y, Uint32 radius){
  Sint32 tick_avatar_y = vec_get_y_int(tick_avatar.pos);
  Sint32 y = relative_align(tick_avatar_y, 0, GAME_H - 1, AVATAR_RADIUS - 1, 
                            GFX_DISPLAY_HEIGHT - AVATAR_RADIUS);
  return y + game_y - tick_avatar_y - radius;
}

void draw_scores(){
  for (Uint32 i = 0; i < SCORE_SIZE; i++){
    Score* s = &tick_scores[i];
    Obj* o = &SCORE_OBJS[i];
    if (s->ttl){
      Sint32 x = vec_get_x_int(s->pos);
      Sint32 y = vec_get_y_int(s->pos);
      Uint32 i = SCORE_TILE_INDEX + 4 * s->index;
      o->attr0 = obj_a0_build(
          y, OBJ_A0_MODE_REG, OBJ_A0_BLEND_NORMAL, 0, 
          OBJ_A0_8BPP, OBJ_A0_SHAPE_WIDE
      );
      o->attr1 = obj_a1_build_reg(x, 0, 0, OBJ_A1_SIZE_W_16x8);
      o->attr2 = obj_a2_build(i, 1, 0);
    } else {
      *o = obj_hide(*o);
    }
  }
}

void draw_trash(){
  // only updade position, everything else is done when its spawned
  for(Uint32 i = 0; i < TRASH_SIZE; i++){
    Sint32 gx = vec_get_x_int(tick_trash[i].pos);
    Sint32 gy = vec_get_y_int(tick_trash[i].pos);
    Sint32 x = game_to_screen_x(gx, trash_radius(&tick_trash[i]));
    Sint32 y = game_to_screen_y(gy, trash_radius(&tick_trash[i]));
    TRASH_OBJS[i] = obj_update_pos(TRASH_OBJS[i], x, y);
    if (obj_offscreen(TRASH_OBJS[i], x, y)){
      TRASH_OBJS[i] = obj_hide(TRASH_OBJS[i]);
    } else {
      TRASH_OBJS[i] = obj_update_mode(TRASH_OBJS[i], OBJ_A0_MODE_REG);
    }
  }
}

void draw_avatar(){

  Sint32 x = game_to_screen_x(vec_get_x_int(tick_avatar.pos), AVATAR_RADIUS);
  Sint32 y = game_to_screen_y(vec_get_y_int(tick_avatar.pos), AVATAR_RADIUS);

  // tick_avatar
  AVATAR_OBJ.attr0 = obj_a0_build(
      y, OBJ_A0_MODE_REG, OBJ_A0_BLEND_NORMAL, 0, 
      OBJ_A0_8BPP, OBJ_A0_SHAPE_SQUARE
  );
  AVATAR_OBJ.attr1 = obj_a1_build_reg(x, 0, 0, OBJ_A1_SIZE_S_32x32);
  Uint32 frame = scene_animation_frame(8, AVATAR_FRAMES);
  Uint32 frame_offset = 8 * frame;
  Uint32 state_offset = (AVATAR_FRAMES * 8 * (4 * (tick_avatar.hit_ttl ? 1 : 0)));
  Uint16 index = AVATAR_TILE_INDEX + frame_offset + state_offset;
  AVATAR_OBJ.attr2 = obj_a2_build(index, 1, 0);
  
  // boost
  if (tick_boost_ttl) {
    Uint16 vflip = 0;
    Uint16 hflip = 0;
    Uint32 horizontal = 0;
    Sint32 bx = x;
    Sint32 by = y;
    if (vec_get_y_float(tick_boost_vec) > 0.0){ // down
      vflip = OBJ_A1_VFLIP;
      bx += 12;
      by -= 10;
    } else if (vec_get_y_float(tick_boost_vec) < 0.0){ // up
      bx += 12;
      by += 26;
    } else if (vec_get_x_float(tick_boost_vec) > 0.0){ // right
      horizontal = 1;
      bx -= 6;
      by += 8;
    }

    BOOST_OBJ_A.attr0 = obj_a0_build(
        by, OBJ_A0_MODE_REG, OBJ_A0_BLEND_NORMAL, 0, 
        OBJ_A0_8BPP, OBJ_A0_SHAPE_SQUARE
    );
    BOOST_OBJ_A.attr1 = obj_a1_build_reg(
        bx, hflip, vflip, OBJ_A1_SIZE_S_16x16
    );
    frame = scene_animation_frame(8, BOOST_FRAMES);
    frame_offset = 4 * frame;
    state_offset = (BOOST_FRAMES * 4 * horizontal);
    index = BOOST_TILE_INDEX + frame_offset + state_offset;
    BOOST_OBJ_A.attr2 = obj_a2_build(index, 1, 0);

  } else {
    BOOST_OBJ_A = obj_hide(BOOST_OBJ_A);
  }
}

void align_planet_bg(){
  if (scene_frame % 3 == 0){
    draw_planet_bg_x = (draw_planet_bg_x + 1) % PLANETS_BG_W;
  }
  draw_planet_bg_y = relative_align(
      vec_get_y_int(tick_avatar.pos), 
      0, GAME_H - 1, PLANETS_BG_Y_MIN, PLANETS_BG_Y_MAX
  );
  PLANETS_BG_REG_HVEC = draw_planet_bg_x;
  PLANETS_BG_REG_VVEC = draw_planet_bg_y;
}

void fadein(){
  gfx_fade_from_black(draw_fadein_ttl, FADEIN_DURATION);
  draw_fadein_ttl--;
}

void draw_power(){
  // draw from bottom to top
  for (Uint32 i = 0; i < POWER_MAX_CELLS; i++){
    Sint32 ammount = (tick_energie - (i * POWER_CELL_SIZE));
    if (ammount > POWER_CELL_SIZE){
      ammount = POWER_CELL_SIZE;
    }
    if (ammount > 0){
      Sint32 x = 240 - 18;
      Sint32 y = (160 - 18 - i * 17);
      Uint16 index = POWERBAR_TILE_INDEX + 4 * (POWER_CELL_SIZE - ammount); 

      // tick_avatar
      POWERBAR_OBJS[i].attr0 = obj_a0_build(
          y, OBJ_A0_MODE_REG, OBJ_A0_BLEND_NORMAL, 0, 
          OBJ_A0_8BPP, OBJ_A0_SHAPE_SQUARE
      );
      POWERBAR_OBJS[i].attr1 = obj_a1_build_reg(x, 0, 0, OBJ_A1_SIZE_S_16x16);
      POWERBAR_OBJS[i].attr2 = obj_a2_build(index, 1, 0);
    } else {
      POWERBAR_OBJS[i] = obj_hide(POWERBAR_OBJS[i]);
    }
  }
}

void draw_points(){
  font_bg8_print(&game_font, vec_make_int(30 - 5 - 6, 0), "SCORE");
  font_bg8_printu(&game_font, vec_make_int(30 - 5, 0), tick_points);
}

void draw_warning(){
  Bool show = scene_toggle_frame(WARNING_FLASH_DURATION);
  if (show and warn_player()) {
    WARNING_OBJ.attr0 = obj_a0_build(
        WARNING_Y, OBJ_A0_MODE_REG, OBJ_A0_BLEND_NORMAL, 0, 
        OBJ_A0_8BPP, OBJ_A0_SHAPE_WIDE
    );
    WARNING_OBJ.attr1 = obj_a1_build_reg(
        WARNING_X, 0, 0, OBJ_A1_SIZE_W_64x32
    );
    Uint32 index_high = WARNING_HIGH_TILE_INDEX;
    Uint32 index_low = WARNING_LOW_TILE_INDEX;
    Sint32 y = vec_get_y_int(tick_avatar.pos);
    Uint32 index = (y < WARNING_HIGH_BORDER) ? index_high : index_low;
    WARNING_OBJ.attr2 = obj_a2_build(index, 1, 0);
  } else {
    WARNING_OBJ = obj_hide(WARNING_OBJ);
  }
}

void game_draw(){
  fadein();
  align_planet_bg();
  draw_avatar();
  draw_power();
  draw_trash();
  draw_points();
  draw_warning();
  draw_scores();
}

