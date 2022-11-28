//
// Created by shlom on 27/11/2022.
//
#include "player.h"
#include <iostream>
#include "worldcup23a1.h"
int main(){
    Player p(1,3,4,6, 5,true);
    Team team(2,4);
    world_cup_t wc(*new world_cup_t());
    wc.add_team(2, 6);

}
