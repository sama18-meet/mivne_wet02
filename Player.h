#ifndef _PLAYER_H
#define _PLAYER_H

#define START_LEVEL 0

#include "Group.h"

class Player {
private:
    int id;
    int score;
    int lvl;
    int group;
public:
    Player(int id, int score, int groupId);
    ~Player() = default;
    Player(const Player&) = default;
    int getId() const;
    int getLvl() const;
    int getGroupId() const;
    int getScore() const;
    void increaseLvl(int inc);
    void setScore(int newScore);
};

#endif // _PLAYER_H