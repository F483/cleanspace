// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <gfx/bg_company.h>
#include <src/gbx.h>
#include <src/scenes.h>

// memory setup
#define BG_REG            gfx_reg_bg0
#define BG_REG_H          gfx_reg_bg0_hpos
#define BG_REG_V          gfx_reg_bg0_vpos
#define BG_FLAG           GFX_DISPLAY_BG0
#define BG_TILEBANK       0
#define BG_TILEBANK_MEM   TILE_BANK_MEM_8BPP(BG_TILEBANK)
#define BG_MAPBANK        31
#define BG_MAPBANK_MEM    MAP_BANK_MEM(BG_MAPBANK)
#define BG_MAPSIZE        GFX_BG_REG_32x32
#define BG_BPP            GFX_BG_8BPP

// source tiles and maps
#define BG_TILESLEN       bg_company_TilesLen
#define BG_TILES          (Tile8*)&bg_company_Tiles
#define BG_MAPLEN         bg_company_MapLen
#define BG_MAP            (Uint16*)&bg_company_Map
#define BG_PALETTE        (Uint16*)&bg_company_Pal

// fade and display times
#define FADEIN_DURATION   60
#define FADEOUT_DURATION  60
#define DISPLAY_DURATION  (60 * 5)

// Player input
#define NEXT_KEYS         (INPUT_KEY_START | INPUT_KEY_SELECT | \
                           INPUT_KEY_A | INPUT_KEY_B)

void company_load(){
  // setup stars bg registers
  gfx_reg_display = GFX_DISPLAY_MODE0 | BG_FLAG;

  BG_REG = GFX_BG_TILE_MEM(BG_TILEBANK) | GFX_BG_MAP_MEM(BG_MAPBANK) | 
           BG_MAPSIZE | BG_BPP;
  BG_REG_H = 0;
  BG_REG_V = 0;

  // copy tiles, map and palette
  memcpy(BG_TILEBANK_MEM, BG_TILES, BG_TILESLEN);
  memcpy(BG_MAPBANK_MEM, BG_MAP, BG_MAPLEN);
  pal_set_bg(BG_PALETTE);
}

void company_tick(){
  static Uint32 ttl = DISPLAY_DURATION;
  if (!ttl or input_key_down(NEXT_KEYS)){
    scene_set(&menu);
  } 
  ttl--;
}

void company_draw(){
  Sint32 ttl_fadein = FADEIN_DURATION - scene_frame - 1;
  Sint32 ttl_fadeout = DISPLAY_DURATION - scene_frame;
  gfx_fade_from_black(ttl_fadein, FADEIN_DURATION);
  gfx_fade_into_black(ttl_fadeout, FADEIN_DURATION);
}

const Scene company = SCENE(&company_load, &company_tick, &company_draw);

