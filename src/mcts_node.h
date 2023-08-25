#pragma once

#include <vector>

#include "state.h"

using namespace std;

struct MCTSNode
{
    GameState state;
    int visits;   // number of visits
    double value; // wins 1.0 - draw 0.5 - lose 0.0
    vector<MCTSNode *> children;

    MCTSNode *parent;
    int move;

    MCTSNode();
    ~MCTSNode();
    MCTSNode(int, MCTSNode *);
    void expand();
    void removeFromParent(MCTSNode *);
    MCTSNode *select();

    double getUCT(MCTSNode *) const;
    void printMCTSNode() const;
};