#include <cmath>
#include <iostream>

#include "config.h"
#include "state.h"
#include "mcts.h"
#include "mcts_node.h"

using namespace std;

MCTS::MCTS(GameState initialState, int it)
{
    root = MCTSNode(initialState);
    iterations = it;
}

MCTS::~MCTS()
{
    // delete root;
    for (MCTSNode *child : root.children)
    {
        child->parent = nullptr;
        delete child;
    }

    root.children.clear();
}

MCTSNode *MCTS::getBestChild() const
{
    if (root.children.empty())
    {
        return nullptr;
    }

    // initialise best child
    MCTSNode *bestChild = nullptr;
    double bestUct = -numeric_limits<double>::infinity();

    // visit all the children
    for (MCTSNode *child : root.children)
    {
        double uct = root.getUCT(child);
        if (uct > bestUct)
        {
            bestUct = uct;
            bestChild = child;
        }
    }
    return bestChild;
}

MCTSNode *MCTS::mcts()
{
    root.expand();
    for (int i = 0; i < iterations; i++)
    {
        MCTSNode *selectedNode = root.select();
        selectedNode->expand();
        int simTime = 10;
        double total = 0.0;
        for (int j = 0; j < simTime; j++)
        {
            total += selectedNode->state.simulate(root.state.currentPlayer, false);
        }
        selectedNode->backPropagate(total / simTime);
    }

    return getBestChild();
}