#include "player.h"

Player *Player_init()
{
    Player *player = malloc(sizeof(Player));
    player->name = "NULL";
    return player;
};

void player_ready(Player *player, char *new_name, int new_class)
{
    player->name = new_name;
    player->p_class = new_class;
}

void player_destroy(Player *player)
{
    free(player);
}