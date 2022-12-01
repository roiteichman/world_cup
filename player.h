//
// Created by teich on 27/11/2022.
//


#ifndef WORLD_CUP_PLAYER_H
#define WORLD_CUP_PLAYER_H


#include <memory>
//include "team.h"

using namespace std;

class Player{
private:
    int m_id;
    int m_team;
    int m_gamePlayed;
    int m_goals;
    int m_cards;
    bool m_goalKeeper;
    shared_ptr<Player> m_closest_left;
    shared_ptr<Player> m_closest_right;
    shared_ptr<Player> m_closest;

public:
    Player(int playerId, int teamId, int gamesPlayed, int scoredGoals, int CardsReceived, bool goalKeeper);
    ~Player() = default;
    Player (const Player& player) = delete;
    Player& operator = (const Player& player) = delete;
    bool operator< (const Player& other) const;
    //void setTeam (const shared_ptr<Team>& newTeam);
    void setGamePlayed (int gamesPlayed);
    void setGoals (int goals);
    void setCards (int cardsReceived);
    void setClosestLeft (shared_ptr<Player> left);
    void setClosestRight (shared_ptr<Player> right);
    void setClosest (shared_ptr<Player> closest);


    int getID() const;
    //shared_ptr<Team> getTeam() const;
    int getGamesPlayed () const;
    int getGoalsScored () const;
    int getCardsReceived () const;
    shared_ptr<Player> getClosestLeft() const;
    shared_ptr<Player> getClosestRight() const;
    shared_ptr<Player> getClosest() const;
};




#endif //WORLD_CUP_PLAYER_H
