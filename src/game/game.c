// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#include <src/gbx.h>

void game_load();
void game_tick();
void game_draw();

const Scene game = SCENE(&game_load, &game_tick, &game_draw);

