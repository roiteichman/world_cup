//
// Created by shlom on 28/11/2022.
//

#ifndef MAIN_CPP_TEAM_H
#define MAIN_CPP_TEAM_H
#include <memory>
#include "Player.h"
#include "AVLTree.h"


using namespace std;

class Team{

private:
    int m_teamId;
    int m_points;
    int m_sumOfGoals;
    int m_sumOfCards;
    int m_numOfPlayers;
    int m_numOfGoalKeepers;
    int m_gamesPlayed;

    AVLTree<shared_ptr<Player>> m_teamPlayersByStats;
    AVLTree<shared_ptr<Player>> m_teamPlayersByIds;
    Player* m_topScorer;
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
    void setTeamPlayersByStats(const AVLTree<shared_ptr<Player>>& TeamPlayersByStats);
    void setTeamPlayersByIds(const AVLTree<shared_ptr<Player>>& TeamPlayersByIds);
    void setMNumOfPlayers(int mNumOfPlayers);
    void setClosestLeft (Team* left);
    void setClosestRight (Team* right);
    void setNumOfGoalKeepers(int mNumOfGoalKeepers);


    void removePlayer (shared_ptr<Player> player);
    void addPlayer (const shared_ptr<Player>& player);

    int getNumOfGoalKeepers() const;
    int size() const;
    Team* getClosestLeft() const;
    Team* getClosestRight() const;
    void get_all_players(int *const output);
    int getID () const;
    int getPoints () const;
    int getGoals () const;
    int getCards () const;
    Player* getTopScorer() const;
    int getGamesPlayed() const;

    void increaseGamesPlayed(int gamesPlayed);
    void update_top_scorer(shared_ptr<Player> player);
    bool isValid() const;



    AVLTree<shared_ptr<struct Player>> & getTeamPlayerByStats ();
    AVLTree<shared_ptr<struct Player>> & getTeamPlayerByIds ();



};
#endif //MAIN_CPP_TEAM_H
