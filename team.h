//
// Created by shlom on 28/11/2022.
//

#ifndef MAIN_CPP_TEAM_H
#define MAIN_CPP_TEAM_H
#include <memory>
#include "player.h"
#include "avl_tree.h"


using namespace std;

class Team{

private:
    int m_teamId;
    int m_points;
    int m_sumOfGoals;
    int m_sumOfCards;
    int m_numOfPlayers;
    bool m_hasGoalKeeper;
    AVLTree<shared_ptr<Player>> m_teamPlayersByStats;
    AVLTree<shared_ptr<Player>> m_teamPlayersByIds;
    shared_ptr<Player> m_topScorer;
    Team* m_closest_left;
    Team* m_closest_right;

public:
    explicit Team(int teamId, int point=0);
    ~Team() = default;
    Team (const Team& team) = delete;
    Team& operator = (const Team& team) = delete;

    bool operator> (const Team& other) const;
    bool operator< (const Team& other) const;


    void setPoints (int points);
    void setGoals (int goals);
    void setCards (int cards);
    void removePlayer (const shared_ptr<Player>& player);
    void addPlayer (const shared_ptr<Player>& player);
    void setClosestLeft (shared_ptr<Team> left);
    void setClosestRight (shared_ptr<Team> right);
    Team* getClosestLeft() const;
    Team* getClosestRight() const;
    AVLTree<shared_ptr<struct Player>> & getTeamPlayerByStats ();
    AVLTree<shared_ptr<struct Player>> & getTeamPlayerByIds ();

    int getID () const;
    int getPoints () const;
    int getGoals () const;
    int getCards () const;
    int getNumOfPlayers () const;

};
#endif //MAIN_CPP_TEAM_H
