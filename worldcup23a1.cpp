#include "worldcup23a1.h"

const bool BY_STATS = true;
const bool BY_IDS = false;
const int VICTORY = 3;
const int DRAW = 1;

world_cup_t::world_cup_t(): m_numOfPlayes(0), m_topScorer(nullptr) , m_teams(* new AVLTree<shared_ptr<Team>>(BY_IDS)),
                            m_notEmptyTeams(* new AVLTree<shared_ptr<Team>>(BY_IDS)),
                            m_validTeams(* new AVLTree<shared_ptr<Team>>(BY_IDS)),
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
    m_notEmptyTeams.remove(m_notEmptyTeams.getRoot(), team);
    m_notEmptyTeams.remove(m_notEmptyTeams.getRoot(), team);

	return StatusType::SUCCESS;
}


StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    if(teamId<=0 || playerId<=0 || gamesPlayed<0 || goals<0 || cards<0){
        return StatusType::INVALID_INPUT;
    }


    // checking if the player exist or the team does not exist
    if ((m_playersByID.findInt(m_playersByID.getRoot(), playerId) != nullptr) || (m_teams.findInt(m_teams.getRoot(), teamId) == nullptr)) {
        return StatusType::FAILURE;
    }

    // there is no team in this ID in system
    if (!m_teams.findInt(m_teams.getRoot(), teamId)){
        return StatusType::FAILURE;
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

    update_previous_next_add_player(player_ptr);

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
    if (team_ptr->isValid() && !(m_validTeams.find(m_validTeams.getRoot(), team_ptr))) {
        m_validTeams.insert(team_ptr);
        update_previous_next_add_team(team_ptr);
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

void world_cup_t::update_previous_next_add_player(shared_ptr<Player> player_ptr) {
    AVLNode<shared_ptr<Player>>* newNode = m_playersByStats.find(m_playersByStats.getRoot(), player_ptr);
    m_playersByStats.findPrevious(newNode);
    m_playersByStats.findNext(newNode);
}

void world_cup_t::update_previous_next_remove_player(shared_ptr<Player> player_ptr) {

    if (player_ptr->getClosestRight()) {
        player_ptr->getClosestRight()->setClosestLeft(player_ptr->getClosestLeft());
        player_ptr->setClosestRight(nullptr);
    }
    if (player_ptr->getClosestLeft()) {
        player_ptr->getClosestLeft()->setClosestRight(player_ptr->getClosestRight());
        player_ptr->setClosestLeft(nullptr);
    }
}


void world_cup_t::update_previous_next_add_team(shared_ptr<Team> team_ptr) {
    AVLNode<shared_ptr<Team>>* newNode = m_validTeams.find(m_validTeams.getRoot(), team_ptr);
    m_validTeams.findPrevious(newNode);
    m_validTeams.findNext(newNode);
}

void world_cup_t::update_previous_next_remove_team(shared_ptr<Team> team_ptr) {

    if (team_ptr->getClosestRight()) {
        team_ptr->getClosestRight()->setClosestLeft(team_ptr->getClosestLeft());
        team_ptr->setClosestRight(nullptr);
    }
    if (team_ptr->getClosestLeft()) {
        team_ptr->getClosestLeft()->setClosestRight(team_ptr->getClosestRight());
        team_ptr->setClosestLeft(nullptr);
    }
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

    update_previous_next_add_player(player_ptr);

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
    // if the team is valid and not in the valid teams tree add her into it
    if (team_ptr->isValid() && !(m_validTeams.find(m_validTeams.getRoot(), team_ptr))) {
        m_validTeams.insert(team_ptr);
        update_previous_next_add_team(team_ptr);
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

    update_previous_next_remove_player(playerPtr);

    m_playersByID.remove(m_playersByID.getRoot(), playerPtr);
    m_playersByStats.remove(m_playersByStats.getRoot(), playerPtr);

    // searching the playerPtr in the not empty teamPtr tree and remove the playerPtr from it
    teamPtr->removePlayer(playerPtr);
    if (!teamPtr->isValid()) {
        m_validTeams.remove(m_validTeams.getRoot(), teamPtr);
        update_previous_next_remove_team(teamPtr);
    }

    // if it was the last player in team, remove the team from the non empty team tree
    if (teamPtr->getNumOfPlayers()==0){
        m_notEmptyTeams.remove(m_notEmptyTeams.getRoot(), teamPtr);
    }

    // update top scorer + closest right of the next one
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
    if (!team1->isValid() || !team2->isValid()){
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

    team1->increaseGamesPlayed(1);
    team2->increaseGamesPlayed(1);

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

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{

    if (newTeamId<=0 || teamId1<=0 || teamId2<=0 || teamId1==teamId2){
        return StatusType::INVALID_INPUT;
    }
    // if team 1 or 2 does not exist
    if (!m_teams.findInt(m_teams.getRoot(), teamId1) || !m_teams.findInt(m_teams.getRoot(), teamId2)){
        return StatusType::FAILURE;
    }
    // check if there is a team with newTeamId that are not one of the two others
    if (m_teams.findInt(m_teams.getRoot(), newTeamId)){
        if (m_teams.findInt(m_teams.getRoot(), newTeamId) != m_teams.findInt(m_teams.getRoot(), teamId1) &&
            m_teams.findInt(m_teams.getRoot(), newTeamId) != m_teams.findInt(m_teams.getRoot(), teamId2) ){
            return StatusType::FAILURE;
        }
    }

    // get the teams to unite
    shared_ptr<Team> team1 = m_teams.findInt(m_teams.getRoot(), teamId1)->getValue();
    shared_ptr<Team> team2 = m_teams.findInt(m_teams.getRoot(), teamId2)->getValue();

    // remove the teams from World Cup trees
    m_teams.remove(m_teams.getRoot(), team1);
    m_notEmptyTeams.remove(m_notEmptyTeams.getRoot(), team1);
    m_validTeams.remove(m_validTeams.getRoot(), team1);

    if(team1->isValid()){
        update_previous_next_remove_team(team1);
    }

    m_teams.remove(m_teams.getRoot(), team2);
    m_notEmptyTeams.remove(m_notEmptyTeams.getRoot(), team2);
    m_validTeams.remove(m_validTeams.getRoot(), team2);

    if(team2->isValid()) {
        update_previous_next_remove_team(team2);
    }

    int sumOfPlayersTotal = team1->getNumOfPlayers() + team2->getNumOfPlayers();

    // allocate arrays
    shared_ptr<Player>* arrTeam1ByStats;
    shared_ptr<Player>* arrTeam1ByIDs;
    shared_ptr<Player>* arrTeam2ByStats;
    shared_ptr<Player>* arrTeam2ByIDs;
    // allocate new arrays for the unite team
    shared_ptr<Player>* arrUniteTeamByStats;
    shared_ptr<Player>* arrUniteTeamByIDs;

    try{
        arrTeam1ByStats = new shared_ptr<Player>[team1->getNumOfPlayers()];
        arrTeam1ByIDs = new shared_ptr<Player>[team1->getNumOfPlayers()];
        arrTeam2ByStats = new shared_ptr<Player>[team2->getNumOfPlayers()];
        arrTeam2ByIDs = new shared_ptr<Player>[team2->getNumOfPlayers()];
        arrUniteTeamByStats = new shared_ptr<Player>[sumOfPlayersTotal];
        arrUniteTeamByIDs = new shared_ptr<Player>[sumOfPlayersTotal];

    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    // fill the arrays with the teams inOrder
    int i = 0, k=0;
    team1->getTeamPlayerByStats().printInOrderT(team1->getTeamPlayerByStats().getRoot(), arrTeam1ByStats, i);
    team1->getTeamPlayerByIds().printInOrderT(team1->getTeamPlayerByIds().getRoot(), arrTeam1ByIDs, k);

    i=0, k=0;
    team2->getTeamPlayerByStats().printInOrderT(team2->getTeamPlayerByStats().getRoot(), arrTeam2ByStats, i);
    team2->getTeamPlayerByIds().printInOrderT(team2->getTeamPlayerByIds().getRoot(), arrTeam2ByIDs, k);

    // update players detail: Games played + teamId
    updatePlayers(arrTeam1ByStats, newTeamId, team1->getNumOfPlayers(), team1->getGamesPlayed());
    updatePlayers(arrTeam2ByStats, newTeamId, team2->getNumOfPlayers(), team2->getGamesPlayed());



    // merging the teams once by Stats and next time by Ids
    mergeArraysByStats(arrTeam1ByStats, arrTeam2ByStats, arrUniteTeamByStats, team1->getNumOfPlayers(),
                       team2->getNumOfPlayers(), sumOfPlayersTotal);
    mergeArraysByIds(arrTeam1ByIDs, arrTeam2ByIDs, arrUniteTeamByIDs, team1->getNumOfPlayers(),
                     team2->getNumOfPlayers(), sumOfPlayersTotal);


    AVLNode<shared_ptr<Player>>* nodeUniteTeamByStats = m_playersByStats.sortedArrayToBST(arrUniteTeamByStats, 0, sumOfPlayersTotal - 1);
    AVLNode<shared_ptr<Player>>* nodeUniteTeamByIDs = m_playersByStats.sortedArrayToBST(arrUniteTeamByIDs, 0, sumOfPlayersTotal - 1);


    AVLTree<shared_ptr<Player>>* unitedTeamByStats;
    AVLTree<shared_ptr<Player>>* unitedTeamById;

    try {
        unitedTeamByStats = (&(* new AVLTree<shared_ptr<Player>>(true, nodeUniteTeamByStats)));
        unitedTeamById = (&(* new AVLTree<shared_ptr<Player>>(false, nodeUniteTeamByIDs)));
    } catch (const bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    // create new Team
    /// we have a problem with try & catch right here
    shared_ptr<Team> newTeam (new Team(newTeamId, team1->getPoints()+team2->getPoints()));

    // update or the other field in newTeam
    newTeam->setTeamPlayersByIds(*unitedTeamById);
    newTeam->setTeamPlayersByStats(*unitedTeamByStats);
    newTeam->setMNumOfPlayers(team1->getNumOfGoalKeepers()+team2->getNumOfGoalKeepers());
    newTeam->setCards(team1->getCards()+team2->getCards());
    newTeam->setGoals(team1->getGoals()+team2->getGoals());
    newTeam->increaseGamesPlayed(team1->getGamesPlayed()+team2->getGamesPlayed());
    newTeam->setMNumOfGoalKeepers(team1->getNumOfGoalKeepers()+team2->getNumOfGoalKeepers());

    m_teams.insert(newTeam);

    if (newTeam->getNumOfPlayers()){
        m_notEmptyTeams.insert(newTeam);
    }

	return StatusType::SUCCESS;
}

void world_cup_t::updatePlayers(shared_ptr<Player> *arrTeamByStats, int uniteTeamID, int size, int teamsGame) {
    for (int j = 0; j < size; ++j) {
        arrTeamByStats[j]->setGamePlayed(teamsGame);
        arrTeamByStats[j]->setTeam(uniteTeamID);
    }
}

void world_cup_t::mergeArraysByStats(shared_ptr<Player> *arrTeam1ByStats, shared_ptr<Player> *arrTeam2ByStats,
                                     shared_ptr<Player> *arrUniteTeamByStats, int team1, int team2, int total) {
    int i=0, k=0;
    for (int j = 0; j < total; ++j) {
        if (i != team1 && k != team2) {
            if (*arrTeam1ByStats[i] < *arrTeam2ByStats[k]) {
                arrUniteTeamByStats[j] = arrTeam1ByStats[i];
                i++;
            } else {
                arrUniteTeamByStats[j] = arrTeam2ByStats[k];
                k++;
            }
        } else if (i == team1) {
            arrUniteTeamByStats[j] = arrTeam2ByStats[k];
            k++;
        } else {
            arrUniteTeamByStats[j] = arrTeam1ByStats[i];
            i++;
        }
    }
}

    void world_cup_t::mergeArraysByIds(shared_ptr<Player> *arrTeam1ByIDs, shared_ptr<Player> *arrTeam2ByIDs,
                                       shared_ptr<Player> *arrUniteTeamByIDs, int team1, int team2, int total) {
        int i=0, k=0;
        for (int j = 0; j < total; ++j) {
            if (i != team1 && k != team2) {
                if (*arrTeam1ByIDs[i] > *arrTeam2ByIDs[k]) {
                    arrUniteTeamByIDs[j] = arrTeam2ByIDs[k];
                    k++;
                } else {
                    arrUniteTeamByIDs[j] = arrTeam1ByIDs[i];
                    i++;
                }
            } else if (i == team1) {
                arrUniteTeamByIDs[j] = arrTeam2ByIDs[k];
                k++;
            } else {
                arrUniteTeamByIDs[j] = arrTeam1ByIDs[i];
                i++;
            }
        }
    }


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

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    /// adding check

    shared_ptr<Team> minIdTeam = m_teams.findInt(m_teams.getRoot(), minTeamId)->getValue();
    shared_ptr<Team> maxIdTeam = m_teams.findInt(m_teams.getRoot(), maxTeamId)->getValue();
    shared_ptr<Team> temp = minIdTeam;


    while (temp!=maxIdTeam){

    }



}

