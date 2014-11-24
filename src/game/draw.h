// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef CLEANSPACE_GAME_DRAW_H
#define CLEANSPACE_GAME_DRAW_H

#define FADEIN_DURATION         60

#define AVATAR_OBJ              OBJ_MEM[0]
#define AVATAR_FRAMES           4
#define AVATAR_TILE_INDEX       0

#define BOOST_OBJ_A             OBJ_MEM[1]
#define BOOST_FRAMES            4
#define BOOST_TILE_INDEX        256

#define WARNING_X               ((GFX_DISPLAY_WIDTH - 64) / 2)
#define WARNING_Y               8
#define WARNING_HIGH_TILE_INDEX 384
#define WARNING_LOW_TILE_INDEX  400
#define WARNING_OBJ             OBJ_MEM[2]

#define POWERBAR_TILE_INDEX     320
#define POWERBAR_OBJS           (&OBJ_MEM[3])

#define SCORE_TILE_INDEX        768
#define SCORE_OBJS              (&POWERBAR_OBJS[POWER_MAX_CELLS])

#define TRASH_TILE_INDEX        512
#define TRASH_OBJS              (&SCORE_OBJS[SCORE_SIZE])

Sint32 game_to_screen_x(Sint32 game_x, Uint32 radius);
Sint32 game_to_screen_y(Sint32 game_y, Uint32 radius);

extern Uint32 draw_planet_bg_x;
extern Uint32 draw_planet_bg_y;
extern Sint32 draw_fadein_ttl;

#endif
