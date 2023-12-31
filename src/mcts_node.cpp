#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <random>

#include "state.h"
#include "mcts_node.h"
#include "config.h"

random_device rd;
mt19937 g(rd());

const double EXPLORATION_CONST = M_SQRT2;

int CREATE_ = 0;
int DELETE_ = 0;

MCTSNode::MCTSNode()
{
    this->parent = nullptr;
    this->move = -1;

    this->state = GameState();
    this->visits = 0;
    this->value = 0.0;
    this->children = vector<MCTSNode *>{};

    CREATE_++;
}

MCTSNode::MCTSNode(GameState s)
{
    this->parent = nullptr;
    this->move = -1;

    this->state = GameState(s);
    this->visits = 0;
    this->value = 0.0;
    this->children = vector<MCTSNode *>{};

    CREATE_++;
}

MCTSNode::~MCTSNode()
{
    for (MCTSNode *child : children)
    {
        if (!child)
        {
            continue;
        }
        child->parent = nullptr;
        delete child;
    }

    children.clear();

    if (parent)
    {
        parent->removeFromParent(this);
        parent = nullptr;

        cout << "Created " << CREATE_ << ", Deleted " << DELETE_ << endl;
    }

    DELETE_++;
}

void MCTSNode::removeFromParent(MCTSNode *child)
{
    auto it = find(children.begin(), children.end(), child);
    if (it != children.end())
        children.erase(it);
}

MCTSNode::MCTSNode(int m, MCTSNode *p)
{
    parent = p;
    move = m;
    // p->printMCTSNode();
    state = GameState(p->state);
    state.applyMove(m);
    // state.printGameState();
    visits = 0;
    value = 0.0;
    children = vector<MCTSNode *>{};

    p->children.push_back(this);
    CREATE_++;
}

// populates children nodes for MCTSNode
void MCTSNode::expand()
{
    // cout << "Attempting an expand..." << endl;
    // printMCTSNode();

    // check if node already has children
    if (this->children.size() >= COLS)
    {
        throw invalid_argument("node already has all children");
    }

    // TODO: allow generating new children
    if (this->children.size() > 0)
    {
        throw invalid_argument("node already has children");
    }

    // generate the children in random order
    vector<int> unexploredMoves{0, 1, 2, 3, 4, 5, 6};
    shuffle(unexploredMoves.begin(), unexploredMoves.end(), g);

    for (int move : unexploredMoves)
    {
        // skip invalid moves
        if (!state.isValid(move))
            continue;

        MCTSNode *newNode = new MCTSNode(move, this);
        // children.push_back(newNode);
    }
}

MCTSNode *MCTSNode::select()
{
    // leaf node
    if (children.empty())
    {
        return this;
    }

    // initialise best child
    MCTSNode *bestChild = nullptr;
    double bestUct = -numeric_limits<double>::infinity();

    // visit all the children
    for (MCTSNode *child : children)
    {
        if (child->visits == 0)
        {
            return child; // Return unvisited child to ensure exploration
        }

        double uct = getUCT(child);
        if (uct > bestUct)
        {
            bestUct = uct;
            bestChild = child;
        }
    }

    return bestChild->select();
}

void MCTSNode::backPropagate(double result)
{
    visits += 1;
    value += result;

    if (parent)
    {
        parent->backPropagate(result);
    }
}

double MCTSNode::getUCT(MCTSNode *child) const
{
    return (static_cast<double>(child->value) / child->visits) +
           EXPLORATION_CONST * sqrt(log(visits) / visits);
}

void MCTSNode::printMCTSNode() const
{
    state.printGameState();
    cout << "Visits: " << visits << endl;
    cout << "Value: " << value << endl;
    cout << "Child node count: " << children.size() << endl;
    cout << "Move: " << move << endl;

    cout << "\n";
}