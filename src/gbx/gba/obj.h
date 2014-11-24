// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_OBJ_H
#define GBX_OBJ_H

#include <src/gbx/common.h>


////////////////////////////////////////////////////////////////////////////////
//                          OBJS                                              //
////////////////////////////////////////////////////////////////////////////////

#define OBJ_OAM_SIZE        0x00400 // bytes (1kb)
#define obj_oam_mem         ((Uint16*)(0x07000000)) // no 8bit writes !
                           
#define OBJ_MEM_LEN         (OBJ_OAM_SIZE / 8)
#define OBJ_MEM             ((Obj*)obj_oam_mem)

typedef struct {
  Uint16 attr0;
  Uint16 attr1;
  Uint16 attr2;
  Sint16 fill;  // careful overlaps with AffineMatrix
} ALIGN4 Obj;


////////////////////////////////////////////////////////////////////////////////
//                          OBJ ATTRIBUTE 0                                   //
//                                                                            //
// BITS NAME                DESCRIPTION                                       //
// 0-7  -                   Vecition on the Y axis.                           //
// 8-9  OBJ_A0_MODE_*       TODO                                              //
// A-B  OBJ_A0_BLEND_*      TODO                                              //
// C    OBJ_A0_MOSAIC       TODO                                              //
// D    OBJ_A0_*BPP         TODO                                              //
// E-F  OBJ_A0_SHAPE_*      TODO                                              //
////////////////////////////////////////////////////////////////////////////////

#define OBJ_A0_Y_MASK       0x00FF  // 255

#define OBJ_A0_MODE_MASK    0x0300
#define OBJ_A0_MODE_REG     0       // Regular object
#define OBJ_A0_MODE_AFF     0x0100  // Affine object
#define OBJ_A0_MODE_HIDE    0x0200  // Inactive object
#define OBJ_A0_MODE_AFF_DBL 0x0300  // Double-size affine object

#define OBJ_A0_BLEND_MASK   0x0C00
#define OBJ_A0_BLEND_NORMAL 0
#define OBJ_A0_BLEND_ALPHA  0x0400  // Enable blend
#define OBJ_A0_BLEND_WINDOW 0x0800  // Use for object window

#define OBJ_A0_MOSAIC       0x1000  // Enable mosaic
#define OBJ_A0_4BPP         0       // Use 4bpp (16 color) tiles
#define OBJ_A0_8BPP         0x2000  // Use 8bpp (256 color) tiles

#define OBJ_A0_SHAPE_MASK   0xC000
#define OBJ_A0_SHAPE_SQUARE 0       // Square shape
#define OBJ_A0_SHAPE_WIDE   0x4000  // Tall shape (height > width)
#define OBJ_A0_SHAPE_TALL   0x8000  // Wide shape (height < width)

inline Uint16 obj_a0_build(Uint16 y, Uint16 mode, Uint16 blend, 
                           Uint16 mosaic, Uint16 bpp, Uint16 shape){
  return ((y & OBJ_A0_Y_MASK) | (mode & OBJ_A0_MODE_MASK) | 
          (blend & OBJ_A0_BLEND_MASK) | (mosaic & OBJ_A0_MOSAIC) | 
          (bpp & OBJ_A0_8BPP)| (shape & OBJ_A0_SHAPE_MASK));
}

inline Obj obj_update_y(Obj obj, Uint16 y){
  BIT_CLEAR(obj.attr0, OBJ_A0_Y_MASK);
  BIT_SET(obj.attr0, y bitand OBJ_A0_Y_MASK);
  return obj;
}

inline Uint16 obj_get_y(Obj obj){
  return obj.attr0 bitand OBJ_A0_Y_MASK;
}

////////////////////////////////////////////////////////////////////////////////
//                              OBJ ATTRIBUTE 1                               //
//                                                                            //
// BITS NAME                    DESCRIPTION                                   //
// 0-8  -                       Vecition on the X axis.                       //
// 9-D  OBJ_A1_AFFINE_ID(n)     TODO                                          //
// C    OBJ_A1_HFLIP            TODO                                          //
// D    OBJ_A1_VFLIP            TODO                                          //
// E-F  OBJ_A1_SIZE             TODO                                          //
////////////////////////////////////////////////////////////////////////////////

#define OBJ_A1_X_MASK           0x01FF  // 511

#define OBJ_A1_AFFINE_ID_MASK   0x3E00
#define OBJ_A1_AFFINE_ID_SHIFT  9
#define OBJ_A1_AFFINE_ID(n)     ((n) << OBJ_A1_AFFINE_ID_SHIFT)

#define OBJ_A1_HFLIP            0x1000  // Horizontal flip (reg obj only)
#define OBJ_A1_VFLIP            0x2000  // Vertical flip (reg obj only)

#define OBJ_A1_SIZE_MASK        0xC000
#define OBJ_A1_SIZE_S_8x8       0       // Square sizes
#define OBJ_A1_SIZE_S_16x16     0x4000  // Square sizes
#define OBJ_A1_SIZE_S_32x32     0x8000  // Square sizes
#define OBJ_A1_SIZE_S_64x64     0xC000  // Square sizes
#define OBJ_A1_SIZE_W_16x8      0       // Wide sizes
#define OBJ_A1_SIZE_W_32x8      0x4000  // Wide sizes
#define OBJ_A1_SIZE_W_32x16     0x8000  // Wide sizes
#define OBJ_A1_SIZE_W_64x32     0xC000  // Wide sizes
#define OBJ_A1_SIZE_T_8x16      0       // Tall sizes
#define OBJ_A1_SIZE_T_8x32      0x4000  // Tall sizes
#define OBJ_A1_SIZE_T_16x32     0x8000  // Tall sizes
#define OBJ_A1_SIZE_T_32x64     0xC000  // Tall sizes

inline Uint16 obj_a1_build_reg(Uint16 x, Uint16 hflip,
                               Uint16 vflip, Uint16 size){
  return ((x & OBJ_A1_X_MASK) | (hflip & OBJ_A1_HFLIP) |
          (vflip & OBJ_A1_VFLIP) | (size & OBJ_A1_SIZE_MASK));
}

inline Uint16 obj_a1_build_aff(Uint16 x, Uint16 size, Uint16 affid){
  return ((x & OBJ_A1_X_MASK) | (size & OBJ_A1_SIZE_MASK) |
          ((affid << OBJ_A1_AFFINE_ID_SHIFT) & OBJ_A1_AFFINE_ID_MASK));
}

inline Obj obj_update_hflip(Obj obj, Uint16 hflip){
  BIT_CLEAR(obj.attr1, OBJ_A1_HFLIP);
  BIT_SET(obj.attr1, hflip & OBJ_A1_HFLIP);
  return obj;
}

inline Obj obj_update_x(Obj obj, Uint16 x){
  BIT_CLEAR(obj.attr1, OBJ_A1_X_MASK);
  BIT_SET(obj.attr1, x & OBJ_A1_X_MASK);
  return obj;
}

inline Uint16 obj_get_x(Obj obj){
  return obj.attr1 bitand OBJ_A1_X_MASK;
}

////////////////////////////////////////////////////////////////////////////////
//                          OBJ ATTRIBUTE 2                                   //
//                                                                            //
// BITS NAME                DESCRIPTION                                       //
// 0-9  -                   Tile index.                                       //
// A-B  OBJ_A2_PRIO(n)      TODO                                              //
// C-F  OBJ_A2_PALBANK(n)   TODO                                              //
////////////////////////////////////////////////////////////////////////////////

#define OBJ_A2_TILE_ID_MASK  0x03FF

#define OBJ_A2_PRIO_MASK     0x0C00
#define OBJ_A2_PRIO_SHIFT    10                         
#define OBJ_A2_PRIO(n)       ((n) << OBJ_A2_PRIO_SHIFT)

#define OBJ_A2_PALBANK_MASK  0xF000
#define OBJ_A2_PALBANK_SHIFT 12
#define OBJ_A2_PALBANK(n)    ((n) << OBJ_A2_PALBANK_SHIFT)

inline Uint16 obj_a2_build(Uint16 id, Uint16 prio, Uint16 palbank){
  return (((palbank << OBJ_A2_PALBANK_SHIFT) & OBJ_A2_PALBANK_MASK) | 
          ((prio << OBJ_A2_PRIO_SHIFT) & OBJ_A2_PRIO_MASK) | 
          (id & OBJ_A2_TILE_ID_MASK));
}


////////////////////////////////////////////////////////////////////////////////
//                              AFFINE MATRICES                               //
////////////////////////////////////////////////////////////////////////////////

#define AFFINE_MATRIX_MEM_LEN   (GFX_MEM_OAM_SIZE / 32)
#define AFFINE_MATRIX_MEM       ((AffineMatrix*)gfx_mem_oam)

typedef struct OBJ_AFFINE {
  Uint16 fill0[3]; // careful overlaps with Obj
  Sint16 pa;
  Uint16 fill1[3]; // careful overlaps with Obj
  Sint16 pb;
  Uint16 fill2[3]; // careful overlaps with Obj
  Sint16 pc;
  Uint16 fill3[3]; // careful overlaps with Obj
  Sint16 pd;
} ALIGN4 AffineMatrix;


////////////////////////////////////////////////////////////////////////////////
//                              MISC FUNCTONS                                 //
////////////////////////////////////////////////////////////////////////////////

inline Uint16 obj_get_w(Obj obj){
  switch(obj.attr0 bitand OBJ_A0_SHAPE_MASK){
    case OBJ_A0_SHAPE_SQUARE:
      switch(obj.attr1 bitand OBJ_A1_SIZE_MASK){
        case OBJ_A1_SIZE_S_8x8:
          return 8;
        case OBJ_A1_SIZE_S_16x16:
          return 16;
        case OBJ_A1_SIZE_S_32x32:
          return 32;
        case OBJ_A1_SIZE_S_64x64:
          return 64;
        default:
          while(1); // unreachable code
      }
    case OBJ_A0_SHAPE_WIDE:
      switch(obj.attr1 bitand OBJ_A1_SIZE_MASK){
        case OBJ_A1_SIZE_W_16x8:
          return 16;
        case OBJ_A1_SIZE_W_32x8:
          return 32;
        case OBJ_A1_SIZE_W_32x16:
          return 32;
        case OBJ_A1_SIZE_W_64x32:
          return 64;
        default:
          while(1); // unreachable code
      }
    case OBJ_A0_SHAPE_TALL:
      switch(obj.attr1 bitand OBJ_A1_SIZE_MASK){
        case OBJ_A1_SIZE_T_8x16:
          return 8;
        case OBJ_A1_SIZE_T_8x32:
          return 8;
        case OBJ_A1_SIZE_T_16x32:
          return 16;
        case OBJ_A1_SIZE_T_32x64:
          return 32;
        default:
          while(1); // unreachable code
      }
    default:
      while(1); // unreachable code
  }
}

inline Uint16 obj_get_h(Obj obj){
  switch(obj.attr0 bitand OBJ_A0_SHAPE_MASK){
    case OBJ_A0_SHAPE_SQUARE:
      switch(obj.attr1 bitand OBJ_A1_SIZE_MASK){
        case OBJ_A1_SIZE_S_8x8:
          return 8;
        case OBJ_A1_SIZE_S_16x16:
          return 16;
        case OBJ_A1_SIZE_S_32x32:
          return 32;
        case OBJ_A1_SIZE_S_64x64:
          return 64;
        default:
          while(1); // unreachable code
      }
    case OBJ_A0_SHAPE_WIDE:
      switch(obj.attr1 bitand OBJ_A1_SIZE_MASK){
        case OBJ_A1_SIZE_W_16x8:
          return 8;
        case OBJ_A1_SIZE_W_32x8:
          return 8;
        case OBJ_A1_SIZE_W_32x16:
          return 16;
        case OBJ_A1_SIZE_W_64x32:
          return 32;
        default:
          while(1); // unreachable code
      }
    case OBJ_A0_SHAPE_TALL:
      switch(obj.attr1 bitand OBJ_A1_SIZE_MASK){
        case OBJ_A1_SIZE_T_8x16:
          return 16;
        case OBJ_A1_SIZE_T_8x32:
          return 32;
        case OBJ_A1_SIZE_T_16x32:
          return 32;
        case OBJ_A1_SIZE_T_32x64:
          return 64;
        default:
          while(1); // unreachable code
      }
    default:
      while(1); // unreachable code
  }
}

inline Obj obj_build_8bpp(Uint16 x, Uint16 y, Uint16 shape, 
                          Uint16 size, Uint16 id){
  Obj obj;
  obj.attr0 = obj_a0_build(
      y, OBJ_A0_MODE_REG, OBJ_A0_BLEND_NORMAL, 0, OBJ_A0_8BPP, shape
  );
  obj.attr1 = obj_a1_build_reg(x, 0, 0, size);
  obj.attr2 = obj_a2_build(id, 0, 0);
  return obj;
}

inline Obj obj_build_8x8_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_SQUARE, OBJ_A1_SIZE_S_8x8, id);
}

inline Obj obj_build_16x16_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_SQUARE, OBJ_A1_SIZE_S_16x16, id);
}

inline Obj obj_build_32x32_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_SQUARE, OBJ_A1_SIZE_S_32x32, id);
}

inline Obj obj_build_64x64_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_SQUARE, OBJ_A1_SIZE_S_64x64, id);
}

inline Obj obj_build_16x8_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_WIDE, OBJ_A1_SIZE_W_16x8, id);
}

inline Obj obj_build_32x8_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_WIDE, OBJ_A1_SIZE_W_32x8, id);
}

inline Obj obj_build_32x16_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_WIDE, OBJ_A1_SIZE_W_32x16, id);
}

inline Obj obj_build_64x32_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_WIDE, OBJ_A1_SIZE_W_64x32, id);
}

inline Obj obj_build_8x16_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_TALL, OBJ_A1_SIZE_T_8x16, id);
}

inline Obj obj_build_8x32_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_TALL, OBJ_A1_SIZE_T_8x32, id);
}

inline Obj obj_build_16x32_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_TALL, OBJ_A1_SIZE_T_16x32, id);
}

inline Obj obj_build_32x64_8bpp(Uint16 x, Uint16 y, Uint16 id){
  return obj_build_8bpp(x, y, OBJ_A0_SHAPE_TALL, OBJ_A1_SIZE_T_32x64, id);
}

inline Obj obj_update_pos(Obj obj, Uint16 x, Uint16 y){
  obj = obj_update_y(obj, y);
  obj = obj_update_x(obj, x);
  return obj;
}

inline Obj obj_update_mode(Obj obj, Uint16 mode){
  BIT_CLEAR(obj.attr0, OBJ_A0_MODE_MASK);
  BIT_SET(obj.attr0, mode bitand OBJ_A0_MODE_MASK);
  return obj;
}

inline Obj obj_hide(Obj obj){
  return obj_update_mode(obj, OBJ_A0_MODE_HIDE);
}

inline void obj_reset(){
  // TODO just zero out everything with memcpy
  for (int i = 0; i < OBJ_MEM_LEN; i++){
    OBJ_MEM[i] = obj_hide(OBJ_MEM[i]);
  }
}

inline Bool obj_offscreen(Obj obj, Sint32 x, Sint32 y){
  // can't use x, y from obj due to integer overflow
  Uint16 w = obj_get_w(obj);
  Uint16 h = obj_get_h(obj);
  Sint32 max_x = GFX_DISPLAY_WIDTH - 1;//OBJ_A1_X_MASK + 1 - w;
  Sint32 min_x = 0 - w;
  Sint32 max_y = GFX_DISPLAY_HEIGHT - 1;//OBJ_A0_Y_MASK + 1 - h;
  Sint32 min_y = 0 - h;
  return x < min_x or x > max_x or y < min_y or y > max_y;
}

#endif
