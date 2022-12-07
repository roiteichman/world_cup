//
// Created by shlom on 27/11/2022.
//
#include "player.h"
#include <iostream>
#include "worldcup23a1.h"
int main(){
    shared_ptr<Player> p(new Player(1,3,4,1, 5,true));
    shared_ptr<Player> p1(new Player(3,3,4,2, 5,true));
    shared_ptr<Player> p2(new Player(4,3,4,3, 5,true));
    shared_ptr<Player> p3(new Player(2,3,4,4, 5,true));

    shared_ptr<Player> pp [] = {p, p1, p2, p3};
    AVLTree<shared_ptr<Player>> ppp = new AVLTree<shared_ptr<Player>>(false);
    AVLNode<shared_ptr<Player>>* pppp= ppp.sortedArrayToAVL(pp, 0, 3);


    Team team(2,4);
    //Team team1(3,4);
    //Team team2(4,4);
    team.addPlayer(p);
    team.addPlayer(p1);
    team.addPlayer(p2);
    team.addPlayer(p3);


    world_cup_t wc(*new world_cup_t());
    wc.add_team(2, 6);
    wc.add_team(2, 4);
    wc.add_team(6,7);
    wc.add_team(8,9);
    wc.add_team(3,5);
    wc.add_player(1,3,4,1, 5,true);
    wc.add_player(2,3,4,2, 5,true);
    wc.add_player(3,3,4,3, 5,true);
    wc.add_player(4,3,4,4, 5,true);

    wc.remove_player(3);

    wc.add_player(111,2,4,10, 5,true);
    wc.add_player(222,2,4,13, 5,true);
    wc.add_player(333,2,4,1, 5,true);
    wc.add_player(444,2,4,4, 5,true);
    wc.play_match(2,3);
    wc.play_match(2,3);
    wc.unite_teams(2,3,2);

    output_t<int> a = wc.get_num_played_games(2);

    output_t<int> t = wc.get_closest_player(111, 3);

    output_t<int> point = wc.get_team_points(2);

    wc.update_player_stats(2, 3, 4, 5);

    output_t<int> topScorer2 = wc.get_top_scorer(3);
    output_t<int> topScorer = wc.get_top_scorer(-3);

    output_t<int> numOfPlayers = wc.get_all_players_count(2);
    output_t<int> numOfPlayersTotal = wc.get_all_players_count(-2);
    output_t<int> numOfPlayersFailure = wc.get_all_players_count(30);

    wc.remove_team(6);

    wc.remove_team(2);

    wc.remove_team(3);
    wc.remove_team(2);

    output_t<int> winner = wc.knockout_winner(2,3);


    // checking get all players
    output_t<int> count = wc.get_all_players_count(-2);
    // Allocate if okay
    int *out_mem = nullptr;
    if (count.status() == StatusType::SUCCESS && (count.ans() > 0)) {
        out_mem = new int[count.ans()];
        for (int i = 0; i < count.ans(); ++i) out_mem[i] = -1;
    }
    // Call function
    StatusType status = wc.get_all_players(-2, out_mem);
    if (status == StatusType::SUCCESS) {
        for (int i = 0; i < count.ans(); ++i)
        {
            cout << out_mem[i] << endl;
        }
    }
    delete[] out_mem;
    // finish checking it

    //team.removePlayer(p1);
    //team.addPlayer(p1);

}
