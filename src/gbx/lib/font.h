// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_FONT_H
#define GBX_FONT_H

#include <src/gbx/common.h>
#include <src/gbx/lib/vec.h>


////////////////////////////////////////////////////////////////////////////////
//                FONT BG 4                   //
////////////////////////////////////////////////////////////////////////////////

#define FONTBG4(tilebank, offset, mapbank, reg_bg, prio) \
  { tilebank, offset, mapbank, reg_bg, prio }

typedef struct {    // Background font using 30x20 grid.
  Uint16 tilebank;  // The tilebank to copy the font tiles into.
  Uint16 offset;    // How many tiles to skip before copying font tiles.
  Uint16 mapbank;   // Which map bank to print to (uses 32x32 map).
  Uint16 reg_bg;    // The background register to use.
  Uint16 prio;      // Background draw priority 0 - 3.
} FontBG4;

void font_bg4_init(const FontBG4* font); // init sys regs and map
void font_bg4_cptiles(const FontBG4* font, const Tile4* tiles); // cp tiles vram
void font_bg4_clear(const FontBG4* font); // remove all chars from map
void font_bg4_print(const FontBG4* font, Uint16 palbank, 
                    Vec pos, const char* str);
void font_bg4_printf(const FontBG4* font, Uint16 palbank, 
                     Vec pos, const char* format, ...);

void font_bg4_printu(const FontBG4* font, Uint16 palbank, Vec pos, Uint16 num);


////////////////////////////////////////////////////////////////////////////////
//                FONT BG 8                   //
////////////////////////////////////////////////////////////////////////////////

#define FONTBG8(tilebank, offset, mapbank, reg_bg, prio) \
  { tilebank, offset, mapbank, reg_bg, prio }

typedef struct {    // Background font using 30x20 grid.
  Uint16 tilebank;  // The tilebank to copy the font tiles into.
  Uint16 offset;    // How many tiles to skip before copying font tiles.
  Uint16 mapbank;   // Which map bank to print to (uses 32x32 map).
  Uint16 reg_bg;    // The background register to use.
  Uint16 prio;      // Background draw priority 0 - 3.
} FontBG8;

void font_bg8_init(const FontBG8* font); // init sys regs and map
void font_bg8_cptiles(const FontBG8* font, const Tile8* tiles); // cp tiles vram
void font_bg8_clear(const FontBG8* font); // remove all chars from map
void font_bg8_print(const FontBG8* font, Vec pos, const char* str);
void font_bg8_printf(const FontBG8* font, Vec pos, const char* format, ...);
void font_bg8_printu(const FontBG8* font, Vec pos, Uint16 num);

#endif
