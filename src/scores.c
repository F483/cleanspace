// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>
#include <src/scores.h>

#define LAST_NAME_RAM_INDEX   4
#define LIST_OFFSET           6

// ROM MEMORY FORMAT
// 1 list for each difficulty
// list of 8 entries per difficulty ordered by points
// BYTE 0 left Uint16 points byte
// BYTE 1 right Uint16 points byte
// BYTE n player name chars

Uint32 score_ram_index(Uint32 difficulty, Uint16 index){
  Uint32 list_index = HIGHSCORE_LIST_SIZE * HIGHSCORE_SIZE * difficulty;
  return LIST_OFFSET + list_index + HIGHSCORE_SIZE * index;
}

void score_write_points(Uint8 difficulty, Uint32 index, Uint16 points){
  Uint16 ram_index = score_ram_index(difficulty, index);
  sav_write16(ram_index, points);
}

Uint16 score_read_points(Uint8 difficulty, Uint32 index){
  Uint16 ram_index = score_ram_index(difficulty, index);
  return sav_read16(ram_index);
}

void score_write_name(Uint8 difficulty, Uint32 index, const char* name){
  Uint32 ram_index = score_ram_index(difficulty, index) + 2;
  for (Uint32 i = 0; i < HIGHSCORE_NAME_SIZE; i++){
    sav_write8(ram_index + i, (Uint8)name[i]);
  }
  // save ram index of last name saved
  sav_write16(LAST_NAME_RAM_INDEX, (Uint16)ram_index);
}

void score_read_name(Uint8 difficulty, Uint32 index, char* dest){
  Uint32 ram_index = score_ram_index(difficulty, index);
  for (Uint32 i = 0; i < HIGHSCORE_NAME_SIZE; i++){
    Uint8 c = sav_read8(ram_index + 2 + i);
    dest[i] = (c == 0) ? '-' : (char)c;
  }
  dest[HIGHSCORE_NAME_SIZE] = '\0';
}

void score_read(Uint8 difficulty, Uint32 index, 
        Uint16* dest_points, char* dest_name){
  *dest_points = score_read_points(difficulty, index);
  score_read_name(difficulty, index, dest_name);
}

Uint16 score_entries_gte(Uint8 difficulty, Uint16 points){
  Uint16 result = 0;
  for (Uint32 i = 0; i < HIGHSCORE_LIST_SIZE; i++){
    if (score_read_points(difficulty, i) >= points){
      result++;
    }
  }
  return result;
}

void score_copy(Uint8 difficulty, Uint32 src_index, Uint32 dest_index){
  Uint32 src_ram_index = score_ram_index(difficulty, src_index);
  Uint32 dest_ram_index = score_ram_index(difficulty, dest_index);
  for (Uint32 i = 0; i < HIGHSCORE_SIZE; i++){
    Uint8 c = sav_read8(src_ram_index + i);
    sav_write8(dest_ram_index + i, c);
  }
}

void score_save(Uint8 difficulty, Uint16 points, const char* name){
  Uint32 save_index = score_entries_gte(difficulty, points);
  if (save_index >= HIGHSCORE_LIST_SIZE)
    while(1); // bad arguments 
  Uint32 replace_index = HIGHSCORE_LIST_SIZE - 1;
  while (replace_index > save_index) {
    score_copy(difficulty, replace_index - 1, replace_index);
    replace_index--;
  }
  score_write_points(difficulty, save_index, points);
  score_write_name(difficulty, save_index, name);
} 

void score_read_last_name(char* dest){
  Uint16 ram_index = sav_read16(LAST_NAME_RAM_INDEX);
  Bool no_saves = ram_index == 0;
  for (Uint32 i = 0; i < HIGHSCORE_NAME_SIZE; i++){
    dest[i] = no_saves ? ' ' : sav_read8(ram_index + i);
  }
  dest[HIGHSCORE_NAME_SIZE] = '\0';
}
