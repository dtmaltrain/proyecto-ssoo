#include "classes.h"
#include <stdio.h>

Character *char_init()
{
    Character *chr = malloc(sizeof(Character));
    chr->name = "NULL";
    return chr;
};

Character* monster;

void monster_ready(Character* monster, int n){
    
    monster -> monster_or_player = 1; 
    monster -> id_class = n;
    if (n == 1)
    {
        monster -> name = "Great JagRuz";
        monster -> hp = 10000;
    }
    else if (n == 2)
    {
        monster -> name = "Ruzalos";
        monster -> hp = 20000;
    }
    else
    {
        monster -> name = "Ruiz, el Gemelo Malvado..";
        monster -> hp = 25000;
    }
    monster -> bleeds = 0;
    monster -> distracted = 0;
    monster -> jump = 0;
}

void player_ready(Character* player, char* name, int p){
    player -> monster_or_player = 0;
    player -> name = name;
    player -> id_class = p;
    if (p == 1)
    {
        player -> hp = 5000;
    }
    else if (p == 2)
    {
        player -> hp = 3000;
    }
    else
    {
        player -> hp = 2500;
    }
    player -> bonus = 0;
    player -> brute = 0;
    player -> bleeds = 0;
    player -> toxin = 0;
    player -> reprobate = 0;
    player -> distracted = 0;
}

void heal(Character* player, int bonus){
    player -> hp += bonus;
}

void hit(Character* monster, int dmg){
    monster -> hp -= dmg;
}

// void m_use_ability(){
//     if (monster -> m_class == 1)
//     {
//         // JagRuz

//         // random 50% 1, 50% 2
//         if (n == 1)
//         {
//             // random player
//             //take_dmg(player, 1000)
//         }
//         else
//         {
//             //for players
//             //take_dmg(player, 500)
//         }
        
//     }
//     else if (monster -> m_class == 2)
//     {
//         // ruzalos
//         // random 40% 1, 60% 2
//         // si jump = True, 100% 2
//         if (n == 1)
//         {
//             // random player
//             //take_dmg(player, 1500)
//             // monster -> jump = True
//         }
//         else
//         {
//             // monster -> jump = False
//             //if player toxin
//             //take_dmg(player, 400)
//             // else
//             //take_dmg(player, 500)
//             // player -> toxin = True
//         }
//     }
//     else
//     {   
//         //Ruiz, el Gemelo Malvado del Profesor Ruz
//         // random 40% 1, 20% 2, 40% 3
//         if (n == 1)
//         {
//             // copiar abilidad ???
//         }
//         else if (n == 2)
//         {
//             //player -> reprobate = True
//         }
//         else
//         {
//             // calcular dmg por rondas
//             //for players
//             //take_dmg(player, dmg)
//         }
//     }
    
// }


void char_destroy(Character* chr)
{
    free(chr);
}
