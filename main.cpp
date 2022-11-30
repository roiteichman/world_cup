//
// Created by shlom on 27/11/2022.
//
#include "player.h"
#include <iostream>
#include "worldcup23a1.h"
int main(){
    shared_ptr<Player> p(new Player(1,3,4,6, 5,true));
    shared_ptr<Player> p1(new Player(2,3,4,6, 5,true));
    shared_ptr<Player> p2(new Player(3,3,4,6, 5,true));
    Team team(2,4);
    team.addPlayer(p);
    team.addPlayer(p1);
    team.addPlayer(p2);
    world_cup_t wc(*new world_cup_t());
    wc.add_team(2, 6);
    team.removePlayer(p1);
    shared_ptr<Player> p3(new Player(4,3,4,6, 5,true));
    team.addPlayer(p3);
    shared_ptr<Player> p4(new Player(5,3,4,6, 5,true));


}
