#include "worldcup23a1.h"

const bool byStats = true;
const bool byIds = false;

world_cup_t::world_cup_t(): m_numOfPlayes(0), m_topScorer(nullptr) ,m_teams(* new AVLTree<shared_ptr<Team>>(byIds)), m_players(* new AVLTree<shared_ptr<Player>>(byStats))
{}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}


StatusType world_cup_t::add_team(int teamId, int points)
{
	// TODO: Your code goes here
	if(teamId<=0 || points<0){
		return StatusType::INVALID_INPUT;
	}

	shared_ptr<Team> team_ptr;

	try{
		team_ptr = shared_ptr<Team>(new Team(teamId, points));
	} catch (const bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
		}

	if (m_teams.getRoot() != NULL){
		if (m_teams.find(m_teams.getRoot(), team_ptr) != NULL) {
			return StatusType::FAILURE;
		}
	}

	try{
        m_teams.insert(team_ptr);
	} catch (const bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}

	return StatusType::SUCCESS;
}
/*
StatusType world_cup_t::remove_team(int teamId)
{
	// TODO: Your code goes here
	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;

 // send to the Player class as a pointer to Team
}


StatusType world_cup_t::remove_player(int playerId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}
*/
output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
    shared_ptr<Team> tempTeam = m_teams.findInt(m_teams.getRoot(), teamId)->getValue();
    AVLTree<shared_ptr<Player>> treeOfTempTeam = tempTeam->getTeamPlayerByIds();
    shared_ptr<Player> tempPlayer = treeOfTempTeam.findInt(treeOfTempTeam.getRoot() , playerId)->getValue();

    return output_t<int>(tempPlayer->getClosest()->getID());

}
/*
output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}
*/
