// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_PAL_H
#define GBX_PAL_H

#include <src/gbx/common.h>

#define PAL_MEM_SIZE          0x00400 // bytes (1kb)
#define pal_mem               ((Uint16*)(0x05000000)) // no 8bit writes !

#define PAL_SIZE              (PAL_MEM_SIZE / 2)  // 512 byte
#define PAL_BG_MEM            (&pal_mem[0])       // first 512 byte
#define PAL_OBJ_MEM           (&pal_mem[256])     // last 512 byte
#define PAL_BANK_SIZE         (PAL_SIZE / 16)
#define PAL_BANK_BG_MEM(i)    (&PAL_BG_MEM[16 * (i)]) 
#define PAL_BANK_OBJ_MEM(i)   (&PAL_OBJ_MEM[16 * (i)])

inline void pal_reset_bg(){
  memset(PAL_BG_MEM, 0, PAL_SIZE);
}

inline void pal_reset_obj(){
  memset(PAL_OBJ_MEM, 0, PAL_SIZE);
}

inline void pal_reset(){
  pal_reset_bg();
  pal_reset_obj();
}

inline void pal_reset_bg_bank(Uint32 bank){
  memset(PAL_BANK_BG_MEM(bank), 0, PAL_BANK_SIZE);
}

inline void pal_reset_obj_bank(Uint32 bank){
  memset(PAL_BANK_OBJ_MEM(bank), 0, PAL_BANK_SIZE);
}

inline void pal_set_bg(const Uint16 pal[256]){
  memcpy(PAL_BG_MEM, pal, PAL_SIZE);
}

inline void pal_set_obj(const Uint16 pal[256]){
  memcpy(PAL_OBJ_MEM, pal, PAL_SIZE);
}

inline void pal_set(const Uint16 bg_pal[256], const Uint16 obj_pal[256]){
  pal_set_bg(bg_pal);
  pal_set_obj(obj_pal);
}

inline void pal_set_bg_bank(Uint32 bank, const Uint16 pal[16]){
  memcpy(PAL_BANK_BG_MEM(bank), pal, PAL_BANK_SIZE);
}

inline void pal_set_obj_bank(Uint32 bank, const Uint16 pal[16]){
  memcpy(PAL_BANK_OBJ_MEM(bank), pal, PAL_BANK_SIZE);
}

inline void pal_get_bg(Uint16 pal[256]){
  memcpy(pal, PAL_BG_MEM, PAL_SIZE);
}

inline void pal_get_obj(Uint16 pal[256]){
  memcpy(pal, PAL_OBJ_MEM, PAL_SIZE);
}

inline void pal_get_bg_bank(Uint32 bank, Uint16 pal[16]){
  memcpy(pal, PAL_BANK_BG_MEM(bank), PAL_BANK_SIZE);
}

inline void pal_get_obj_bank(Uint32 bank, Uint16 pal[16]){
  memcpy(pal, PAL_BANK_OBJ_MEM(bank), PAL_BANK_SIZE);
}

inline void pal_get(Uint16 bg_pal[256], Uint16 obj_pal[256]){
  pal_get_bg(bg_pal);
  pal_get_obj(obj_pal);
}

#endif
