// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_SAV_H
#define GBX_SAV_H

#include <src/gbx/common.h>


////////////////////////////////////////////////////////////////////////////////
//                   CART RAM                                                 //
////////////////////////////////////////////////////////////////////////////////

#define SAV_SIZE     0x10000 // 64k CAREFUL THIS MAY NOT BE TRUE!!!!!
#define SAV_MEM      ((Uint8*)0x0E000000)

inline Uint8 sav_read8(Uint16 index){
  return SAV_MEM[index];
}

inline void sav_write8(Uint16 index, Uint8 data){
  SAV_MEM[index] = data; // can't overflow 2^16 == SAV_SIZE
}

inline Uint16 sav_read16(Uint16 index){
  Uint8 a = sav_read8(index + 0);
  Uint8 b = sav_read8(index + 1);
  return ((Uint16)a << 8) + (Uint16)b;
}

inline void sav_write16(Uint16 index, Uint16 data){
  Uint8 a = (Uint8)(data >> 8);
  Uint8 b = (Uint8)data;
  sav_write8(index + 0, a);
  sav_write8(index + 1, b);
}

inline Uint32 sav_read32(Uint16 index){
  Uint8 a = sav_read8(index + 0);
  Uint8 b = sav_read8(index + 1);
  Uint8 c = sav_read8(index + 2);
  Uint8 d = sav_read8(index + 3);
  return ((Uint32)a << 24) + ((Uint32)b << 16) + ((Uint32)c << 8) + (Uint32)d;
}

inline void sav_write32(Uint16 index, Uint32 data){
  Uint8 a = (Uint8)(data >> 24);
  Uint8 b = (Uint8)(data >> 16);
  Uint8 c = (Uint8)(data >> 8);
  Uint8 d = (Uint8)data;
  sav_write8(index + 0, a);
  sav_write8(index + 1, b);
  sav_write8(index + 2, c);
  sav_write8(index + 3, d);
}

inline void sav_init(Uint32 nonce){
  if (sav_read32(0) != nonce){ // uninitialized cart ram
    for (Uint32 i = 0; i < 65536; i++){
      SAV_MEM[i] = 0; // wipe with zeros
    }
    sav_write32(0, nonce);
  } 
}

#endif
