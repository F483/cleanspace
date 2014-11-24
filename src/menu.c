// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>
#include <gfx/font8.h>
#include <gfx/bg_menu.h>
#include <gfx/obj_menu.h>
#include <src/scenes.h>
#include <src/menu.h>
#include <src/scores.h>
#include <src/game/tick.h>

// background setup
#define BG_FLAG             GFX_DISPLAY_BG0
#define BG_REG              gfx_reg_bg0
#define BG_REG_H            gfx_reg_bg0_hpos
#define BG_REG_V            gfx_reg_bg0_vpos
#define BG_TILEBANK         0
#define BG_TILEBANK_MEM     TILE_BANK_MEM_8BPP(BG_TILEBANK)
#define BG_MAPBANK          30
#define BG_MAPBANK_MEM      MAP_BANK_MEM(BG_MAPBANK)
#define BG_MAPSIZE          GFX_BG_REG_64x32
#define BG_BPP              GFX_BG_8BPP
#define BG_PRIO             GFX_BG_PRIO(3)

#define BG_X_MIN            0
#define BG_X_MAX            (512 - GFX_DISPLAY_WIDTH)

// font setup
#define FONT_BG_FLAG        GFX_DISPLAY_BG3
#define FONT_BG_REG         3
#define FONT_BG_REG_H       gfx_reg_bg3_hpos
#define FONT_BG_REG_V       gfx_reg_bg3_vpos
#define FONT_TILEBANK       0
#define FONT_OFFSET         (bg_menu_TilesLen / (8 * 8))
#define FONT_MAPBANK        29

// logo setup
#define LOGO_OBJ_L          OBJ_MEM[0] 
#define LOGO_OBJ_M          OBJ_MEM[1] 
#define LOGO_OBJ_R          OBJ_MEM[2] 
#define LOGO_INDEX_L        ((16 *  8 + 8 ) * 2)
#define LOGO_INDEX_M        ((16 * 12 + 8 ) * 2)
#define LOGO_INDEX_R        ((16 * 16 + 8 ) * 2)

// menu setup
#define MAINMENU_OBJ        OBJ_MEM[3] 
#define MAINMENU_INDEX      0

// difficulty setup
#define DIFFICULTY_OBJ_T    OBJ_MEM[4] 
#define DIFFICULTY_OBJ_L    OBJ_MEM[5] 
#define DIFFICULTY_OBJ_R    OBJ_MEM[6] 
#define DIFFICULTY_T_INDEX  (8 * 2)
#define DIFFICULTY_L_INDEX  ((16 * 4 +  8) * 2)
#define DIFFICULTY_R_INDEX  ((16 * 4 + 12) * 2)

// tipjar_setup
#define TIPJAR_OBJ_0        OBJ_MEM[7]  // top left  64x64
#define TIPJAR_OBJ_1        OBJ_MEM[8]  // top right 32x64
#define TIPJAR_OBJ_2        OBJ_MEM[9]  // bottom left 64x32
#define TIPJAR_OBJ_3        OBJ_MEM[10] // bottom right 32x32
#define TIPJAR_INDEX_0      ((16 * 20 +  0) * 2) // top left  64x64
#define TIPJAR_INDEX_1      ((16 * 20 +  8) * 2) // top right 32x64
#define TIPJAR_INDEX_2      ((16 * 28 +  0) * 2) // bottom left 64x32
#define TIPJAR_INDEX_3      ((16 * 28 +  8) * 2) // bottom right 32x32
#define QRCODE_OBJ_0        OBJ_MEM[11] // top left  64x64
#define QRCODE_OBJ_1        OBJ_MEM[12] // top right 32x64
#define QRCODE_OBJ_2        OBJ_MEM[13] // bottom left 64x32
#define QRCODE_OBJ_3        OBJ_MEM[14] // bottom right 32x32
#define QRCODE_INDEX_0      ((16 *  8 +  0) * 2) // top left  64x64
#define QRCODE_INDEX_1      ((16 * 20 + 12) * 2) // top right 32x64
#define QRCODE_INDEX_2      ((16 * 16 +  0) * 2) // bottom left 64x32
#define QRCODE_INDEX_3      ((16 * 28 + 12) * 2) // bottom right 32x32

// selected
#define SELECTED_OBJ        OBJ_MEM[15]
#define SELECTED_INDEX      ((16 * 6 + 8) * 2)
#define SELECTED_FRAMES     4

Bool text_cleared;
const FontBG8 font = FONTBG8(FONT_TILEBANK, FONT_OFFSET, 
                             FONT_MAPBANK, FONT_BG_REG, 0);


//////////////////////////////////
// MENU TRANSITIONS AND SCREENS //
//////////////////////////////////

#define TRANSITION_TIME     60

const Scene* next_scene = NULL;

typedef enum {
  TS_OPEN,
  TS_RIGHT,
  TS_LEFT,
  TS_CLOSE,
  TS_DONE,
} Transition;

Transition transition_state;
Uint32   transition_ttl;

typedef enum {
  SCR_NONE     = 0, // closing
  SCR_MAIN     = 1,
  SCR_DIFFICULTY = 2,
  SCR_SCORES   = 3,
  SCR_TIPJAR   = 4,
  SCR_CREDITS  = 5
} Screen;

Screen curr_screen;
Screen prev_screen;

void trigger_transition(Screen screen){
  prev_screen = curr_screen;
  curr_screen = screen;
  switch(curr_screen){
    case SCR_NONE:
      transition_state = TS_CLOSE;
      break;
    case SCR_MAIN:
      transition_state = TS_LEFT;
      break;
    default:
      transition_state = TS_RIGHT;
  }
  transition_ttl = TRANSITION_TIME;
  font_bg8_clear(&font);
  text_cleared = true;
}


///////////////
// Main Menu //
///////////////

#define MAINMENU_SIZE 4

typedef enum { 
  MM_PLAY     = 0, 
  MM_SCORES     = 1, 
  MM_TIPJAR     = 2,
  MM_CREDITS    = 3
} MainMenuEntrie;

MainMenuEntrie mm_selected = MM_PLAY;


///////////////////////
// Difficulty Screen //
///////////////////////

typedef enum { 
  DM_EASY   = 0, 
  DM_MEDIUM   = 1, 
  DM_HARD   = 2
} DifficultyMenuEntrie;

DifficultyMenuEntrie dm_selected = DM_MEDIUM;
DifficultyMenuEntrie sm_selected = DM_MEDIUM;

// names for gameover and scores
const char*  DM_NAMES[DIFFICULTY_SIZE] = {
  "EASY",
  "MEDIUM",
  "HARD"
}; 


////////////////////
// Credits Screen //
////////////////////

const char* CREDITS = 
"      CREDITS\n"
"\n"
" FABIAN BARKHAU GAME DESIGN\n"
"        PROGRAMMING\n"
"        SOUND\n"
"BURCAG YILDIRIM GRAPHICS\n"
"\n"
"\n"
"    SPECIAL THANKS\n"
"\n"
"      ZID TESTING\n"
"       TODO MUSIC\n"
"  JASPER VIJN TONC & USENTI\n"
;


/////////////////////
// Load Functions. //
/////////////////////

Vec logo_open;
Vec logo_left;
Vec logo_right;
Vec logo_close;
Vec logo_pos;

Vec bg_open;
Vec bg_left;
Vec bg_right;
Vec bg_close;
Vec bg_pos;

Vec mainmenu_open;
Vec mainmenu_left;
Vec mainmenu_right;
Vec mainmenu_close;
Vec mainmenu_pos;

Vec difficulty_open;
Vec difficulty_left;
Vec difficulty_right;
Vec difficulty_close;
Vec difficulty_pos;

Vec tipjar_open;
Vec tipjar_left;
Vec tipjar_right;
Vec tipjar_close;
Vec tipjar_pos;

void set_vecs(){
  logo_open = vec_make_int(24, -32);
  logo_left = vec_make_int(24, 8);
  logo_right = vec_make_int(24, 8);
  logo_close = vec_make_int(24, -32);
  logo_pos = logo_open;

  bg_open = vec_make_int(128, 0);
  bg_left = vec_make_int(0, 0);
  bg_right = vec_make_int(128, 0);
  bg_close = vec_make_int(256, 0);
  bg_pos = bg_open;

  mainmenu_open = vec_make_int(144, 160);
  mainmenu_left = vec_make_int(144, 48);
  mainmenu_right = vec_make_int(-96, 48);
  mainmenu_close = vec_make_int(-96, 48);
  mainmenu_pos = mainmenu_open;

  difficulty_open = vec_make_int(272, 56);
  difficulty_left = vec_make_int(272, 56);
  difficulty_right = vec_make_int(32, 56);
  difficulty_close = vec_make_int(-104, 56);
  difficulty_pos = difficulty_open;

  tipjar_open = vec_make_int(256, 52);
  tipjar_left = vec_make_int(256, 52);
  tipjar_right = vec_make_int(16, 52);
  tipjar_close = vec_make_int(-112, 52);
  tipjar_pos = tipjar_open;
}

void menu_load(){

  // renderer
  gfx_reg_display = GFX_DISPLAY_MODE0 | BG_FLAG | FONT_BG_FLAG |
            GFX_DISPLAY_OBJ | GFX_DISPLAY_OBJ_2D;
  BG_REG = GFX_BG_TILE_MEM(BG_TILEBANK) | GFX_BG_MAP_MEM(BG_MAPBANK) | 
       BG_MAPSIZE | BG_BPP | BG_PRIO;

  // copy vram data
  memcpy(BG_TILEBANK_MEM, &bg_menu_Tiles, bg_menu_TilesLen);
  memcpy(TILE_OBJ_MEM_8BPP, &obj_menu_Tiles, obj_menu_TilesLen);
  memcpy(BG_MAPBANK_MEM, &bg_menu_Map, bg_menu_MapLen);

  // font
  font_bg8_init(&font);
  font_bg8_cptiles(&font, (Tile8*)font8_Tiles);

  set_vecs();

  // init menu
  curr_screen = SCR_MAIN;
  prev_screen = curr_screen;
  transition_ttl = TRANSITION_TIME;
  transition_state = TS_OPEN;
  text_cleared = true;

  pal_set(bg_menu_Pal, obj_menu_Pal); // copy palettes
}


////////////////////
// Draw Functions //
////////////////////

void draw_selected(){ 
  if (transition_state != TS_DONE){
    SELECTED_OBJ = obj_hide(SELECTED_OBJ);
    return;
  }

  Sint32 x = 0;
  Sint32 y = 0;
  Uint16 hflip = 0;
  Uint32 frame = scene_animation_frame(8, SELECTED_FRAMES);
  Uint32 index = SELECTED_INDEX + frame * 4;
  switch(curr_screen){
    case SCR_MAIN:
      x = 216;
      y = 48 +  mm_selected * 16;
      break;
    case SCR_DIFFICULTY:
      x = 8;
      y = 56 +  dm_selected * 16;
      hflip = OBJ_A1_HFLIP;
      break;
    case SCR_SCORES:
      x = 8;
      y = 60;
      hflip = OBJ_A1_HFLIP;
      break;
    default:
      SELECTED_OBJ = obj_hide(SELECTED_OBJ);
      return;
  }
  SELECTED_OBJ = obj_build_16x16_8bpp(x, y, index);
  SELECTED_OBJ = obj_update_hflip(SELECTED_OBJ, hflip);
}

void print_credits_screen(){
  font_bg8_print(&font, vec_make_int(0, 6), CREDITS);
}

void print_scores_screen(){
  font_bg8_print(&font, vec_make_int(10, 6), "HIGHSCORES");
  font_bg8_print(&font, vec_make_int(4, 8), "DIFFICULTY:");
  font_bg8_print(&font, vec_make_int(16, 8), DM_NAMES[sm_selected]);
  Uint16 points = 0;
  char name[HIGHSCORE_NAME_SIZE + 1];
  for (Uint32 i = 0; i < HIGHSCORE_LIST_SIZE; i++){
    score_read(sm_selected, i, &points, name);
    font_bg8_printf(&font, vec_make_int(4, 10 + i), "%i.", i + 1);
    font_bg8_printu(&font, vec_make_int(7, 10 + i), points);
    font_bg8_print(&font, vec_make_int(13, 10 + i), name);
  }
}

void draw_difficulty_objs(){
  if (curr_screen == SCR_DIFFICULTY or prev_screen == SCR_DIFFICULTY){
    Sint32 y = vec_get_y_int(difficulty_pos);
    Sint32 x = vec_get_x_int(difficulty_pos);
    DIFFICULTY_OBJ_T = obj_build_64x32_8bpp(x +  0, y +  0, 
                                            DIFFICULTY_T_INDEX);
    DIFFICULTY_OBJ_L = obj_build_32x16_8bpp(x +  0, y + 32, 
                                            DIFFICULTY_L_INDEX);
    DIFFICULTY_OBJ_R = obj_build_32x16_8bpp(x + 32, y + 32, 
                                            DIFFICULTY_R_INDEX);
  } else {
    DIFFICULTY_OBJ_T = obj_hide(DIFFICULTY_OBJ_T);
    DIFFICULTY_OBJ_L = obj_hide(DIFFICULTY_OBJ_L);
    DIFFICULTY_OBJ_R = obj_hide(DIFFICULTY_OBJ_R);
  }
}

void draw_tipjar_objs(){
  if (curr_screen == SCR_TIPJAR or prev_screen == SCR_TIPJAR){
    // tipjar
    Sint32 y = vec_get_y_int(tipjar_pos);
    Sint32 x = vec_get_x_int(tipjar_pos);
    TIPJAR_OBJ_0 = obj_build_64x64_8bpp(x +  0, y +  0, TIPJAR_INDEX_0);
    TIPJAR_OBJ_1 = obj_build_32x64_8bpp(x + 64, y +  0, TIPJAR_INDEX_1);
    TIPJAR_OBJ_2 = obj_build_64x32_8bpp(x +  0, y + 64, TIPJAR_INDEX_2);
    TIPJAR_OBJ_3 = obj_build_32x32_8bpp(x + 64, y + 64, TIPJAR_INDEX_3);

    // qrcode
    x += 112;
    QRCODE_OBJ_0 = obj_build_64x64_8bpp(x +  0, y +  0, QRCODE_INDEX_0);
    QRCODE_OBJ_1 = obj_build_32x64_8bpp(x + 64, y +  0, QRCODE_INDEX_1);
    QRCODE_OBJ_2 = obj_build_64x32_8bpp(x +  0, y + 64, QRCODE_INDEX_2);
    QRCODE_OBJ_3 = obj_build_32x32_8bpp(x + 64, y + 64, QRCODE_INDEX_3);
  } else {
    // tipjar
    TIPJAR_OBJ_0 = obj_hide(TIPJAR_OBJ_0);
    TIPJAR_OBJ_1 = obj_hide(TIPJAR_OBJ_1);
    TIPJAR_OBJ_2 = obj_hide(TIPJAR_OBJ_2);
    TIPJAR_OBJ_3 = obj_hide(TIPJAR_OBJ_3);

    // qrcode
    QRCODE_OBJ_0 = obj_hide(QRCODE_OBJ_0);
    QRCODE_OBJ_1 = obj_hide(QRCODE_OBJ_1);
    QRCODE_OBJ_2 = obj_hide(QRCODE_OBJ_2);
    QRCODE_OBJ_3 = obj_hide(QRCODE_OBJ_3);
  }
}

void draw_objs(){
  Sint32 y;
  Sint32 x;

  // logo
  y = vec_get_y_int(logo_pos);
  x = vec_get_x_int(logo_pos);
  LOGO_OBJ_L = obj_build_64x32_8bpp(x +   0, y, LOGO_INDEX_L);
  LOGO_OBJ_M = obj_build_64x32_8bpp(x +  64, y, LOGO_INDEX_M);
  LOGO_OBJ_R = obj_build_64x32_8bpp(x + 128, y, LOGO_INDEX_R);

  // main menu
  y = vec_get_y_int(mainmenu_pos);
  x = vec_get_x_int(mainmenu_pos);
  MAINMENU_OBJ = obj_build_64x64_8bpp(x, y, MAINMENU_INDEX);

}

void print_screen(){
  // draw text if transition finished
  if (transition_state == TS_DONE and text_cleared){
    switch(curr_screen){
      case SCR_SCORES:
        print_scores_screen();
        break;
      case SCR_CREDITS:
        print_credits_screen();
        break;
      case SCR_TIPJAR:    // nothing to draw, using objects
      case SCR_DIFFICULTY:  // nothing to draw, using objects
      case SCR_MAIN:      // nothing to draw, using objects
      case SCR_NONE:      // nothing to draw, blend out done
        break; 
    }
    text_cleared = false;
  }
}

void draw_bg(){
  BG_REG_H = vec_get_x_int(bg_pos);
  BG_REG_V = vec_get_y_int(bg_pos);
}

void menu_fadein(){
  if (transition_state != TS_CLOSE){
    gfx_fade_from_black(TRANSITION_TIME - scene_frame - 1, TRANSITION_TIME);
  } 
}

void menu_fadeout(){
  if (transition_state == TS_CLOSE) {
    gfx_fade_into_black(transition_ttl - 1, TRANSITION_TIME);
  }
}

void menu_draw(){
  menu_fadein();
  menu_fadeout();
  draw_bg();
  draw_objs();
  draw_difficulty_objs();
  draw_tipjar_objs();
  draw_selected();
  print_screen();
}


////////////////////
// TICK FUNCTIONS //
////////////////////

void play_snd_select(){
  synth_play_channel2(SYNTH_NOTE_A, SYNTH_OCTAVE_C5);
}

void play_snd_navigate(){
  synth_play_channel2(SYNTH_NOTE_E, SYNTH_OCTAVE_C5);
}

void play_snd_back(){
  synth_play_channel2(SYNTH_NOTE_C, SYNTH_OCTAVE_C5);
}

void transition_tick(){
  // move elements
  Uint32 ttl = transition_ttl;
  switch(transition_state){
    case TS_OPEN:
      logo_pos = vec_approach(ttl, logo_pos, logo_left);
      bg_pos = vec_approach(ttl, bg_pos, bg_left);
      mainmenu_pos = vec_approach(ttl, mainmenu_pos, mainmenu_left);
      difficulty_pos = vec_approach(ttl, difficulty_pos, difficulty_left);
      tipjar_pos = vec_approach(ttl, tipjar_pos, tipjar_left);
      break;
    case TS_CLOSE:
      logo_pos = vec_approach(ttl, logo_pos, logo_close);
      bg_pos = vec_approach(ttl, bg_pos, bg_close);
      mainmenu_pos = vec_approach(ttl, mainmenu_pos, mainmenu_close);
      difficulty_pos = vec_approach(ttl, difficulty_pos, difficulty_close);
      tipjar_pos = vec_approach(ttl, tipjar_pos, tipjar_close);
      break;
    case TS_LEFT:
      logo_pos = vec_approach(ttl, logo_pos, logo_left);
      bg_pos = vec_approach(ttl, bg_pos, bg_left);
      mainmenu_pos = vec_approach(ttl, mainmenu_pos, mainmenu_left);
      difficulty_pos = vec_approach(ttl, difficulty_pos, difficulty_left);
      tipjar_pos = vec_approach(ttl, tipjar_pos, tipjar_left);
      break;
    case TS_RIGHT:
      logo_pos = vec_approach(ttl, logo_pos, logo_right);
      bg_pos = vec_approach(ttl, bg_pos, bg_close);
      mainmenu_pos = vec_approach(ttl, mainmenu_pos, mainmenu_right);
      difficulty_pos = vec_approach(ttl, difficulty_pos, difficulty_right);
      tipjar_pos = vec_approach(ttl, tipjar_pos, tipjar_right);
      break;
    case TS_DONE:
      break; // do nothing
  }

  // end transition
  if (transition_ttl == 0){
    transition_state = TS_DONE;
  }

  // dec transition
  if (transition_ttl){
    transition_ttl--;
  }
}

void score_screen_tick(){
  if (input_key_hit(INPUT_KEY_A bitor INPUT_KEY_START bitor INPUT_KEY_RIGHT)){
    play_snd_navigate();
    sm_selected = (sm_selected + 1) % DIFFICULTY_SIZE;
    font_bg8_clear(&font);
    text_cleared = true;
  } else if (input_key_hit(INPUT_KEY_LEFT)) {
    play_snd_navigate();
    sm_selected = (sm_selected + DIFFICULTY_SIZE - 1) % DIFFICULTY_SIZE;
    font_bg8_clear(&font);
    text_cleared = true;
  } else if (input_key_hit(INPUT_KEY_B)){
    play_snd_back();
    trigger_transition(SCR_MAIN);
  }
}

void info_screen_tick(){
  // only go back to main screen
  if (input_key_hit(INPUT_KEY_B)){
    play_snd_back();
    trigger_transition(SCR_MAIN);
  }
}

void difficulty_screen_tick(){
  // move selected
  if (input_key_hit(INPUT_KEY_UP)){
    play_snd_navigate();
    dm_selected = (dm_selected + DIFFICULTY_SIZE - 1) % DIFFICULTY_SIZE;
  } else if (input_key_hit(INPUT_KEY_DOWN)){
    play_snd_navigate();
    dm_selected = (dm_selected + 1) % DIFFICULTY_SIZE;
  }

  // handle user input
  if (input_key_hit(INPUT_KEY_A | INPUT_KEY_START)){
    play_snd_select();
    next_scene = &game;
    tick_difficulty = dm_selected;
    trigger_transition(SCR_NONE);
  } else if (input_key_hit(INPUT_KEY_B)){
    play_snd_back();
    trigger_transition(SCR_MAIN);
  }
}

void main_screen_tick(){
  // move selected
  if (input_key_hit(INPUT_KEY_UP)){
    play_snd_navigate();
    mm_selected = (mm_selected + MAINMENU_SIZE - 1) % MAINMENU_SIZE;
  } else if (input_key_hit(INPUT_KEY_DOWN)){
    play_snd_navigate();
    mm_selected = (mm_selected + 1) % MAINMENU_SIZE;
  }

  // handle user input
  if (input_key_hit(INPUT_KEY_A | INPUT_KEY_START)){
    play_snd_select();
    switch(mm_selected){
      case MM_PLAY:
        trigger_transition(SCR_DIFFICULTY);
        break;
      case MM_SCORES:
        trigger_transition(SCR_SCORES);
        break;
      case MM_TIPJAR:
        trigger_transition(SCR_TIPJAR);
        break;
      case MM_CREDITS:
        trigger_transition(SCR_CREDITS);
        break;
    }
  }
}

void close_tick(){
  if (transition_ttl == 0){
    scene_set(next_scene);
  }
}

void menu_tick(){
  switch(curr_screen){
    case SCR_MAIN:
      main_screen_tick();
      break;
    case SCR_DIFFICULTY:
      difficulty_screen_tick();
      break;
    case SCR_NONE:
      close_tick();
      break;
    case SCR_SCORES: // fall though
      score_screen_tick();
      break;
    case SCR_TIPJAR:
    case SCR_CREDITS:
      info_screen_tick();
      break;
  }
  transition_tick(); // FIXME crashes when returning on no$gba v2.6a wtf
}

const Scene menu = SCENE(&menu_load, &menu_tick, &menu_draw);

