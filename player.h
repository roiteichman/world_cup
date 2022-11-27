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
    shared_ptr<Player> m_closest_left;
    shared_ptr<Player> m_closest_right;
    shared_ptr<Player> m_closest;

public:
    Player(int id, int gamesPlayed, int scoredGoals, int CardsReceived);
    ~Player() = default;
    Player (const Player& player) = delete;
    Player& operator = (const Player& player) = delete;
    bool operator< (const Player& other) const;
    void setGamePlayed (int gamesPlayed);
    void setGoals (int goals);
    void setCards (int cardsReceived);
    int getGamesPlayed () const;
    int getGoalsScored () const;
    int getCardsReceived () const;
    int getID() const;

};




#endif //WORLD_CUP_PLAYER_H
