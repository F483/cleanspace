// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>
#include <gfx/font8.h>
#include <gfx/bg_gameover.h>
#include <src/scenes.h>
#include <src/game/tick.h>
#include <src/menu.h>
#include <src/scores.h>

// background setup
#define BG_FLAG           GFX_DISPLAY_BG0
#define BG_REG            gfx_reg_bg0
#define BG_REG_H          gfx_reg_bg0_hpos
#define BG_REG_V          gfx_reg_bg0_vpos
#define BG_TILEBANK       0
#define BG_TILEBANK_MEM   TILE_BANK_MEM_8BPP(BG_TILEBANK)
#define BG_MAPBANK        31
#define BG_MAPBANK_MEM    MAP_BANK_MEM(BG_MAPBANK)
#define BG_MAPSIZE        GFX_BG_REG_32x32
#define BG_BPP            GFX_BG_8BPP
#define BG_PRIO           GFX_BG_PRIO(3)

// font setup
#define FONT_BG_FLAG      GFX_DISPLAY_BG3
#define FONT_BG_REG       3
#define FONT_BG_REG_H     gfx_reg_bg3_hpos
#define FONT_BG_REG_V     gfx_reg_bg3_vpos
#define FONT_TILEBANK     0
#define FONT_OFFSET       (bg_gameover_TilesLen / (8 * 8))
#define FONT_MAPBANK      30

// delays times
#define RESULT_DELAY_TTL  (60 * 5)

#define CONFIRM_KEYS      (INPUT_KEY_START bitor INPUT_KEY_A)

Uint16 bg_pos_y = 0;
Uint32 result_delay_ttl = 0;

Uint32 new_rank = 0;
Uint32 name_index = 0;
char   name_data[HIGHSCORE_NAME_SIZE + 1];

#define CHARS_SIZE      37
char*   chars_valid = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
Uint32  chars_index = 0;

static const FontBG8 font = FONTBG8(FONT_TILEBANK, FONT_OFFSET, 
                  FONT_MAPBANK, FONT_BG_REG, 0);

Uint32 chars_index_of(char c){
  // FIXME lol slow
  Uint32 index = 0;
  while(chars_valid[index] != c and index < CHARS_SIZE){
    index++;
  }
  return index;
}

void gameover_draw(){

  // fade in
  gfx_fade_from_black(59 - scene_frame, 60);

  // scroll screen
  Bool to_low = (bg_pos_y + 1 + GFX_DISPLAY_HEIGHT) > 256;
  if (!to_low and (scene_frame % 3 == 0)){
    BG_REG_V = bg_pos_y = (bg_pos_y + 1);
  }

  // TODO use sprite instead of font
  font_bg8_print(&font, vec_make_int(13,3), "GAME\nOVER"); 

  // score
  if (!result_delay_ttl){
    font_bg8_print(&font, vec_make_int(7, 7), "DIFFICULTY"); 
    font_bg8_print(&font, vec_make_int(18, 7), DM_NAMES[tick_difficulty]);
    font_bg8_printf(&font, vec_make_int(12, 8), "SCORE %i", tick_points); 
    if (new_rank){
      font_bg8_printf(&font, vec_make_int(13, 9), "RANK %i", new_rank); 
      font_bg8_print(&font, vec_make_int(10, 11), "ENTER NAME"); 
      font_bg8_print(&font, vec_make_int(9, 13), name_data); 
      char* curser = scene_toggle_frame(4) ? " ^ " : "   ";
      font_bg8_print(&font, vec_make_int(8 + name_index, 14), curser); 
    } 
  }
}

void gameover_tick(){
  // text delay while scrolling down
  if (result_delay_ttl){
    if (input_key_hit(CONFIRM_KEYS)){
      result_delay_ttl = 0; // display right away
    } else {
      result_delay_ttl--;
    }

  // save and go back to main menu
  } else if (input_key_hit(INPUT_KEY_START)){
    synth_play_channel2(SYNTH_NOTE_A, SYNTH_OCTAVE_C5);
    if (new_rank){
      score_save(tick_difficulty, tick_points, name_data);
    }
    scene_set(&menu);

  // enter highscore name if new_rank
  } else if (new_rank){
    if (input_key_hit(INPUT_KEY_UP)){
      synth_play_channel2(SYNTH_NOTE_E, SYNTH_OCTAVE_C5);
      chars_index = menu_index_next(CHARS_SIZE, chars_index);
      name_data[name_index] = chars_valid[chars_index];
    } else if (input_key_hit(INPUT_KEY_DOWN)){
      synth_play_channel2(SYNTH_NOTE_E, SYNTH_OCTAVE_C5);
      chars_index = menu_index_prev(CHARS_SIZE, chars_index);
      name_data[name_index] = chars_valid[chars_index];
    } else if (input_key_hit(INPUT_KEY_LEFT bitor INPUT_KEY_B)){
      synth_play_channel2(SYNTH_NOTE_E, SYNTH_OCTAVE_C5);
      name_index = menu_index_prev(HIGHSCORE_NAME_SIZE, name_index);
      chars_index = chars_index_of(name_data[name_index]);
    } else if (input_key_hit(INPUT_KEY_RIGHT bitor INPUT_KEY_A)){
      synth_play_channel2(SYNTH_NOTE_E, SYNTH_OCTAVE_C5);
      name_index = menu_index_next(HIGHSCORE_NAME_SIZE, name_index);
      chars_index = chars_index_of(name_data[name_index]);
    }
  }
}

Uint32 get_default_name_index(){
  for(Uint32 i = 0; i < HIGHSCORE_NAME_SIZE; i++){
    if (name_data[i] == ' '){
      return i;
    }
  }
  return HIGHSCORE_NAME_SIZE - 1;
}

void load_name(){
  score_read_last_name(name_data);
  name_index = get_default_name_index();
  chars_index = chars_index_of(name_data[name_index]);
}

void gameover_load(){

  // renderer
  gfx_reg_display = GFX_DISPLAY_MODE0 | BG_FLAG | FONT_BG_FLAG;
  BG_REG = GFX_BG_TILE_MEM(BG_TILEBANK) | GFX_BG_MAP_MEM(BG_MAPBANK) | 
       BG_MAPSIZE | BG_BPP | BG_PRIO;
  BG_REG_H = 0;
  BG_REG_V = 0;

  // copy tiles, map and palette
  memcpy(BG_TILEBANK_MEM, &bg_gameover_Tiles, 
       bg_gameover_TilesLen);
  memcpy(BG_MAPBANK_MEM, &bg_gameover_Map, 
       bg_gameover_MapLen);

  // font
  font_bg8_init(&font);
  font_bg8_cptiles(&font, (Tile8*)font8_Tiles);

  // reset vars
  result_delay_ttl = RESULT_DELAY_TTL;
  bg_pos_y = 0;
  Uint32 entries = score_entries_gte(tick_difficulty, tick_points);
  new_rank = entries >= HIGHSCORE_LIST_SIZE ? 0 : (entries + 1);

  if (new_rank){
    load_name();
  }

  pal_set_bg(bg_gameover_Pal);
}

const Scene gameover = SCENE(&gameover_load, &gameover_tick, &gameover_draw);
