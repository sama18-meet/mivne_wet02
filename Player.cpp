#include "Player.h"

Player::Player(int id, int score, int groupId) : id(id), score(score), lvl(START_LEVEL), group(groupId) {}

int Player::getId() const {
    return this->id;
}

int Player::getLvl() const {
    return this->lvl;
}

int Player::getGroupId() const {
    return this->group;
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

