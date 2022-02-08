#include"library2.h"
#include"PlayersManager.h"

void* Init(int k, int scale) {
    try {
        PlayersManager *DS = new PlayersManager(k, scale);
        return (void*)DS;
    }
    catch (std::bad_alloc&) {
        return nullptr;
    }
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2) {
    if (DS == nullptr) {
        return INVALID_INPUT;
    }
    PlayersManager* pm = (PlayersManager*)DS;
    int k = pm->getK();
    if (GroupID1 <= 0 || GroupID2 <= 0 || GroupID1 > k || GroupID2 > k) {
        return INVALID_INPUT;
    }
    try {
        bool success = pm->mergeGroups(GroupID1, GroupID2);
        return success ? SUCCESS : FAILURE;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score) {
    if (DS == nullptr) {
        return INVALID_INPUT;
    }
    PlayersManager* pm = (PlayersManager*)DS;
    int k = pm->getK();
    int scale = pm->getScale();
    if (GroupID <= 0 || GroupID > k || PlayerID <= 0 || score > scale || score <= 0) {
        return INVALID_INPUT;
    }
    try {
        bool success = pm->addPlayer(PlayerID, GroupID, score);
        return success ? SUCCESS : FAILURE;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType RemovePlayer(void *DS, int PlayerID) {
    if (DS == nullptr || PlayerID <= 0) {
        return INVALID_INPUT;
    }
    PlayersManager* pm = (PlayersManager*)DS;
    try {
        bool success = pm->removePlayer(PlayerID);
        return success ? SUCCESS : FAILURE;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease) {
    if (DS == nullptr || PlayerID <= 0 || LevelIncrease <= 0) {
        return INVALID_INPUT;
    }
    PlayersManager* pm = (PlayersManager*)DS;
    try {
        bool success = pm->increasePlayerIdLvl(PlayerID, LevelIncrease);
        return success ? SUCCESS : FAILURE;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore) {
    if (DS == nullptr) {
        return INVALID_INPUT;
    }
    PlayersManager* pm = (PlayersManager*)DS;
    int scale = pm->getScale();
    if (PlayerID <= 0 || NewScore > scale || NewScore <= 0) {
        return INVALID_INPUT;
    }
    try {
        bool success = pm->changePlayerIdScore(PlayerID, NewScore);
        return success ? SUCCESS : FAILURE;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType GetPercentOfPlayersWithScoreInBounds
            (void *DS, int GroupID, int score, int lowerLevel, int higherLevel, double * players) {
    if (DS == nullptr || players == nullptr) {
        return INVALID_INPUT;
    }
    PlayersManager* pm = (PlayersManager*)DS;
    int k = pm->getK();
    if (GroupID < 0 || GroupID > k) {
        return INVALID_INPUT;
    }
    try {
        bool success = pm->getPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel, players);
        return success ? SUCCESS : FAILURE;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * avgLevel) {
    if (DS == nullptr || avgLevel == nullptr) {
        return INVALID_INPUT;
    }
    PlayersManager* pm = (PlayersManager*)DS;
    int k = pm->getK();
    if (GroupID < 0 || GroupID > k || m <= 0) {
        return INVALID_INPUT;
    }
    try {
        bool success = pm->averageHighestPlayerLevelByGroup(GroupID, m, avgLevel);
        return success ? SUCCESS : FAILURE;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m, int* LowerBoundPlayers, int* HigherBoundPlayers) {
    if (DS == nullptr || LowerBoundPlayers == nullptr || HigherBoundPlayers == nullptr) {
        return INVALID_INPUT;
    }
    PlayersManager* pm = (PlayersManager*)DS;
    int k = pm->getK();
    int scale = pm->getScale();
    if (GroupID < 0 || GroupID > k || m < 0 || score > scale || score <= 0) {
        return INVALID_INPUT;
    }
    try {
        bool success = pm->getPlayersBound(GroupID, score, m, LowerBoundPlayers, HigherBoundPlayers);
        return success ? SUCCESS : FAILURE;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

void Quit(void **DS) {
    PlayersManager* pm = (PlayersManager*)* DS;
    delete pm;
    *DS = nullptr;
}
