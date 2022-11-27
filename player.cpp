//
// Created by shlom on 27/11/2022.
//

#include "player.h"

Player::Player(int id, int gamesPlayed, int scoredGoals, int CardsReceived):
m_id(id), m_gamePlayed(gamesPlayed), m_goals(scoredGoals), m_cards(CardsReceived) {
}


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
