// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef CLEANSPACE_GAME_TICK_H
#define CLEANSPACE_GAME_TICK_H

#define AVATAR_RADIUS           16

typedef struct {
  Vec   pos;
  Vec   vec;
  Uint32  hit_ttl;
} Avatar;

typedef enum {
  TRASH_SMALL = 0,
  TRASH_BIG   = 1,
  POWER_CELL  = 2
} TrashType;

typedef struct {
  Vec     pos;
  Vec     vec;
  TrashType type;
} Trash;

typedef struct {
  Vec   pos;
  Vec   vec;
  Uint32  ttl;
  Uint32  index;
} Score;

#define GAME_H                  256

#define TRASH_SIZE              32
#define TRASH_REM_LEFT          48
#define TRASH_REM_TOP           (-16)
#define TRASH_REM_BOTTOM        ((GAME_H) + 16)

#define POWER_CELL_SIZE         8
#define POWER_MAX_CELLS         8
#define POWER_MAXIMUM           (POWER_CELL_SIZE * POWER_MAX_CELLS)

#define SCORE_TTL               60
#define SCORE_SIZE              4

#define BOOST_TIME              30
#define BOOST_ACCEL             0.025

#define WARNING_FLASH_DURATION  6 
#define WARNING_HIGH_BORDER     48
#define WARNING_LOW_BORDER      (GAME_H - 48)

extern Uint32     tick_difficulty;
extern Avatar     tick_avatar;
extern Uint32     tick_energie;
extern Uint16     tick_points;
extern Vec        tick_boost_vec;
extern Uint32     tick_boost_ttl;
extern Trash      tick_trash[TRASH_SIZE];
extern Score      tick_scores[SCORE_SIZE];
extern Uint32     tick_new_trash_x;

inline Uint32 trash_radius(Trash* trash){
  return trash->type == TRASH_BIG ? 16 : 8;
}

inline void trash_clear(Trash* trash){
  trash->pos =  vec_make_int(TRASH_REM_LEFT - 100, 0); // off screen
  trash->vec =  vec_make_int(0, 0);   // so it doesn't move
  trash->type = 0;
}

/**
 * returns true if players is near top or bottom of screen.
 * see WARNING_HIGH_BORDER and WARNING_LOW_BORDER
 */
Bool warn_player();

#endif
