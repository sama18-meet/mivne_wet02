#include "Player.h"

Player::Player(int id, int score, Group* group) : id(id), score(score), lvl(START_LEVEL), group(group) {}

int Player::getId() const {
    return this->id;
}

int Player::getLvl() const {
    return this->lvl;
}

Group* Player::getGroup() const {
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

