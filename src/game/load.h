// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef CLEANSPACE_GAME_LOAD_H
#define CLEANSPACE_GAME_LOAD_H

#include <gfx/font8.h>
#include <gfx/bg_game_planets.h>
#include <gfx/bg_game_stars.h>
#include <gfx/obj_game.h>


/////////////////
// BACKGROUNDS //
/////////////////

#define STARS_BG_BPP            GFX_BG_8BPP
#define STARS_BG                GFX_DISPLAY_BG0
#define STARS_BG_REG            gfx_reg_bg0
#define STARS_BG_REG_HVEC       gfx_reg_bg0_hpos
#define STARS_BG_REG_VVEC       gfx_reg_bg0_vpos
#define STARS_BG_TILEBANK       3
#define STARS_BG_TILEBANK_MEM   TILE_BANK_MEM_8BPP(STARS_BG_TILEBANK)
#define STARS_BG_MAPBANK        30 
#define STARS_BG_MAPSIZE        GFX_BG_REG_32x32
#define STARS_BG_MAP_MEM        MAP_BANK_MEM(STARS_BG_MAPBANK)
#define STARS_BG_PRIO           GFX_BG_PRIO(3)
#define TILES_STARS_COUNT       16

#define PLANETS_BG_BPP          GFX_BG_8BPP
#define PLANETS_BG              GFX_DISPLAY_BG1
#define PLANETS_BG_REG          gfx_reg_bg1
#define PLANETS_BG_REG_HVEC     gfx_reg_bg1_hpos
#define PLANETS_BG_REG_VVEC     gfx_reg_bg1_vpos
#define PLANETS_BG_TILEBANK     0 // 0 - 2 with room for mapbank at the end
#define PLANETS_BG_TILEBANK_MEM TILE_BANK_MEM_8BPP(PLANETS_BG_TILEBANK)
#define PLANETS_BG_MAPBANK      22 // 22 + 23
#define PLANETS_BG_MAPSIZE      GFX_BG_REG_64x32
#define PLANETS_BG_PRIO         GFX_BG_PRIO(2)
#define PLANETS_BG_MAP_MEM_L    MAP_BANK_MEM(PLANETS_BG_MAPBANK + 0)
#define PLANETS_BG_MAP_MEM_R    MAP_BANK_MEM(PLANETS_BG_MAPBANK + 1)
#define PLANETS_BG_W            512
#define PLANETS_BG_H            256
#define PLANETS_BG_Y_MIN        0
#define PLANETS_BG_Y_MAX        (PLANETS_BG_H - GFX_DISPLAY_HEIGHT - 1) // inc

#define FONT_BG                 GFX_DISPLAY_BG3
#define FONT_REG_BG             3
#define FONT_TILEBANK           3
#define FONT_OFFSET             TILES_STARS_COUNT
#define FONT_MAPBANK            31

extern const FontBG8 game_font;

extern Uint16 game_obj_pal_buf[256];

#endif
