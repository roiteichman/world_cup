//
// Created by shlom on 27/11/2022.
//

#include "player.h"

Player::Player(int playerId, shared_ptr<Team> team, int gamesPlayed, int scoredGoals, int CardsReceived, bool goalKeeper):
m_id(playerId), m_team(team) ,m_gamePlayed(gamesPlayed), m_goals(scoredGoals), m_cards(CardsReceived), m_goalKeeper(goalKeeper),
m_closest_left(nullptr), m_closest_right(nullptr), m_closest(nullptr)
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

void Player::setTeam(const shared_ptr<Team>& newTeam) {
    m_team->removePlayer(this);
    newTeam->addPlayer(this);
    m_team=newTeam;
}

void Player::setClosestLeft(const shared_ptr<Player>& left) {
    m_closest_left=left;
}

void Player::setClosestRight(const shared_ptr<Player> &right) {
    m_closest_right=right;
}

void Player::setClosest(const shared_ptr<Player> &closest) {
    m_closest=closest;
}

shared_ptr<Team> Player::getTeam() const {
    return m_team;
}

shared_ptr<Player> Player::getClosestLeft() const {
    return m_closest_left;
}

shared_ptr<Player> Player::getClosestRight() const {
    return m_closest_right;
}

shared_ptr<Player> Player::getClosest() const {
    return m_closest;
}

