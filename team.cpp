//
// Created by teich on 27/11/2022.
//

#include "team.h"

Team::Team(int teamId, int point):
    m_teamId(teamId), m_points(point), m_sumOfGoals(0), m_sumOfCards(0), m_numOfPlayers(0), m_hasGoalKeeper(false),
    m_teamPlayers(* new AVLTree<shared_ptr<Player>>), m_topScorer(nullptr)
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

int Team::getTeamId() const {
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

bool Team::operator<(const Team &other) const {
    return m_teamId<other.m_teamId;
}


void Team::removePlayer(const shared_ptr<Player> &player) {
    m_teamPlayers.remove(m_teamPlayers.getRoot(),player);
}

void Team::addPlayer(const shared_ptr<Player> &player) {
    m_teamPlayers.insert(player);
}

