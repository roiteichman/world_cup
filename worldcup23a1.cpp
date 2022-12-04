#include "worldcup23a1.h"

const bool BY_STATS = true;
const bool BY_IDS = false;
const int VICTORY = 3;
const int DRAW = 1;
const int VALID_TEAM = 3;///not good!!!!!!

world_cup_t::world_cup_t(): m_numOfPlayes(0), m_topScorer(nullptr) , m_teams(* new AVLTree<shared_ptr<Team>>(BY_IDS)),
                            m_notEmptyTeams(* new AVLTree<shared_ptr<Team>>(BY_IDS)),
                            m_playersByID(* new AVLTree<shared_ptr<Player>>(BY_IDS)),
                            m_playersByStats(* new AVLTree<shared_ptr<Player>>(BY_STATS))
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

    // if this player already exist
    if (m_playersByID.getRoot() != nullptr){
        // checking if the player exist or the team does not exist
        if ((m_playersByID.findInt(m_playersByID.getRoot(), playerId) != nullptr) || (m_teams.findInt(m_teams.getRoot(), teamId) == nullptr)) {
            return StatusType::FAILURE;
        }
    }

    shared_ptr<Team> team_ptr;
    team_ptr= m_teams.findInt(m_teams.getRoot(), teamId)->getValue();


    shared_ptr<Player> player_ptr;

    try{
        // note that we enter the player thr decrease the amount of games that he played because we have field of games of the team and we will return them together when we want to know how much he play
        player_ptr = shared_ptr<Player>(new Player(playerId, teamId, gamesPlayed-team_ptr->getGamesPlayed(), goals, cards, goalKeeper));
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    // entering the player by ID
    try{
        m_playersByID.insert(player_ptr);
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
    // entering the player by Stats
    try{
        m_playersByStats.insert(player_ptr);
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    update_previous_next(player_ptr);

    // entering the player to the team
    try{
        team_ptr->addPlayer(player_ptr);
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    // checking if needed to push the team to the non empty team tree
    if (team_ptr->getNumOfPlayers()==1){
        // push it
        try{
            m_notEmptyTeams.insert(team_ptr);
        } catch (const bad_alloc& e){
            return StatusType::ALLOCATION_ERROR;
        }
    }

    update_top_scorer(player_ptr);
    m_numOfPlayes++;

    return StatusType::SUCCESS;
}

void world_cup_t::update_top_scorer(shared_ptr<Player> player) {

    // first player is the top scorer
    if (!m_topScorer){
        m_topScorer=player;
    }

    else if (((m_topScorer->getGoalsScored() == player->getGoalsScored()) && (m_topScorer->getID()<player->getID())) ||
    (m_topScorer->getGoalsScored()<player->getGoalsScored())){
        m_topScorer = player;
    }
}

void world_cup_t::update_previous_next(shared_ptr<Player> player_ptr) {
    AVLNode<shared_ptr<Player>>* newNode = m_playersByStats.find(m_playersByStats.getRoot(), player_ptr);
    m_playersByStats.findPrevious(newNode);
    m_playersByStats.findNext(newNode);
}


StatusType world_cup_t::add_player(shared_ptr<Player> player_ptr, shared_ptr<Team> team_ptr) {

    // entering the player by ID
    try{
        m_playersByID.insert(player_ptr);
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
    // entering the player by Stats
    try{
        m_playersByStats.insert(player_ptr);
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    update_previous_next(player_ptr);

    // entering the player to the team
    try{
        team_ptr->addPlayer(player_ptr);
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    // checking if needed to push the team to the non empty team tree
    if (team_ptr->getNumOfPlayers()==1){
        // push it
        try{
            m_notEmptyTeams.insert(team_ptr);
        } catch (const bad_alloc& e){
            return StatusType::ALLOCATION_ERROR;
        }
    }

    update_top_scorer(player_ptr);

    m_numOfPlayes++;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if(playerId<=0)
        return StatusType::INVALID_INPUT;
    // if playerPtr does not exist
    if (!m_playersByID.findInt(m_playersByID.getRoot(), playerId))
        return StatusType::FAILURE;
    // get pointer to the playerPtr
    shared_ptr<Player> playerPtr = m_playersByID.findInt(m_playersByID.getRoot(), playerId)->getValue();

    shared_ptr<Team> teamPtr = m_notEmptyTeams.findInt(m_notEmptyTeams.getRoot(), playerPtr->getTeamID())->getValue();

    m_playersByID.remove(m_playersByID.getRoot(), playerPtr);
    m_playersByStats.remove(m_playersByStats.getRoot(), playerPtr);

    // searching the playerPtr in the not empty teamPtr tree and remove the playerPtr from it
    teamPtr->removePlayer(playerPtr);

    // if it was the last player in team, remove the team from the non empty team tree
    if (teamPtr->getNumOfPlayers()==0){
        m_notEmptyTeams.remove(m_notEmptyTeams.getRoot(), teamPtr);
    }

    // update top scorer
    if (playerPtr==m_topScorer){
        shared_ptr<Player> closestLeft (playerPtr->getClosestLeft());
        m_topScorer = closestLeft;
    }

    m_numOfPlayes--;

    return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed, int scoredGoals, int cardsReceived)
{
    if (playerId<=0 || gamesPlayed<0 || scoredGoals<0 || cardsReceived<0){
        return StatusType::INVALID_INPUT;
    }
    if (!m_playersByID.findInt(m_playersByID.getRoot(), playerId)) {
        return StatusType::FAILURE;
    }

    shared_ptr<Player> player = m_playersByID.findInt(m_playersByID.getRoot(), playerId)->getValue();

    shared_ptr<Team> team = m_notEmptyTeams.findInt(m_notEmptyTeams.getRoot(), player->getTeamID())->getValue();

    this->remove_player(playerId);

    player->setGamePlayed(gamesPlayed);
    player->setGoals(scoredGoals);
    player->setCards(cardsReceived);

    return this->add_player(player, team);

}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1<=0 || teamId2 <=0 || teamId1==teamId2){
        return StatusType::INVALID_INPUT;
    }
    // checking if exist
    if (!(m_teams.findInt(m_teams.getRoot(), teamId1)) || !(m_teams.findInt(m_teams.getRoot(), teamId2))){
        return StatusType::FAILURE;
    }

    shared_ptr<Team> team1 = m_teams.findInt(m_teams.getRoot(), teamId1)->getValue();
    shared_ptr<Team> team2 = m_teams.findInt(m_teams.getRoot(), teamId2)->getValue();
    // checking if valid teams
    if (!((team1->getNumOfGoalKeepers()) && (team1->getNumOfPlayers()>=VALID_TEAM)) || !((team2->getNumOfGoalKeepers()) && (team1->getNumOfPlayers()>=VALID_TEAM))){
        return StatusType::FAILURE;
    }

    int statsTeam1 = team1->getPoints()+team1->getGoals()-team1->getCards();
    int statsTeam2 = team2->getPoints()+team2->getGoals()-team2->getCards();

    if (statsTeam1<statsTeam2){
        team2->setPoints(VICTORY);
    }
    else if(statsTeam1==statsTeam2){
        team1->setPoints(DRAW);
        team2->setPoints(DRAW);
    }
    else{
        team1->setPoints(VICTORY);
    }

    team1->increaseGamesPlayed();
    team2->increaseGamesPlayed();

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if(playerId<=0)
        return StatusType::INVALID_INPUT;
    if (!m_playersByID.findInt(m_playersByID.getRoot(), playerId))
        return StatusType::FAILURE;
    shared_ptr<Player> player = m_playersByID.findInt(m_playersByID.getRoot(), playerId)->getValue();
    shared_ptr<Team> team = m_notEmptyTeams.findInt(m_notEmptyTeams.getRoot(), player->getTeamID())->getValue();

    // adding the amount of games of the player with the amount of the team
    output_t<int> output(player->getGamesPlayed()+team->getGamesPlayed());
    return output;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId<=0){
        return StatusType::INVALID_INPUT;
    }
    // checking if exist
    if (!(m_teams.findInt(m_teams.getRoot(), teamId))){
        return StatusType::FAILURE;
    }
    shared_ptr<Team> team = m_teams.findInt(m_teams.getRoot(), teamId)->getValue();

    output_t<int> output(team->getPoints());
    return output;
}
/*
StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}
*/

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if (teamId==0){
        return StatusType::INVALID_INPUT;
    }
    if (teamId>0){
        // checking if team exist
        if (!(m_teams.findInt(m_teams.getRoot(), teamId))){
            return StatusType::FAILURE;
        }
        // checking if there are players in team
        else if (m_teams.findInt(m_teams.getRoot(), teamId)->getValue()->getNumOfPlayers()==0){
            return StatusType::FAILURE;
        }
        // return top scorer in teamID
        return m_teams.findInt(m_teams.getRoot(), teamId)->getValue()->getTopScorer()->getID();
    }
    // no players in system
    if (!m_topScorer){
        return StatusType::FAILURE;
    }

    return m_topScorer->getID();
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if (teamId==0){
        return StatusType::INVALID_INPUT;
    }
    if (teamId>0) {
        // checking if team exist
        if (!(m_teams.findInt(m_teams.getRoot(), teamId))) {
            return StatusType::FAILURE;
        }
        return m_teams.findInt(m_teams.getRoot(), teamId)->getValue()->getNumOfPlayers();
    }
    return m_numOfPlayes;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if (teamId==0 || output == NULL){
        return StatusType::INVALID_INPUT;
    }
    else if (teamId>0){
        // if the team exist and has player
        if (m_notEmptyTeams.findInt(m_notEmptyTeams.getRoot(), teamId)){
            shared_ptr<Team> team = m_notEmptyTeams.findInt(m_notEmptyTeams.getRoot(), teamId)->getValue();
            team->get_all_players(output);
            return StatusType::SUCCESS;
        }
    }
    else if (m_numOfPlayes){
        int i = 0;
        m_playersByStats.printInOrder(m_playersByStats.getRoot(), output, i);
        return StatusType::SUCCESS;
    }
    return StatusType::FAILURE;
}

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
