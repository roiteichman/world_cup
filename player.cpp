//
// Created by shlom on 27/11/2022.
//

#include "player.h"
#include <cstdlib>


Player::Player(int playerId, int teamId, int gamesPlayed, int scoredGoals, int CardsReceived, bool goalKeeper):
m_id(playerId), m_team(teamId), m_gamePlayed(gamesPlayed), m_goals(scoredGoals), m_cards(CardsReceived), m_goalKeeper(goalKeeper),
m_closest_left(NULL), m_closest_right(NULL), m_closest(NULL)
{}

bool Player::operator<(const Player &other) const {
    if(this->m_goals<other.getGoalsScored())
        return true;
    if(this->m_goals==other.getGoalsScored()) {
        if (this->m_cards > other.getCardsReceived())
            return true;
        if (this->m_cards == other.getCardsReceived())
            if (this->m_id < other.getID())
                return true;
    }
    return false;
}

bool Player::operator>(const Player &other) const {
    return m_id>other.getID();
}


void Player::setGamePlayed(int gamesPlayed) {
    m_gamePlayed+=gamesPlayed;
}

void Player::setGoals(int goals) {
    m_goals+=goals;
}

void Player::setCards(int cardsReceived) {
    m_cards+=cardsReceived;
}

int Player::getGamesPlayed() const {
    return m_gamePlayed;
}

int Player::getGoalsScored() const {
    return m_goals;
}

int Player::getCardsReceived() const {
    return m_cards;
}

int Player::getID() const {
    return m_id;
}

/*void Player::setTeam(const shared_ptr<Team>& newTeam) {
    m_team->removePlayer(this);
    newTeam->addPlayer(this);
    m_team=newTeam;
}*/

void Player::setClosestLeft(shared_ptr<Player> left) {
    m_closest_left=left;
}

void Player::setClosestRight(shared_ptr<Player> right) {
    m_closest_right=right;
}



shared_ptr<Player> Player::getClosestRight() const {
    return m_closest_right;
}

shared_ptr<Player> Player::getClosestLeft() const {
    return m_closest;
}


/*shared_ptr<Team> Player::getTeam() const {
    return m_team;
}*/

shared_ptr<Player> Player::getClosest() const {
    int distanceGoalsLeft = abs(m_goals - m_closest_left->getGoalsScored());
    int distanceGoalsRight = abs(m_closest_right->getGoalsScored() - m_goals);
    int distanceCardsLeft = abs(m_cards - m_closest_left->getCardsReceived());
    int distanceCardsRight = abs(m_closest_right->getCardsReceived() - m_cards);
    int distanceIdsLeft = abs(m_id - m_closest_left->getID());
    int distanceIdsRight = abs(m_closest_right->getID()-m_id);

    if(distanceGoalsLeft < distanceGoalsRight)
        return m_closest_left;
    if(distanceGoalsLeft == distanceGoalsRight) {
        if (distanceCardsLeft < distanceCardsRight)
            return m_closest_left;
        if (distanceCardsLeft == distanceCardsRight)
            if (distanceIdsLeft < distanceIdsRight)
                return m_closest_left;
    }
    return m_closest_right;
}


