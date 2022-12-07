//
// Created by teich on 27/11/2022.
//

#include "team.h"

const bool BY_STATS = true;
const bool BY_IDS = false;
const int VALID_TEAM = 3;
/// TODO: not good!!!!!!

Team::Team(int teamId, int point):
        m_teamId(teamId), m_points(point), m_sumOfGoals(0), m_sumOfCards(0), m_numOfPlayers(0), m_numOfGoalKeepers(0), m_gamesPlayed(0),
        m_teamPlayersByStats(* new AVLTree<shared_ptr<Player>>(BY_STATS)), m_teamPlayersByIds(* new AVLTree<shared_ptr<Player>>(BY_IDS)),
        m_topScorer(nullptr), m_closest_left(nullptr), m_closest_right(nullptr)
{}

void Team::setPoints(int points) {
    m_points += points;
}

void Team::setGoals(int goals) {
    m_sumOfGoals+=goals;
}

void Team::setCards(int cards) {
    m_sumOfCards+=cards;
}

int Team::getID() const {
    return m_teamId;
}

int Team::getPoints() const {
    return m_points;
}

int Team::getGoals() const {
    return m_sumOfGoals;
}

int Team::getCards() const {
    return m_sumOfCards;
}

int Team::getNumOfPlayers() const {
    return m_numOfPlayers;
}

bool Team::operator>(const Team &other) const {
    return m_teamId>other.m_teamId;
}

bool Team::operator<(const Team &other) const {
    return m_teamId<other.m_teamId;
}

void Team::removePlayer(const shared_ptr<Player> &player) {
    m_sumOfGoals -= player->getGoalsScored();
    m_sumOfCards -= player->getCardsReceived();
    m_numOfPlayers--;
    if (player->isGoalkeeper()){
        m_numOfGoalKeepers--;
    }


    AVLNode<shared_ptr<Player>>* playerNode = m_teamPlayersByStats.find(m_teamPlayersByStats.getRoot(), player);

    // update top scorer
    if (player==m_topScorer){
        // if the top scorer has a parent he is the new top scorer
        if (playerNode->getParent()){
            m_topScorer = (playerNode->getParent()->getValue());
        }
        else if (playerNode->getLeft()){
            m_topScorer=(playerNode->getLeft()->getValue());
        }
        else{
            m_topScorer = nullptr;
        }
    }

    m_teamPlayersByStats.remove(m_teamPlayersByStats.getRoot(), player);
    m_teamPlayersByIds.remove(m_teamPlayersByIds.getRoot(), player);

}

void Team::addPlayer(const shared_ptr<Player> &player) {
    m_teamPlayersByStats.insert(player);
    m_teamPlayersByIds.insert(player);
    m_numOfPlayers++;
    m_sumOfGoals += player->getGoalsScored();
    m_sumOfCards += player->getCardsReceived();
    if (player->isGoalkeeper()){
        m_numOfGoalKeepers++;
    }
    update_top_scorer(player);
}

void Team::setClosestLeft(Team* left) {
    m_closest_left=left;
}

void Team::setClosestRight(Team* right) {
    m_closest_right=right;
}


Team* Team::getClosestLeft() const {
    return m_closest_left;
}

Team* Team::getClosestRight() const {
    return m_closest_right;
}

AVLTree<shared_ptr<Player>> &Team::getTeamPlayerByStats() {
    return m_teamPlayersByStats;
}


AVLTree<shared_ptr<Player>> &Team::getTeamPlayerByIds(){
    return m_teamPlayersByIds;
}

int Team::getNumOfGoalKeepers() const {
    return m_numOfGoalKeepers;
}

int Team::getGamesPlayed() const {
    return m_gamesPlayed;
}

void Team::increaseGamesPlayed(int gamesPlayed) {
    m_gamesPlayed+=gamesPlayed;
}

void Team::update_top_scorer(shared_ptr<Player> player) {

    // first player is the top scorer
    if (!m_topScorer){
        m_topScorer=player;
    }

    else if (((m_topScorer->getGoalsScored() == player->getGoalsScored()) && (m_topScorer->getID()<player->getID())) ||
             (m_topScorer->getGoalsScored()<player->getGoalsScored())){
        m_topScorer = player;
    }
}

const shared_ptr<Player> &Team::getTopScorer() const {
    return m_topScorer;
}

void Team::get_all_players(int *const output) {

    int i = 0;
    m_teamPlayersByStats.printInOrder(m_teamPlayersByStats.getRoot(), output, i);
}

void Team::setTeamPlayersByStats(const AVLTree<shared_ptr<Player>>& TeamPlayersByStats) {
    m_teamPlayersByStats = TeamPlayersByStats;
}

void Team::setTeamPlayersByIds(const AVLTree<shared_ptr<Player>>& TeamPlayersByIds) {
    m_teamPlayersByIds = TeamPlayersByIds;
}

void Team::setMNumOfPlayers(int mNumOfPlayers) {
    m_numOfPlayers = mNumOfPlayers;
}

void Team::setMNumOfGoalKeepers(int mNumOfGoalKeepers) {
    m_numOfGoalKeepers += mNumOfGoalKeepers;
}

bool Team::isValid() const {
    return (m_numOfGoalKeepers) && (m_numOfPlayers>=VALID_TEAM);
}

