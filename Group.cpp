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
    std::cout << "1) "<< "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
    std::cout << "2)" << "numPlayers: " << prey->numPlayers << ", numLvl0Players: " << prey->numLvl0Players << ", playersTree->getSize(): " << prey->playersTree->getSize() << std::endl;
    RankTreeOPK* newPlayersTree = new RankTreeOPK(playersTree, prey->playersTree);
    delete playersTree;
    playersTree = newPlayersTree;
    numPlayers += prey->numPlayers;
    delete prey->playersTree;
    prey->playersTree = nullptr;
    for (int i=0; i<SCALE_MAX; ++i) {
        lvl0PlayersScores[i] += prey->lvl0PlayersScores[i];
    }
    numLvl0Players += prey->numLvl0Players;
    std::cout << "3) " << "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
    assert(numPlayers == (numLvl0Players + playersTree->getSize()));
}

void Group::addPlayerOfLvl0(int score) {
    lvl0PlayersScores[score]++;
    numLvl0Players++;
    numPlayers++;
    std::cout << "id: " << id << ", score: " << score << std::endl;
    std::cout << "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
    assert(numPlayers == (numLvl0Players + playersTree->getSize()));
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
    std::cout << "lvl: " << lvl << std::endl;
    std::cout << "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
    assert(numPlayers == (numLvl0Players + playersTree->getSize()));
}

void Group::increasePlayerLvl(int oldLvl, int newLvl, int score) {
    std::cout << "1) " << "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
    assert(newLvl >0);
    if (oldLvl == 0) {
        lvl0PlayersScores[score]--;
        numLvl0Players--;
        playersTree->insert(newLvl, score);
    }
    else {
        if (oldLvl==13 && newLvl == 22) {
            std::cout << "here" << std::endl;
        }
        playersTree->remove(oldLvl, score);
        std::cout << "3)" << "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
        playersTree->insert(newLvl, score);
    }
    std::cout << "oldLvl: " << oldLvl << ", newLvl: " << newLvl << std::endl;
    std::cout << "2) " << "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
    assert(numPlayers == (numLvl0Players + playersTree->getSize()));
}

void Group::changePlayerScore(int lvl, int oldScore, int newScore) {
    std::cout << "1)" << "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
    if (lvl == 0) {
        lvl0PlayersScores[oldScore]--;
        lvl0PlayersScores[newScore]++;
    }
    else {
        playersTree->remove(lvl, oldScore);
        std::cout << "3)" << "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
        playersTree->insert(lvl, newScore);
    }
    std::cout << "id: " << id << ", lvl: " << lvl << std::endl;
    std::cout << "2)" << "numPlayers: " << numPlayers << ", numLvl0Players: " << numLvl0Players << ", playersTree->getSize(): " << playersTree->getSize() << std::endl;
    assert(numPlayers == (numLvl0Players + playersTree->getSize()));
}

bool Group::getPercentOfPlayersWithScoreInBounds(int score, int lowerLvl, int higherLvl, double* res) const {
    int keyMulInRange=0;
    bool success = playersTree->getPercentOfValueInKeyBounds(lowerLvl, higherLvl, score, res, &keyMulInRange);
    if (lowerLvl<=0 && numLvl0Players != 0) {
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
    if (m==0) {
        *lowerBound = 0;
        *higherBound = 0;
        return true;
    }
    //std::cout << "calling function getPlayerBound with m=" << m << ". score=" << score << std::endl;
    //std::cout << "numPlayersInGroup=" << numPlayers << ". numLvl0Players=" << numLvl0Players << ". nulLvl0withScore=" << lvl0PlayersScores[score] << std::endl;
    //playersTree->printBT(score);
    bool success = playersTree->getValRange(score, m, lowerBound, higherBound);
    //std::cout << "done with avl func. lowerBound=" << *lowerBound << ". upperBound=" << *higherBound << std::endl;
    if (!success) {
        getValRangeLvl0Players(score, m, lowerBound, higherBound);
    }
    return true;
}

void Group::getValRangeLvl0Players(int score, int m, int* lowerBound, int* higherBound) const {
    m = m - playersTree->getSize();
    int lvl0NotOfScore = numLvl0Players - lvl0PlayersScores[score];
    int oldHigherBound = *higherBound;
    if (m>=lvl0NotOfScore && m <= numLvl0Players) {
        *lowerBound = oldHigherBound + m - lvl0NotOfScore;
    }
    else if (m>=lvl0NotOfScore && m>numLvl0Players) {
        *lowerBound = oldHigherBound + lvl0PlayersScores[score];
    }
    else if (m<lvl0NotOfScore) {
        *lowerBound = oldHigherBound;
    }
    if (m>=lvl0PlayersScores[score]) {
        *higherBound += lvl0PlayersScores[score];
    }
    else {
        *higherBound += m;
    }
}
