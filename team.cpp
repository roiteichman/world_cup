//
// Created by teich on 27/11/2022.
//

#include "team.h"

const bool byStats = true;
const bool byIds = false;

Team::Team(int teamId, int point):
    m_teamId(teamId), m_points(point), m_sumOfGoals(0), m_sumOfCards(0), m_numOfPlayers(0), m_hasGoalKeeper(false),
    m_teamPlayersByStats(* new AVLTree<shared_ptr<Player>>(byStats)), m_teamPlayersByIds(* new AVLTree<shared_ptr<Player>>(byIds)),
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
    m_teamPlayersByStats.remove(m_teamPlayersByStats.getRoot(), player);
    m_teamPlayersByIds.remove(m_teamPlayersByIds.getRoot(), player);
}

void Team::addPlayer(const shared_ptr<Player> &player) {
    m_teamPlayersByStats.insert(player);
    m_teamPlayersByIds.insert(player);
    m_sumOfGoals += player->getGoalsScored();
    m_sumOfCards += player->getCardsReceived();
}

void Team::setClosestLeft(shared_ptr<Team> left) {
    m_closest_left=&(*(left));
}

void Team::setClosestRight(shared_ptr<Team> right) {
    m_closest_right=&(*(right));
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



