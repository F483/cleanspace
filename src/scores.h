// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef CLEANSPACE_HIGHSCORES_H
#define CLEANSPACE_HIGHSCORES_H

#define HIGHSCORE_LIST_SIZE   8
#define HIGHSCORE_NAME_SIZE   12
#define HIGHSCORE_SIZE        (HIGHSCORE_NAME_SIZE + 2) // +2 points

/**
 * Makes sure cart rom is properly initialized.
 */
void score_init();

/** 
 * returns number of entries >= points for given difficulty
 */
Uint16 score_entries_gte(Uint8 difficulty, Uint16 points);

/**
 * @param dest_name size must be HIGHSCORE_NAME_SIZE + 1 chars (null terminated)
 */
void score_read(Uint8 difficulty, Uint32 index, 
                Uint16* dest_points, char* dest_name);

void score_save(Uint8 difficulty, Uint16 points, const char* name);

void score_read_last_name(char* dest); // filled with spaces if no saves

#endif
