#include "bot.h"

#include <math.h>

#include "structure.h"
extern node corner[54];
extern side edge[72];
extern int nextdevelopCard;
int botOption(int state, player *Players, int index, piece *lands) {
    if (state == 1) {
        return 1;
    }
    if (Players[index].card->size) {
        return 2;
    }
    if (Players[index].resource[METAL] >= 3 &&
        Players[index].resource[WHEAT] >= 2) {
        for (int i = 0; i < Players[index].haveNode->size; ++i) {
            if (corner[Players[index].haveNode->data[i]].type == SWTTLEMENT) {
                return 1;
            }
        }
    }
    if (Players[index].resource[METAL] >= 3 &&
        Players[index].resource[WHEAT] >= 2) {
        for (int i = 0; i < Players[index].haveNode->size; ++i) {
            if (corner[Players[index].haveNode->data[i]].type == SWTTLEMENT) {
                return 2;
            }
        }
    }
    if (Players[index].resource[BRICKS] >= 1 &&
        Players[index].resource[WOOD] >= 1 &&
        Players[index].resource[WHEAT] >= 1 &&
        Players[index].resource[WOOL] >= 1) {
        for (int i = 0; i < Players[index].haveNode->size; ++i) {
            int can = 1;
            for (int j = 0; j < 2; ++j) {
                if (edge[Players[index].haveSide->data[i]].linkedNode[j] ==
                    None) {
                    can = 1;
                    for (int k = 0; k < 3; ++k) {
                        if (edge[Players[index].haveSide->data[i]]
                                .linkedNode[j]
                                ->linkedNode[k]
                                ->belong != NONE) {
                            can = 0;
                            break;
                        }
                    }
                    if (can) return 1;
                }
            }
        }
    }
    if (Players[index].resource[BRICKS] >= 1 &&
        Players[index].resource[WOOD] >= 1 &&
        Players[index].haveSide->size <= 25) {
        for (int i = 0; i < Players[index].haveSide->size; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 3; ++k) {
                    if (edge[Players[index].haveSide->data[i]]
                            .linkedNode[j]
                            ->linkedSide[k]
                            ->belong == None) {
                        return 3;
                    }
                }
            }
        }
    }
    if (Players[index].resource[WOOL] >= 1 &&
        Players[index].resource[WHEAT] >= 1 &&
        Players[index].resource[METAL] >= 1 && nextdevelopCard < 25) {
        return 4;
    }
    return 0;
}

int botRobber(piece *land, int playerID) {
    int bestID = -1;
    double bestWeights = -1;
    for (int i = 0; i < 19; ++i) {
        double nowWeights = 0;
        int peoplenearhere = 0;
        for (int j = 0; j < 6; ++j) {
            if (land[i].linkedNode[j]->belong != playerID &&
                land[i].linkedNode[j]->belong != PUBLIC) {
                peoplenearhere++;
            } else if (land[i].linkedNode[j]->belong != playerID) {
                peoplenearhere = 0;
                break;
            }
        }
        nowWeights = (6 - abs(land[i].number - 7) *
                              log(10 * (land[i].type == DESERT ? 0.1 : 1)) *
                              sqrt(peoplenearhere));
        if (nowWeights > bestWeights) {
            bestID = i;
            bestWeights = nowWeights;
        }
    }
    return bestID;
}