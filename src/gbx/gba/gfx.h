// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_GFX_H
#define GBX_GFX_H

#include <src/gbx/common.h>
#include <src/gbx/lib/math.h>

#define GFX_DISPLAY_WIDTH     240
#define GFX_DISPLAY_HEIGHT    160


////////////////
// SYS MEMORY //
////////////////

#define GFX_MEM_VRAM_SIZE       0x18000 // bytes (96kb)
#define gfx_mem_vram            ((Uint16*)(0x06000000)) // no 8bit writes !


///////////////////////////////
// DISPLAY CONTROL REGISTERS //
///////////////////////////////

#define gfx_reg_display         (*(VUint32*)0x04000000)
#define gfx_reg_vcount          (*(VUint16*)0x04000006)
#define gfx_reg_bg0             (*(VUint16*)0x04000008)
#define gfx_reg_bg1             (*(VUint16*)0x0400000A)
#define gfx_reg_bg2             (*(VUint16*)0x0400000C)
#define gfx_reg_bg3             (*(VUint16*)0x0400000E)
#define gfx_reg_bg0_hpos        (*(VUint16*)0x04000010)
#define gfx_reg_bg0_vpos        (*(VUint16*)0x04000012)
#define gfx_reg_bg1_hpos        (*(VUint16*)0x04000014)
#define gfx_reg_bg1_vpos        (*(VUint16*)0x04000016)
#define gfx_reg_bg2_hpos        (*(VUint16*)0x04000018)
#define gfx_reg_bg2_vpos        (*(VUint16*)0x0400001A)
#define gfx_reg_bg3_hpos        (*(VUint16*)0x0400001C)
#define gfx_reg_bg3_vpos        (*(VUint16*)0x0400001E)


////////////////////////////////////////////////////////////////////////////////
//                              DISPLAY REG SETTINGS                          //
//                                                                            //
// BITS NAME                    DESCRIPTION                                   //
// 0-2  GFX_DISPLAY_MODE*       Sets mode! 0, 1, 2 tiled; 3, 4, 5 bitmap.     //
// 3    GFX_DISPLAY_GB          Is set if cartridge is a GBC game. Read-only. //
// 4    GFX_DISPLAY_PAGE*       Set page for mode 4, 5 page flipping          //
// 5    GFX_DISPLAY_OAM_HBL     Allow OAM updates in HBLank.                  //
// 6    GFX_DISPLAY_OBJ_(1|2)D  OBJ-VRAM as array or matrix.                  //
// 7    GFX_DISPLAY_BLANK       Force screen blank.                           //
// 8-B  GFX_DISPLAY_*           Enable background and obj rendering           //
// D-F  GFX_DISPLAY_WIN*        TODO                                          //
////////////////////////////////////////////////////////////////////////////////

#define GFX_DISPLAY_MODE0       0x0000 // set mode 0: bg 0-4: reg
#define GFX_DISPLAY_MODE1       0x0001 // set mode 1: bg 0-1: reg; bg 2: affine
#define GFX_DISPLAY_MODE2       0x0002 // set mode 2: bg 2-3: affine
#define GFX_DISPLAY_MODE3       0x0003 // set mode 3: bg2: 240x160@16bpp
#define GFX_DISPLAY_MODE4       0x0004 // set mode 4: bg2: 240x160@8bpp
#define GFX_DISPLAY_MODE5       0x0005 // set mode 5: bg2: 160x128@16bpp
#define GFX_DISPLAY_GB          0x0008 // GBC indicator
#define GFX_DISPLAY_PAGE0       0x0000 // render page 0, draw to page 1
#define GFX_DISPLAY_PAGE1       0x0010 // render page 1, draw to page 0
#define GFX_DISPLAY_OAM_HBL     0x0020 // Allow OAM updates in HBlank
#define GFX_DISPLAY_OBJ_2D           0 // OBJ-VRAM as matrix
#define GFX_DISPLAY_OBJ_1D      0x0040 // OBJ-VRAM as array
#define GFX_DISPLAY_BLANK       0x0080 // Force screen blank
#define GFX_DISPLAY_BG0         0x0100 // enable background 0
#define GFX_DISPLAY_BG1         0x0200 // enable background 1
#define GFX_DISPLAY_BG2         0x0400 // enable background 2
#define GFX_DISPLAY_BG3         0x0800 // enable background 3
#define GFX_DISPLAY_OBJ         0x1000 // enable obj objects
#define GFX_DISPLAY_WIN0        0x2000 // Enable window 0
#define GFX_DISPLAY_WIN1        0x4000 // Enable window 1
#define GFX_DISPLAY_WINOBJ      0x8000 // Enable object window


////////////////////////////////////////////////////////////////////////////////
//                              BACKGROUND REG SETTINGS                       //
//                                                                            //
// BITS NAME                    DESCRIPTION                                   //
// 0-1  GFX_BG_PRIO(n)          Priority set drawing order of backgrounds.    //
// 2-3  GFX_BG_TILE_MEM(n)      Set which tile mem bank to read tiles from.   //
// 4-5  -                       Not used                                      //
// 6    GFX_BG_MOSAIC           Enable mosaic effect                          //
// 7    GFX_BG_*BPP             4bpp(16x16 palette) or 8bpp(256 palette)tiles.//
// 8-C  GFX_BG_MAP_MEM(n)       Set which map mem bank to read the map from.  //
// D    GFX_BG_WRAP             Set affine maps to wrap around edges.         //
// E-F  GFX_BG_(REG|AFF)_**x**  Set map size.                                 //
////////////////////////////////////////////////////////////////////////////////

#define GFX_BG_MOSAIC           0x0040  // enable mosaic effect 
#define GFX_BG_WRAP             0x2000  // Wrap around edges of affine bgs
#define GFX_BG_4BPP             0       // 4bpp tiles with 16x16 palette
#define GFX_BG_8BPP             0x0080  // 8bpp tiles with 256 palette
#define GFX_BG_REG_32x32        0       // (256x256 px)
#define GFX_BG_REG_64x32        0x4000  // (512x256 px)
#define GFX_BG_REG_32x64        0x8000  // (256x512 px)
#define GFX_BG_REG_64x64        0xC000  // (512x512 px)
#define GFX_BG_AFF_16x16        0       // (128x128 px)
#define GFX_BG_AFF_32x32        0x4000  // (256x256 px)
#define GFX_BG_AFF_64x64        0x8000  // (512x512 px)
#define GFX_BG_AFF_128x128      0xC000  // (1024x1024 px)
#define GFX_BG_PRIO_MASK        0x0003
#define GFX_BG_PRIO_SHIFT       0
#define GFX_BG_PRIO(n)          ((n) << GFX_BG_PRIO_SHIFT)
#define GFX_BG_TILE_MEM_MASK    0x000C
#define GFX_BG_TILE_MEM_SHIFT   2
#define GFX_BG_TILE_MEM(n)      ((n) << GFX_BG_TILE_MEM_SHIFT)
#define GFX_BG_MAP_MEM_MASK     0x1F00
#define GFX_BG_MAP_MEM_SHIFT    8
#define GFX_BG_MAP_MEM(n)       ((n) << GFX_BG_MAP_MEM_SHIFT)


////////////////////////////////////////////////////////////////////////////////
//                              COLOR EFFECTS CONTORL REGISTERS               //
////////////////////////////////////////////////////////////////////////////////

#define gfx_reg_cfx_ctrl        (*(VUint16*)0x04000050) // BLDCTL
#define gfx_reg_cfx_alpha       (*(VUint16*)0x04000052) // BLDALPHA
#define gfx_reg_cfx_fade        (*(VUint16*)0x04000054) // BLDY


////////////////////////////////////////////////////////////////////////////////
//                              COLOR EFFECTS REG SETTINGS                    //
////////////////////////////////////////////////////////////////////////////////

#define GFX_CFX_CTRL_T1_BG0     0x0001 // 1st Target Background 0
#define GFX_CFX_CTRL_T1_BG1     0x0002 // 1st Target Background 1
#define GFX_CFX_CTRL_T1_BG2     0x0004 // 1st Target Background 2
#define GFX_CFX_CTRL_T1_BG3     0x0008 // 1st Target Background 3
#define GFX_CFX_CTRL_T1_OBJ     0x0010 // 1st Target Top-most OBJ
#define GFX_CFX_CTRL_T1_BD      0x0020 // 1st Target Backdrop)
#define GFX_CFX_CTRL_NONE       0x0000 // Disable color effects
#define GFX_CFX_CTRL_ALPHA      0x0040 // 1st+2nd Target mixed
#define GFX_CFX_CTRL_GAMMA_INC  0x0080 // 1st Target becomes whiter
#define GFX_CFX_CTRL_GAMMA_DEC  0x00C0 // 1st Target becomes blacker
#define GFX_CFX_CTRL_T2_BG0     0x0100 // 2nd Target Background 0
#define GFX_CFX_CTRL_T2_BG1     0x0200 // 2nd Target Background 1
#define GFX_CFX_CTRL_T2_BG2     0x0400 // 2nd Target Background 2
#define GFX_CFX_CTRL_T2_BG3     0x0800 // 2nd Target Background 3
#define GFX_CFX_CTRL_T2_OBJ     0x1000 // 2nd Target Top-most OBJ
#define GFX_CFX_CTRL_T2_BD      0x2000 // 2nd Target Backdrop

// EVA Coefficient (1st Target) 
#define GFX_CFX_ALPHA_T1_MASK   0x001F
#define GFX_CFX_ALPHA_T1_SHIFT  0
#define GFX_CFX_ALPHA_T1(n)     ((n) << GFX_CFX_ALPHA_EVA_SHIFT)

// EVB Coefficient (2st Target) 
#define GFX_CFX_ALPHA_T2_MASK   0x1F
#define GFX_CFX_ALPHA_T2_SHIFT  8
#define GFX_CFX_ALPHA_T2(n)     ((n) << GFX_CFX_ALPHA_EVB_SHIFT)

// EVY Coefficient
#define GFX_CFX_GAMMA_RANGE     15
#define GFX_CFX_GAMMA_MASK      0x000F
#define GFX_CFX_GAMMA_SHIFT     0
#define GFX_CFX_GAMMA(n)        ((n) << GFX_CFX_ALPHA_EVA_SHIFT)


//////////////////////
// HELPER FUNCTIONS //
//////////////////////


/**
 * If ttl >= duration it will wait to fade in.
 * @param duration of the fade effect in frames
 * @param ttl fade time remaning in frames
 * @param fadein if true otherwise fadeout
 * @param brighten if true fade to white otherwise black
 */
inline void gfx_fade(Sint32 ttl, Uint32 duration, Bool fadein, Bool brighten){
  if (ttl >= (Sint32)duration or ttl < 0) // to early or done
    return; 

  if (brighten){
    gfx_reg_cfx_ctrl = GFX_CFX_CTRL_GAMMA_INC | GFX_CFX_CTRL_T1_BG0 | 
                       GFX_CFX_CTRL_T1_BG1 | GFX_CFX_CTRL_T1_BG2 | 
                       GFX_CFX_CTRL_T1_BG3 | GFX_CFX_CTRL_T1_OBJ | 
                       GFX_CFX_CTRL_T1_BD;
  } else {
    gfx_reg_cfx_ctrl = GFX_CFX_CTRL_GAMMA_DEC | GFX_CFX_CTRL_T1_BG0 | 
                       GFX_CFX_CTRL_T1_BG1 | GFX_CFX_CTRL_T1_BG2 | 
                       GFX_CFX_CTRL_T1_BG3 | GFX_CFX_CTRL_T1_OBJ | 
                       GFX_CFX_CTRL_T1_BD;
  }

  Uint32 n = fadein ? (Uint32)ttl : (duration - ttl);
  gfx_reg_cfx_fade = relative_align(n, 0, duration, 0, GFX_CFX_GAMMA_RANGE);
}

inline void gfx_fade_into_white(Sint32 ttl, Uint32 duration){
  gfx_fade(ttl, duration, false, true);
}

inline void gfx_fade_into_black(Sint32 ttl, Uint32 duration){
  gfx_fade(ttl, duration, false, false);
}

inline void gfx_fade_from_white(Sint32 ttl, Uint32 duration){
  gfx_fade(ttl, duration, true, true);
}

inline void gfx_fade_from_black(Sint32 ttl, Uint32 duration){
  gfx_fade(ttl, duration, true, false);
}

inline void gfx_vsync_busywait(){
  while(gfx_reg_vcount >= 160);   // wait till VDraw
  while(gfx_reg_vcount < 160);  // wait till VBlank
}

inline void gfx_reset(){
  // TODO zero out vram with memcpy
  gfx_reg_display   = 0;
  gfx_reg_bg0       = 0;
  gfx_reg_bg0_hpos  = 0;
  gfx_reg_bg0_vpos  = 0;
  gfx_reg_bg1       = 0;
  gfx_reg_bg1_hpos  = 0;
  gfx_reg_bg1_vpos  = 0;
  gfx_reg_bg2       = 0;
  gfx_reg_bg2_hpos  = 0;
  gfx_reg_bg2_vpos  = 0;
  gfx_reg_bg3       = 0;
  gfx_reg_bg3_hpos  = 0;
  gfx_reg_bg3_vpos  = 0;
  gfx_reg_cfx_ctrl  = 0;
  gfx_reg_cfx_alpha = 0;
  gfx_reg_cfx_fade  = 0;
}

#endif
