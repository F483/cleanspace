// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_TILE_H
#define GBX_TILE_H

#include <src/gbx/common.h>
#include <src/gbx/gba/gfx.h>

typedef struct { Uint32 data[8];  } Tile4; // 4bpp 8x8 tile
typedef struct { Uint32 data[16]; } Tile8; // 8bpp 8x8 tile

#define TILE_SIZE_4BPP          32 // byte
#define TILE_SIZE_8BPP          64 // byte


////////////////////////////////////////////////////////////////////////////////
//                              MEMORY LAYOUT                                 //
//                                                                            //
// gfx_mem_vram + 0x0000 0x4000 0x8000 0xC000 0x10000 0x14000                 //
// TILE_MEM       0      1      2      3      OBJ     OBJM3                   //
// MAP_MEM        0....7 8...15 16..23 24..31                                 //
////////////////////////////////////////////////////////////////////////////////

#define TILE_BANK_COUNT         4 
#define TILE_BANK_SIZE          0x4000 // bytes (16kb)
#define TILE_BANK_MEM(i)        (&gfx_mem_vram[((TILE_BANK_SIZE/2)*(i))])
#define TILE_BANK_LEN_4BPP      (TILE_BANK_SIZE / TILE_SIZE_4BPP)
#define TILE_BANK_LEN_8BPP      (TILE_BANK_SIZE / TILE_SIZE_8BPP)
#define TILE_BANK_MEM_4BPP(i)   ((Tile4*)TILE_BANK_MEM(i))
#define TILE_BANK_MEM_8BPP(i)   ((Tile8*)TILE_BANK_MEM(i))

#define TILE_OBJ_SIZE           0x8000 // bytes (32kb)
#define TILE_OBJ_MEM            (&gfx_mem_vram[(TILE_BANK_SIZE/2)*4])
#define TILE_OBJ_LEN_4BPP       (TILE_OBJ_SIZE / TILE_SIZE_4BPP)
#define TILE_OBJ_LEN_8BPP       (TILE_OBJ_SIZE / TILE_SIZE_8BPP)
#define TILE_OBJ_MEM_4BPP       ((Tile4*)TILE_OBJ_MEM)
#define TILE_OBJ_MEM_8BPP       ((Tile8*)TILE_OBJ_MEM)
#define TILE_OBJM3_OFFSET_4BPP  (TILE_OBJ_LEN_4BPP / 2)
#define TILE_OBJM3_OFFSET_8BPP  (TILE_OBJ_LEN_8BPP / 2)
#define TILE_OBJM3_MEM_4BPP     (&TILE_OBJ_MEM_4BPP[TILE_OBJM3_OFFSET_4BPP])
#define TILE_OBJM3_MEM_8BPP     (&TILE_OBJ_MEM_8BPP[TILE_OBJM3_OFFSET_8BPP])

#endif
