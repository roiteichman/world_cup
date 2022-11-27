//
// Created by teich on 27/11/2022.
//

#ifndef WORLD_CUP_PLAYER_H
#define WORLD_CUP_PLAYER_H

#include "team.h"
#include <memory>
using namespace std;

class Player{
private:
    int m_id;
    int m_gamePlayed;
    int m_goals;
    int m_cards;
    bool m_goalKeeper;
    shared_ptr<Team> m_team;



public:
    Player(int id, int gamesPlayed, int scoredGoals, int CardsReceived);
    ~Player() = default;
    Player (const Player& player) = delete;
    Player& opertor= (const Player& player) = delete;



};





#endif //WORLD_CUP_PLAYER_H
