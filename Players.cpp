#include "Player.h"
#define START_LEVEL 0


Player::Player(int id, int score, int groupID) : id(id), score(score), lvl(START_LEVEL), groupID(groupID) {}

int Player::getId() const {
    return this->id;
}

int Player::getLvl() const {
    return this->lvl;
}

int Player::getGroup() const {
    return this->groupID;
}

int Player::getScore() const {
    return this->score;
}

void Player::setScore(int newScore) {
    this->score = newScore;
}

void Player::increaseLvl(int inc) {
    this->lvl += inc;
}


