// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>
#include <src/menu.h>
#include <src/scenes.h>
#include <src/game/tick.h>
#include <src/game/draw.h>
#include <src/game/load.h>

Vec     tick_boost_vec;            // init in src/game/load.c
Uint32  tick_boost_ttl;            // init in src/game/load.c
Avatar  tick_avatar;               // init in src/game/load.c
Uint16  tick_points;               // init in src/game/load.c
Uint32  tick_difficulty;           // init in src/menu.c
Uint32  tick_energie;              // init in src/game/load.c
Trash   tick_trash[TRASH_SIZE];    // init in src/game/load.c
Score   tick_scores[SCORE_SIZE];   // init in src/game/load.c
Uint32  tick_new_trash_x;          // init in src/game/load.c

Bool warn_player(){
  Sint32 y = vec_get_y_int(tick_avatar.pos);
  return (y < WARNING_HIGH_BORDER or y >= WARNING_LOW_BORDER);
}

void play_player_warning(){
  if (scene_trigger_frame(WARNING_FLASH_DURATION / 2) and warn_player()) {
    snd_reg_c1_sweep = snd_sweep_build(5, SND_SWEEP_DIR_INC, 7);
    synth_play_channel1(SYNTH_NOTE_D, SYNTH_OCTAVE_C3);
  }
}

void play_powerup_sound(){
  snd_reg_c1_sweep = snd_sweep_build(3, SND_SWEEP_DIR_INC, 7);
  synth_play_channel1(SYNTH_NOTE_E, SYNTH_OCTAVE_C3);
}

void play_gg_sound(){
  snd_reg_c1_sweep = snd_sweep_build(3, SND_SWEEP_DIR_DEC, 7);
  synth_play_channel1(SYNTH_NOTE_A, SYNTH_OCTAVE_C3);
}

void update_tick_avatar(){

  // boost when player wants to boost, has boost and not currently boosting
  Bool can_boost = !tick_boost_ttl and tick_energie;
  if(input_key_down(INPUT_KEY_UP) and can_boost){ 
    synth_play_channel4(SYNTH_NOTE_CIS, SYNTH_OCTAVE_C0);
    tick_boost_vec = vec_make_float( 0.00, - BOOST_ACCEL);
    tick_boost_ttl = BOOST_TIME;
    tick_energie--;
  } else if(input_key_down(INPUT_KEY_DOWN) and can_boost){ 
    synth_play_channel4(SYNTH_NOTE_CIS, SYNTH_OCTAVE_C0);
    tick_boost_vec = vec_make_float( 0.00, BOOST_ACCEL);
    tick_boost_ttl = BOOST_TIME;
    tick_energie--;
  } else if(input_key_down(INPUT_KEY_A) and can_boost){ 
    synth_play_channel4(SYNTH_NOTE_AIS, SYNTH_OCTAVE_C0);
    if (tick_energie > 1){
      tick_boost_vec = vec_make_float(BOOST_ACCEL + BOOST_ACCEL, 0.00);
      tick_boost_ttl = BOOST_TIME;
      tick_energie -= 2;
    } else {
      tick_boost_vec = vec_make_float(BOOST_ACCEL, 0.00);
      tick_boost_ttl = BOOST_TIME;
      tick_energie--;
    }
  }

  // move tick_avatar
  if (tick_boost_ttl){
    tick_avatar.vec = vec_add(tick_avatar.vec, tick_boost_vec);
    tick_boost_ttl--;
  } 
  tick_avatar.pos = vec_add(tick_avatar.pos, tick_avatar.vec);

  if (tick_avatar.hit_ttl){
    tick_avatar.hit_ttl--;
  }
}

Uint32 tick_trash_index(Uint32 rand_n, TrashType type){
  // power cell index
  if (type == POWER_CELL){
    return POWERBAR_TILE_INDEX;
  } 
  
  // large trash index
  else if (type == TRASH_BIG) {
    return TRASH_TILE_INDEX + (8 * (rand_n % 4));
  }

  rand_n = rand_n % 16;
  // first small row
  if (rand_n < 8){ 
    return TRASH_TILE_INDEX + 128 + (4 * rand_n);
  } 
  // second small row
  return TRASH_TILE_INDEX + 192 + (4 * (rand_n - 8));
}

Uint32 trash_offset(Uint32 seed){
  return (32 + (seed % 16)) * (1 + tick_difficulty);
}

Uint32 drop_chance(){
  return (tick_difficulty + 1) * (tick_difficulty + 1);
}

void spawn_tick_trash(Trash* t, Obj* o){

  Uint32 rand_a = rand();
  Uint32 rand_b = rand();

  tick_new_trash_x += trash_offset(rand_a);
  Uint32 new_tick_trash_y = rand_b % GAME_H;

  t->pos = vec_make_int(tick_new_trash_x, new_tick_trash_y);
  t->vec = vec_make_int(0,0);
  
  // cheap weighted random choice
  Uint32 tw = rand_a % (4 + 16 + (DIFFICULTY_SIZE - tick_difficulty)); 
  t->type = (tw < 4) ? TRASH_BIG : (tw < 20 ? TRASH_SMALL : POWER_CELL);

  // ok to update tick_trash obj because its off screen before and after
  Sint32 x = game_to_screen_x(vec_get_x_int(t->pos), trash_radius(t));
  Sint32 y = game_to_screen_y(vec_get_y_int(t->pos), trash_radius(t));
  Uint32 index = tick_trash_index(rand_b, t->type);
  Uint32 hflip = rand_a % 2 ? OBJ_A1_HFLIP : 0;
  Uint32 vflip = rand_b % 2 ? OBJ_A1_VFLIP : 0;
  Bool isbig = t->type == TRASH_BIG;
  Uint32 size = isbig ? OBJ_A1_SIZE_S_32x32 : OBJ_A1_SIZE_S_16x16;

  o->attr0 = obj_a0_build(
    y, OBJ_A0_MODE_REG, OBJ_A0_BLEND_NORMAL, 0, 
    OBJ_A0_8BPP, OBJ_A0_SHAPE_SQUARE
  );
  o->attr1 = obj_a1_build_reg(x, hflip, vflip, size);
  o->attr2 = obj_a2_build(index, 1, 0);

  if (obj_offscreen(*o, x, y)){
    *o = obj_hide(*o);
  }
}

void spawn_score(Trash* t, Uint32 index){
  static Uint32 score_index = 0;
  Score* s = &tick_scores[score_index];
  s->ttl = SCORE_TTL;
  Sint32 x = game_to_screen_x(vec_get_x_int(t->pos), trash_radius(t));
  Sint32 y = game_to_screen_y(vec_get_y_int(t->pos), trash_radius(t));
  Vec dest = vec_make_int(192, 0);
  s->pos = vec_make_int(x, y);
  s->vec = vec_div_int(vec_sub(dest, s->pos), SCORE_TTL);
  s->index = index;
  score_index = (score_index + 1) % SCORE_SIZE;
}

void add_tick_points(Trash* t){
  Uint32 factor = trash_radius(t) / 8; // size is 1 or 2
  Uint32 speed = vec_length_sqrd_int(tick_avatar.vec);
  if (speed > 32) speed = 32; // limit to 32
  Sint32 index = relative_align(speed * factor, 0, 64, 0, 15);
  tick_points += 100 * (index + 1);
  spawn_score(t, index);
}

void on_collision_power_cell(Trash* t){
  trash_clear(t);
  tick_energie += POWER_CELL_SIZE;
  if (tick_energie > POWER_MAXIMUM) {
    tick_energie = POWER_MAXIMUM;
  }
  play_powerup_sound();
}

void on_collision_trash(Trash* t, Bool first_hit){
  tick_avatar.hit_ttl = 30;
  Bool will_leave_orbit = fix_neq(vec_get_y(t->vec), fix_zero);
  if (first_hit and will_leave_orbit){
    add_tick_points(t);
  }
  if (first_hit){
    if (trash_radius(t) == 8){
      synth_play_channel2(SYNTH_NOTE_A, SYNTH_OCTAVE_C3);
    } else {
      synth_play_channel2(SYNTH_NOTE_C, SYNTH_OCTAVE_C3);
    }
  }
}

void collision_detect(Trash* t){
  Fix trad = int_to_fix(trash_radius(t)); 
  Fix arad = int_to_fix(AVATAR_RADIUS);
  Fix tmass = (trash_radius(t) == 8 ? float_to_fix(1.0) : float_to_fix(2.0));
  Fix amass = float_to_fix(4.0);
  Bool first_hit = vec_eq(t->vec, vec_zero);
  if (collision_circle_check(t->pos, trad, tick_avatar.pos, arad)){
    if (t->type == POWER_CELL) {
      on_collision_power_cell(t);
    } else {
      collision_circle_bounce(t->pos, &t->vec, trad, tmass, 
                              tick_avatar.pos, &tick_avatar.vec, 
                              arad, amass);
      if (fix_lt(vec_get_x(tick_avatar.vec), fix_one)){
        tick_avatar.vec = vec_set_x(tick_avatar.vec, fix_one);
      }
      on_collision_trash(t, first_hit);
    }
  }
}

void update_tick_trash(){
  for(Uint32 i = 0; i < TRASH_SIZE; i++){
    tick_trash[i].pos = vec_add(tick_trash[i].pos, tick_trash[i].vec);
    collision_detect(&tick_trash[i]);
    Sint32 x = vec_get_x_int(tick_trash[i].pos);
    Sint32 y = vec_get_y_int(tick_trash[i].pos);
    Sint32 rm_left = vec_get_x_int(tick_avatar.pos) - TRASH_REM_LEFT;
    if (x < rm_left or y < TRASH_REM_TOP or y > TRASH_REM_BOTTOM){
      spawn_tick_trash(&tick_trash[i], &TRASH_OBJS[i]);
    }
  }
}

void check_game_over(){
  Bool to_low = vec_get_y_int(tick_avatar.pos) >= GAME_H;
  Bool to_high = vec_get_y_int(tick_avatar.pos) < 0;
  if (to_low or to_high){
    scene_set(&gameover); 
    play_gg_sound();
  }
}

void update_tick_scores(){
  for(Uint32 i = 0; i < SCORE_SIZE; i++){
    Score* s = &tick_scores[i];
    if (s->ttl){
      s->pos = vec_add(s->pos, s->vec);
      s->ttl--;
    }
  }
}

void game_tick(){
  update_tick_avatar();
  update_tick_trash();
  update_tick_scores();
  play_player_warning();
  check_game_over();
}
