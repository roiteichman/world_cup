// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef WORLDCUP23A1_H_
#define WORLDCUP23A1_H_

#include "wet1util.h"
#include <memory>
#include <cmath>
using namespace std;

#include "AVLTree.h"
#include "Player.h"
#include "Team.h"


struct FakeTeam{

    int m_id{};
    int m_stats{};

    FakeTeam();
    FakeTeam(int id, int stats);

    bool game(FakeTeam* other);
};


class world_cup_t {
private:
    int m_numOfPlayers;
    int m_numOfValidTeams;
    Player* m_topScorer;
    AVLTree<shared_ptr<Team>> m_teams;
    AVLTree<shared_ptr<Team>> m_notEmptyTeams;
    AVLTree<shared_ptr<Team>> m_validTeams;
    AVLTree<shared_ptr<Player>> m_playersByID;
    AVLTree<shared_ptr<Player>> m_playersByStats;



public:
    // <DO-NOT-MODIFY> {

    world_cup_t();
    virtual ~world_cup_t();


    StatusType add_team(int teamId, int points);

    StatusType remove_team(int teamId);

    StatusType add_player(int playerId, int teamId, int gamesPlayed,
                          int goals, int cards, bool goalKeeper);

    StatusType add_player(shared_ptr<Player> player_ptr, shared_ptr<Team> team_ptr);

    StatusType remove_player(int playerId);

    StatusType update_player_stats(int playerId, int gamesPlayed,
                                   int scoredGoals, int cardsReceived);

    void update_top_scorer(shared_ptr<Player> player);

    void update_previous_next_add_player(shared_ptr<Player> player_ptr);
    static void update_previous_next_remove_player(shared_ptr<Player> player_ptr);

    void update_previous_next_add_team(shared_ptr<Team> team_ptr);
    static void update_previous_next_remove_team(shared_ptr<Team> team_ptr);

    static void mergeArraysByStats(shared_ptr<Player>* arrTeam1ByStats, shared_ptr<Player>* arrTeam2ByStats, shared_ptr<Player>* arrUniteTeamByStats, int team1, int team2, int total);

    static void mergeArraysByIds(shared_ptr<Player>* arrTeam1ByIDs, shared_ptr<Player>* arrTeam2ByIDs, shared_ptr<Player>* arrUniteTeamByIDs, int team1, int team2, int total);

    static void updatePlayers(shared_ptr<Player>* arrTeamByStats, int uniteTeamID, int size, int teamsGame);

    StatusType play_match(int teamId1, int teamId2);

    output_t<int> get_num_played_games(int playerId);

    output_t<int> get_team_points(int teamId);

    StatusType unite_teams(int teamId1, int teamId2, int newTeamId);

    output_t<int> get_top_scorer(int teamId);

    output_t<int> get_all_players_count(int teamId);

    StatusType get_all_players(int teamId, int *const output);

    output_t<int> get_closest_player(int playerId, int teamId);

    int playSimulation(FakeTeam* teams, int size);

    output_t<int> knockout_winner(int minTeamId, int maxTeamId);

    // } </DO-NOT-MODIFY>

};


#endif // WORLDCUP23A1_H_
