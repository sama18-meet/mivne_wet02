#include "Group.h"

Group::Group(int id) : id(id), numPlayers(0), numLvl0Players(0) {
    playersTree = new RankTreeOPK();
    for (int i=0; i<SCALE_MAX; ++i) {
        Group::lvl0PlayersScores[i] = 0;
    }
}

int Group::getId() {
    return id;
}

void Group::swallow(Group* prey) {
    RankTreeOPK* newPlayersTree = new RankTreeOPK(playersTree, prey->playersTree);
    delete playersTree;
    playersTree = newPlayersTree;
    numPlayers += prey->numPlayers;
    for (int i=0; i<SCALE_MAX; ++i) {
        lvl0PlayersScores[i] += prey->lvl0PlayersScores[i];
    }
    numLvl0Players += prey->numLvl0Players;
}

void Group::addPlayerOfLvl0(int score) {
    lvl0PlayersScores[score]++;
    numLvl0Players++;
    numPlayers++;
}

void Group::removePlayer(int lvl, int score) {
    if (lvl == 0) {
        lvl0PlayersScores[score]--;
        numLvl0Players--;
    }
    else {
        playersTree->remove(lvl, score);
    }
    numPlayers--;
}

void Group::increasePlayerLvl(int oldLvl, int newLvl, int score) {
    assert(newLvl >0);
    if (oldLvl == 0) {
        lvl0PlayersScores[score]--;
        numLvl0Players--;
        playersTree->insert(newLvl, score);
    }
    else {
        playersTree->remove(oldLvl, score);
        playersTree->insert(newLvl, score);
    }
}

void Group::changePlayerScore(int lvl, int oldScore, int newScore) {
    if (lvl == 0) {
        lvl0PlayersScores[oldScore]--;
        lvl0PlayersScores[newScore]++;
    }
    else {
        playersTree->remove(lvl, oldScore);
        playersTree->insert(lvl, newScore);
    }
}

bool Group::getPercentOfPlayersWithScoreInBounds(int score, int lowerLvl, int higherLvl, double* res) const {
    int keyMulInRange=0;
    bool success = playersTree->getPercentOfValueInKeyBounds(lowerLvl, higherLvl, score, res, &keyMulInRange);
    if (lowerLvl==0 && numLvl0Players != 0) {
        double sum = (*res * (double)keyMulInRange)/100 + lvl0PlayersScores[score]; //TODO maybe problem of converting between float and int
        int totalNumOfPlayers = numLvl0Players + keyMulInRange;
        *res = 100*(sum / (double)totalNumOfPlayers);
        return true;
    }
    return success;
}

bool Group::averageHighestPlayerLevelByGroup(int m, double* res) const {
    if (m > numPlayers) {
        return false;
    }
    *res = playersTree->getAvgHighest(m);
    return true;
}

bool Group::getPlayersBound(int score, int m, int* lowerBound, int* higherBound) const {
    if (m > numPlayers) {
        return false;
    }
    bool success = playersTree->getValRange(score, m, lowerBound, higherBound);
    if (!success) {
        getValRangeLvl0Players(score, m, lowerBound, higherBound);
    }
    return true;
}

void Group::getValRangeLvl0Players(int score, int m, int* lowerBound, int* higherBound) const {
    int currSum = m - playersTree->getSize(); //runs after tree had been checked
    *higherBound += fmin(currSum, lvl0PlayersScores[score]);
    for (int i = 1; i < SCALE_MAX; ++i) {
        if (i != score) {
            currSum -= lvl0PlayersScores[i];
        }
    }
    *lowerBound += (currSum < 0)? 0 : currSum;
}
