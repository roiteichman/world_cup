
#define CATCH_CONFIG_MAIN

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
//#include "catch.hpp"
#include <stdlib.h>
#include "../worldcup23a1.h"


int main(){
    world_cup_t *oj = new world_cup_t();
    int playerId = 1;
    StatusType res;
    for (int teamId = 1; teamId < 10; teamId += 2)
    {
        res = oj->add_team(teamId, 1000 / ((6 - teamId) * (6 - teamId)));

        for (int startingId = playerId; playerId < startingId + 10 + teamId; ++playerId)
        {
            res = oj->add_player(playerId, teamId, 1, playerId, 3, true);
        }
        ++playerId;
    }
    // Strengths team1:73 team3:319 team5:1485 team7:1816 team9:1384

    res = oj->add_team(4, 1000000);

    output_t<int> res1 = oj->knockout_winner(8, 9);

    output_t<int> res2 = oj->knockout_winner(1, 1);

    output_t<int> res3 = oj->knockout_winner(2, 4);

    output_t<int> res4 = oj->knockout_winner(3, 9);
    output_t<int> res6 = oj->knockout_winner(0, 5);

    output_t<int> res7 = oj->knockout_winner(1, 9);

    res = oj->add_player(999, 3, 1, 3000, 200, false);
    // Strengths team1:73 team3:3119 team5:1485 team7:1816 team9:1384

    output_t<int> res8 = oj->knockout_winner(1, 999);
}
