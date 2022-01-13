#include "PlayersManager.h"

PlayersManager::PlayersManager(int k, int scale) : k(k), scale(scale), uf(k), playersArray(), allPlayersGroup(0) {}

bool PlayersManager::mergeGroups(int groupId1, int groupId2) {
    // todo: check if no need to merge. this might happen internally (in uf & merge).
    Group* group1 = uf.find(groupId1);
    Group* group2 = uf.find(groupId2);
    if (group1 == group2) {
        return true;
    }
    int newId = uf.unite(group1->getId(), group2->getId());
    if (newId == group1->getId()) {
        group1->swallow(group2);
    }
    else {
        assert(newId == group2->getId());
        group2->swallow(group1);
    }
    return true;
}

bool PlayersManager::addPlayer(int playerId, int groupId, int score) {
    if (playersArray[playerId] != nullptr) {
        return false;
    }
    Group* group = uf.find(groupId);
    Player* p = new Player(playerId, score, group);
    playersArray.insert(playerId, p);
    group->addPlayerOfLvl0(score);
    allPlayersGroup.addPlayerOfLvl0(score);
    return true;
}

bool PlayersManager::removePlayer(int playerId) {
    Player* p = playersArray[playerId];
    if (p == nullptr) {
        return false;
    }
    int lvl = p->getLvl();
    int score = p->getScore();
    Group* g = p->getGroup();
    g->removePlayer(lvl, score);
    allPlayersGroup.removePlayer(lvl, score);
    playersArray.remove(playerId);
    delete p;
    return true;
}

bool PlayersManager::increasePlayerIdLvl(int playerId, int lvlIncrease) {
    Player* p = playersArray[playerId];
    if (p == nullptr) {
        return false;
    }
    int oldLvl = p->getLvl();
    int newLvl = oldLvl + lvlIncrease;
    int score = p->getScore();
    Group* g = p->getGroup();
    g->increasePlayerLvl(oldLvl, newLvl, score);
    allPlayersGroup.increasePlayerLvl(oldLvl, newLvl, score);
    p->increaseLvl(lvlIncrease);
    return true;
}

bool PlayersManager::changePlayerIdScore(int playerId, int newScore) {
    Player* p = playersArray[playerId];
    if (p == nullptr) {
        return false;
    }
    int oldScore = p->getScore();
    int lvl = p->getLvl();
    Group* g = p->getGroup();
    g->changePlayerScore(lvl, oldScore, newScore);
    allPlayersGroup.changePlayerScore(lvl, oldScore, newScore);
    p->setScore(newScore);
    return true;
}

bool PlayersManager::getPercentOfPlayersWithScoreInBounds(int groupId, int score, int lowerLvl, int higherLvl, double* res) {
    if (groupId == 0) {
        return allPlayersGroup.getPercentOfPlayersWithScoreInBounds(score, lowerLvl, higherLvl, res);
    }
    Group* g = uf.find(groupId);
    return g->getPercentOfPlayersWithScoreInBounds(score, lowerLvl, higherLvl, res);
}
bool PlayersManager::averageHighestPlayerLevelByGroup(int groupId, int m, double* res) {
    if (groupId == 0) {
        return  allPlayersGroup.averageHighestPlayerLevelByGroup(m, res);
    }
    Group* g = uf.find(groupId);
    return  g->averageHighestPlayerLevelByGroup(m, res);
}
bool PlayersManager::getPlayersBound(int groupId, int score, int m, int* lowerBound, int* higherBound) {
    if (groupId == 0) {
        return allPlayersGroup.getPlayersBound(score, m, lowerBound, higherBound);
    }
    Group* g = uf.find(groupId);
    return g->getPlayersBound(score, m, lowerBound, higherBound);
}