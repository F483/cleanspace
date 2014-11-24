// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>
#include <src/game/tick.h>
#include <src/game/load.h>
#include <src/game/draw.h>

const FontBG8 game_font = FONTBG8(FONT_TILEBANK, FONT_OFFSET, 
                          FONT_MAPBANK, FONT_REG_BG, 0);

void load_stars_bg_map(){
  for (Uint16 y = 0; y < 20; y++){
    for (Uint16 x = 0; x < 30; x++) {
      Uint16 tile = rand() % TILES_STARS_COUNT;
      Uint16 tileid = tile;
      INDEX_2D(x, y, 32, STARS_BG_MAP_MEM) = tme_build(tileid, 0, 0, 0);
    }
  }
}

void load_video_memory(){

  // copy tiles
  memcpy(PLANETS_BG_TILEBANK_MEM, &bg_game_planets_Tiles, 
         bg_game_planets_TilesLen);
  memcpy(STARS_BG_TILEBANK_MEM, &bg_game_stars_Tiles, bg_game_stars_TilesLen);
  memcpy(TILE_OBJ_MEM_8BPP, &obj_game_Tiles, 
         obj_game_TilesLen);
  font_bg8_cptiles(&game_font, (Tile8*)font8_Tiles);

  // load maps
  memcpy(PLANETS_BG_MAP_MEM_L, &bg_game_planets_Map, bg_game_planets_MapLen);
  load_stars_bg_map();

  pal_set(bg_game_planets_Pal, obj_game_Pal);
}

void load_display_registers(){
  // setup display
  gfx_reg_display = GFX_DISPLAY_MODE0 | PLANETS_BG | STARS_BG | FONT_BG |
                    GFX_DISPLAY_OBJ | GFX_DISPLAY_OBJ_2D;

  // setup stars bg registers
  STARS_BG_REG = GFX_BG_TILE_MEM(STARS_BG_TILEBANK) | 
                 GFX_BG_MAP_MEM(STARS_BG_MAPBANK) | 
                 STARS_BG_MAPSIZE | STARS_BG_BPP | STARS_BG_PRIO;

  // setup planet bg registers
  PLANETS_BG_REG = GFX_BG_TILE_MEM(PLANETS_BG_TILEBANK) | 
                   GFX_BG_MAP_MEM(PLANETS_BG_MAPBANK) | 
                   PLANETS_BG_MAPSIZE | PLANETS_BG_BPP | PLANETS_BG_PRIO;

  // setup font
  font_bg8_init(&game_font);
}

void load_gfx(){
  load_display_registers();
  load_video_memory();
}

void init_game_tick(){
  Vec tick_avatar_pos = vec_make_int(0, GAME_H / 2);
  Vec tick_avatar_vec = vec_make_int(2, 0);

  tick_boost_vec = vec_zero;
  tick_boost_ttl = 0;
  tick_avatar = (Avatar){ tick_avatar_pos, tick_avatar_vec, 0 };
  tick_energie = POWER_MAXIMUM / 2;
  tick_points = 0;
  tick_new_trash_x = 64;

  // put all tick_trash off screen so it will be respawned
  for(Uint32 i = 0; i < TRASH_SIZE; i++){
    trash_clear(&tick_trash[i]);
  }
  
  // clear tick_scores
  for(Uint32 i = 0; i < SCORE_SIZE; i++){
    tick_scores[i].ttl = 0;
  }
}

void init_game_draw(){
  draw_planet_bg_x = 0;
  draw_planet_bg_y = 0;
  draw_fadein_ttl = FADEIN_DURATION - 1;
}

void game_load(){
  load_gfx();
  init_game_draw();
  init_game_tick();
}

