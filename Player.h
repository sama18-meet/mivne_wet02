#ifndef _PLAYER_H
#define _PLAYER_H

class Player {
private:
    int id;
    int lvl;
    int groupID;
    int score;
public:
    Player(int id, int lvl, int groupID);
    ~Player() = default;
    int getId() const;
    int getLvl() const;
    int getGroup() const;
    int getScore() const;
    void increaseLvl(int inc);
    void setScore(int newScore);
};

#endif // _PLAYER_H
