// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>
#include <stdio.h>
#include <stdarg.h>

#define _MAX_X_TILES        (GFX_DISPLAY_WIDTH / 8)
#define _MAX_Y_TILES        (GFX_DISPLAY_HEIGHT / 8)
#define _PRINTF_BUFFER_SIZE (_MAX_X_TILES * _MAX_Y_TILES + _MAX_Y_TILES)
char* _printf_buffer[_PRINTF_BUFFER_SIZE];

void font_init_sysregs(Uint16 font_reg_bg, Uint16 font_tilebank, 
                       Uint16 font_mapbank, Uint16 font_prio, 
                       Uint16 gfx_bg_bpp){

  // get bg registers
  volatile Uint16* reg_bg = NULL; 
  volatile Uint16* reg_bg_hpos = NULL; 
  volatile Uint16* reg_bg_vpos = NULL; 
  switch(font_reg_bg){
    case 0: 
      reg_bg    = &gfx_reg_bg0; 
      reg_bg_hpos = &gfx_reg_bg0_hpos; 
      reg_bg_vpos = &gfx_reg_bg0_vpos; 
      break;
    case 1: 
      reg_bg    = &gfx_reg_bg1; 
      reg_bg_hpos = &gfx_reg_bg1_hpos; 
      reg_bg_vpos = &gfx_reg_bg1_vpos; 
      break;
    case 2: 
      reg_bg    = &gfx_reg_bg2; 
      reg_bg_hpos = &gfx_reg_bg2_hpos; 
      reg_bg_vpos = &gfx_reg_bg2_vpos; 
      break;
    case 3: 
      reg_bg    = &gfx_reg_bg3; 
      reg_bg_hpos = &gfx_reg_bg3_hpos; 
      reg_bg_vpos = &gfx_reg_bg3_vpos; 
      break;
    default: 
      // should never happen
      return;
  }

  // setup registers
  *reg_bg = GFX_BG_TILE_MEM(font_tilebank) | GFX_BG_MAP_MEM(font_mapbank) | 
            GFX_BG_REG_32x32 | gfx_bg_bpp | GFX_BG_PRIO(font_prio);
  *reg_bg_hpos = 0;
  *reg_bg_vpos = 0;

}

void font_bg8_init(const FontBG8* font){
  font_init_sysregs(font->reg_bg, font->tilebank, font->mapbank, font->prio, 
                    GFX_BG_8BPP);
  font_bg8_clear(font);
}

void font_bg4_init(const FontBG4* font){
  font_init_sysregs(font->reg_bg, font->tilebank, font->mapbank, font->prio, 
                    GFX_BG_4BPP);
  font_bg4_clear(font);
}

void font_bg8_cptiles(const FontBG8* font, const Tile8* tiles){
  Tile8* tile_mem = &(TILE_BANK_MEM_8BPP(font->tilebank)[font->offset]);
  memcpy(tile_mem, tiles, TILE_SIZE_8BPP * 96);
}

void font_bg4_cptiles(const FontBG4* font, const Tile4* tiles){
  Tile4* tile_mem = &(TILE_BANK_MEM_4BPP(font->tilebank)[font->offset]);
  memcpy(tile_mem, tiles, TILE_SIZE_4BPP * 96);
}

void font_bg_clear(Uint16 mapbank, Uint16 offset){
  Uint32 eraser = (Uint32)offset | (Uint32)offset << 16;
  Uint32* map = (Uint32*)MAP_BANK_MEM(mapbank);
  // only clear first 20 lines that can be seen
  for(int i = 0; i < (20 * 16); i += 16){
    // unroll to clear entire line at once
    map[i +  0] = eraser; map[i +  1] = eraser; map[i +  2] = eraser; 
    map[i +  3] = eraser; map[i +  4] = eraser; map[i +  5] = eraser; 
    map[i +  6] = eraser; map[i +  7] = eraser; map[i +  8] = eraser; 
    map[i +  9] = eraser; map[i + 10] = eraser; map[i + 11] = eraser;
    map[i + 12] = eraser; map[i + 13] = eraser; map[i + 14] = eraser;
    // skip last one since it can never be seen
  }
}

void font_bg8_clear(const FontBG8* font){
  font_bg_clear(font->mapbank, font->offset);
}

void font_bg4_clear(const FontBG4* font){
  font_bg_clear(font->mapbank, font->offset);
}

void font_bg8_printf(const FontBG8* font, Vec pos, const char* format, ...){
  va_list args;
  va_start(args, format);
  vsnprintf((char*)&_printf_buffer, _PRINTF_BUFFER_SIZE, format, args);
  font_bg8_print(font, pos, (char*)&_printf_buffer);
  va_end(args);
}

void font_bg4_printf(const FontBG4* font, Uint16 palbank, 
          Vec pos, const char* format, ...){
  va_list args;
  va_start(args, format);
  vsnprintf((char*)&_printf_buffer, _PRINTF_BUFFER_SIZE, format, args);
  font_bg4_print(font, palbank, pos, (char*)&_printf_buffer);
  va_end(args);
}

void font_bg8_printu(const FontBG8* font, Vec pos, Uint16 num){
  char buffer[6];
  uitoa(buffer, num);
  font_bg8_print(font, pos, buffer);
}

void font_bg4_printu(const FontBG4* font, Uint16 palbank, 
           Vec pos, Uint16 num){
  char buffer[6];
  uitoa(buffer, num);
  font_bg4_print(font, palbank, pos, buffer);
}

void font_bg_print(Uint16 font_offset, Uint16 font_mapbank, Uint16 palbank, 
           Vec pos, const char* str){
  // TODO make this slow bitch faster
  Sint32 pos_x = vec_get_x_int(pos);
  Uint32 c = 0;
  Sint32 cx = pos_x;
  Sint32 cy = vec_get_y_int(pos);
  Sint32 index_offset = font_offset - 32;
  Uint32 tme_palbank = TME_PALBANK(palbank);
  Uint16* map = MAP_BANK_MEM(font_mapbank);
  while((c=*str++) != '\0'){ // TODO faster with duff's device?
    if (cy >= 20) { // can never be visible (below bottom of screen)
      return;
    } else if (c >= 32 && c <= 126) {
      if (cy < 0 or cx >= 30) { // off screen
        cx++; // move curser
        continue;
      }
      // draw char
      // TODO faster 16 palbank luckup tables?
      INDEX_2D(cx, cy, 32, map) = (c + index_offset) | tme_palbank;
      cx++; // move curser
    } else if (c == '\n') { // new line char
      cx = pos_x;
      cy++;
    }
  }
}

void font_bg8_print(const FontBG8* font, Vec pos, const char* str){
  font_bg_print(font->offset, font->mapbank, 0, pos, str);
}

void font_bg4_print(const FontBG4* font, Uint16 palbank, 
          Vec pos, const char* str){
  font_bg_print(font->offset, font->mapbank, palbank, pos, str);
}

