#include <stdlib.h>

#pragma once
typedef struct player
{
    char *name;
    int hp;
    int p_class;
} Player;

Player *Player_init();
void player_ready(Player *player, char *new_name, int new_class);
void player_destroy(Player *player);