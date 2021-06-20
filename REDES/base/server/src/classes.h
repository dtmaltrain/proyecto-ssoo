#include <stdlib.h>
#pragma once



typedef struct Character
{
    int hp;
    int id_class;
    int monster_or_player; //0 SI PLAYER | 1 SI MONSTER
    char* name; 
    int bonus;
    int brute;
    int toxin;
    int reprobate;
    int bleeds;
    int distracted;
    int jump;
    int hp_max;

} Character;

extern Character* monster;

Character *char_init();
void player_ready(Character* player, char* name, int p);
void player_destroy(Character* player);
void p_use_ability(Character* player, int n);
void m_use_ability();
void heal(Character* player, int bonus);
void hit(Character* atk, Character* chr, int dmg);
void monster_ready(Character* monster, int n);
void char_destroy(Character* chr);