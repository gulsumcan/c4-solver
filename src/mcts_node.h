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
    MCTSNode(GameState);
    MCTSNode(int, MCTSNode *);
    ~MCTSNode();

    MCTSNode *select();
    void expand();
    void backPropagate(double);

    double getUCT(MCTSNode *) const;
    void printMCTSNode() const;
    void removeFromParent(MCTSNode *);
};