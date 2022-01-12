#ifndef _PLAYER_H
#define _PLAYER_H

#define START_LEVEL 0

#include "Group.h"

class Player {
private:
    int id;
    int score;
    int lvl;
    Group* group;
public:
    Player(int id, int score, Group* group);
    ~Player() = default;
    Player(const Player&) = default;
    int getId() const;
    int getLvl() const;
    Group* getGroup() const;
    int getScore() const;
    void increaseLvl(int inc);
    void setScore(int newScore);
};

#endif // _PLAYER_H