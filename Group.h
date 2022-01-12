#ifndef WET2_CODE_GROUP_H
#define WET2_CODE_GROUP_H

#include "RankTree.h"

#define SCALE_MAX 200

class Group {
private:
    int id;
    RankTreeOPK playersTree;
    int numPlayers;
    int lvl0PlayersScores[SCALE_MAX];
    int numLvl0Players;

public:
    Group(int id);
    ~Group() = default;
    void swallow(Group* prey);
    void addPlayerOfLvl0(int score);
    void removePlayer(int lvl, int score);
    void increasePlayerLvl(int oldLvl, int newLvl, int score);
    void changePlayerScore(int lvl, int oldScore, int newScore);
    double getPercentOfPlayersWithScoreInBounds(int score, int lowerLvl, int higherLvl) const;
    bool averageHighestPlayerLevelByGroup(int m, double* res) const;
    bool getPlayersBound(int score, int m, int* lowerBound, int* higherBound) const;
};


#endif //WET2_CODE_GROUP_H
