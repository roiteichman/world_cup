//
// Created by teich on 27/11/2022.
//

#include "team.h"

Team::Team(int teamId, int point):
    m_teamId(teamId), m_points(point)
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
