#ifndef WET2_CODE_PLAYERSMANAGER_H
#define WET2_CODE_PLAYERSMANAGER_H

#include "UnionFind.h"
//#include "RankTree.h"
#include "HashTable.h"
#include "Group.h"
#include "Player.h"
class PlayersManager {

private:
    int k;
    int scale;
    UnionFind<Group> uf;
    HashTable<Player*> playersArray;
    Group allPlayersGroup;

public:
    PlayersManager(int k, int scale);
    ~PlayersManager() = default;
    bool mergeGroups(int groupId1, int groupId2);
    bool addPlayer(int playerId, int groupId, int score);
    bool removePlayer(int playerId);
    bool increasePlayerIdLvl(int playerId, int lvlIncrease);
    bool changePlayerIdScore(int playerId, int newScore);
    bool getPercentOfPlayersWithScoreInBounds(int groupId, int score, int lowerLvl, int higherLvl, double* res);
    bool averageHighestPlayerLevelByGroup(int groupId, int m, double* res);
    bool getPlayersBound(int groupId, int score, int m, int* lowerBound, int* higherBound);
    int getK() { return k; };
    int getScale() { return scale; };


};


#endif //WET2_CODE_PLAYERSMANAGER_H
