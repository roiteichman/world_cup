#include "worldcup23a1.h"

const bool byStats = true;
const bool byIds = false;

world_cup_t::world_cup_t(): m_numOfPlayes(0), m_topScorer(nullptr) , m_teams(* new AVLTree<shared_ptr<Team>>(byIds)), m_playersByID(* new AVLTree<shared_ptr<Player>>(byIds)),
                            m_playersByStats(* new AVLTree<shared_ptr<Player>>(byStats))
{}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId<=0 || points<0){
		return StatusType::INVALID_INPUT;
	}

	shared_ptr<Team> team_ptr;

	try{
		team_ptr = shared_ptr<Team>(new Team(teamId, points));
	} catch (const bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
		}

	// if this team already exist
    if (m_teams.getRoot() != nullptr){
		if (m_teams.findInt(m_teams.getRoot(), teamId) != nullptr) {
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

StatusType world_cup_t::remove_team(int teamId)
{
	if(teamId<=0)
        return StatusType::INVALID_INPUT;
    // checking if the team exist and not empty
    if (!m_teams.findInt(m_teams.getRoot(), teamId))
        return StatusType::FAILURE;
    shared_ptr<Team> team = m_teams.findInt(m_teams.getRoot(), teamId)->getValue();
    if (team->getTeamPlayerByIds().getRoot())
        return StatusType::FAILURE;
    m_teams.remove(m_teams.getRoot(), team);

	return StatusType::SUCCESS;
}


StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    if(teamId<=0 || playerId<=0 || gamesPlayed<0 || goals<0 || cards<0){
        return StatusType::INVALID_INPUT;
    }

    shared_ptr<Player> player_ptr;

    try{
        player_ptr = shared_ptr<Player>(new Player(playerId, teamId, gamesPlayed, goals, cards, goalKeeper));
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    // if this player already exist
    if (m_playersByID.getRoot() != nullptr){
        // checking if the player exist or the team does not exist
        if ((m_playersByID.findInt(m_playersByID.getRoot(), playerId) != nullptr) || (m_teams.findInt(m_teams.getRoot(), teamId) == nullptr)) {
            return StatusType::FAILURE;
        }
    }

    try{
        m_playersByID.insert(player_ptr);
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        m_playersByStats.insert(player_ptr);
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        m_teams.findInt(m_teams.getRoot() ,teamId)->getValue()->addPlayer(player_ptr);
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if(playerId<=0)
        return StatusType::INVALID_INPUT;
    if (!m_playersByID.findInt(m_playersByID.getRoot(), playerId))
        return StatusType::FAILURE;
    shared_ptr<Player> player = m_playersByID.findInt(m_playersByID.getRoot(), playerId)->getValue();
	m_playersByID.remove(m_playersByID.getRoot(), player);
    m_playersByStats.remove(m_playersByStats.getRoot(), player);

    m_teams.findInt(m_teams.getRoot(), player->getTeamID())->getValue()->removePlayer(player);

	return StatusType::SUCCESS;

}
/*
StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}
*/
StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    // TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if(playerId<=0)
        return StatusType::INVALID_INPUT;
    if (!m_playersByID.findInt(m_playersByID.getRoot(), playerId))
        return StatusType::FAILURE;
    shared_ptr<Player> player = m_playersByID.findInt(m_playersByID.getRoot(), playerId)->getValue();
    return output_t<int>(player->getGamesPlayed());
}
/*
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
    if (playerId<=0 || teamId<=0){
        output_t<int> output(StatusType::INVALID_INPUT);
        return output;
    }
    // check if the team exist
    if(m_teams.findInt(m_teams.getRoot(), teamId)){
        // get the team
        Team* tempTeam = &(*(m_teams.findInt(m_teams.getRoot(), teamId)->getValue()));
        // get the tree of the team
        //AVLTree<shared_ptr<Player>> treeOfTempTeam = tempTeam->getTeamPlayerByIds();
        // check that the player exist
        if (tempTeam->getTeamPlayerByIds().findInt(tempTeam->getTeamPlayerByIds().getRoot() , playerId)){
            // get the player
            Player* tempPlayer = &(*(tempTeam->getTeamPlayerByIds().findInt(tempTeam->getTeamPlayerByIds().getRoot() , playerId)->getValue()));
            // if its not the only player in System
            if (tempPlayer->getClosest()){
                output_t<int> output(tempPlayer->getClosest()->getID());
                return output;
            }
        }
    }
    output_t<int> output(StatusType::FAILURE);
    return output;

}
/*
output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}
*/
