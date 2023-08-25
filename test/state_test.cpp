#include <iostream>
#include <cassert>

#include "../src/config.h"
#include "../src/state.h"
#include "../src/mcts_node.h"

using namespace std;

const bool STATE_TEST = false;
const bool MCTS_NODE_TEST = true;

// Helper function to initialize a GameState object with an empty board
GameState createEmptyGameState()
{
    GameState state;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            state.board[i][j] = 0;
        }
    }
    state.currentPlayer = 1;
    return state;
};

void expectException(void (*func)(GameState, int, int), GameState state, int player, int move)
{
    bool exceptionThrown = false;

    try
    {
        func(state, player, move);
    }
    catch (...) // or any exception at all
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown); // or whatever else you want to do
}

void testMoves()
{
    int board[ROWS][COLS] = {
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
    };
    GameState state = GameState(board, 1);

    // invalid moves
    assert(!state.isValid(1));
    assert(!state.isValid(-1));
    assert(!state.isValid(COLS));

    // valid moves
    assert(state.isValid(0));
    assert(state.isValid(6));
}

void testVerticalWin()
{
    int board[ROWS][COLS] = {
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
    };
    GameState state = GameState(board, 1);

    state.applyMove(1);
    assert(state.isWin(1, 1));
}

void testHorizontalWin()
{
    int board[ROWS][COLS] = {
        {0, 1, 2, 2, 1, 1, 2},
        {0, 1, 0, 2, 2, 2, 1},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
    };
    GameState state = GameState(board, 2);

    assert(!state.isWin(2, 2));
    assert(!state.isWin(2, 3));

    state.applyMove(2);
    assert(state.isWin(2, 2));
}

void diagonalWinIllegalState(GameState state, int player, int move)
{
    state.isWin(player, move);
}

void testDiagonalLeftWin()
{
    int board[ROWS][COLS] = {
        {0, 1, 2, 2, 1, 1, 2},
        {0, 1, 0, 2, 2, 2, 1},
        {0, 1, 0, 1, 1, 2, 2},
        {0, 0, 0, 0, 1, 2, 1},
        {0, 0, 0, 0, 0, 0, 2},
        {0, 0, 0, 0, 0, 0, 1},
    };
    GameState state = GameState(board, 1);

    expectException(diagonalWinIllegalState, state, 2, 1);
    expectException(diagonalWinIllegalState, state, 2, 0);
    expectException(diagonalWinIllegalState, state, 1, 5);
    expectException(diagonalWinIllegalState, state, 1, 0);

    cout << "Testing diagonal wins...\n\nbefore:" << endl;
    state.printGameState();
    state.applyMove(2);
    cout << "after moving 2:\n";
    state.printGameState();
    assert(state.isWin(1, 2));
    cout << "\n\n";

    // reset
    state.board[1][2] = 0;
    state.currentPlayer = 1;

    cout << "before:\n";
    state.printGameState();
    state.applyMove(5);
    cout << "after moving 5:\n";
    state.printGameState();
    cout << "\n\n";
    assert(state.isWin(1, 5));

    // reset
    state.board[4][5] = 0;
    state.currentPlayer = 1;

    assert(!state.isWin(1, 1));
    assert(!state.isWin(1, 3));
    assert(!state.isWin(1, 4));

    assert(!state.isWin(2, 2));
    assert(!state.isWin(2, 5));
}

void testDiagonalRighttWin()
{
    int board[ROWS][COLS] = {
        {0, 1, 2, 2, 1, 1, 2},
        {0, 1, 1, 2, 2, 2, 1},
        {0, 1, 1, 1, 0, 2, 2},
        {0, 1, 1, 2, 0, 1, 1},
        {0, 1, 0, 0, 0, 0, 2},
        {0, 2, 0, 0, 0, 0, 1},
    };
    GameState state = GameState(board, 2);

    expectException(diagonalWinIllegalState, state, 2, 2);
    expectException(diagonalWinIllegalState, state, 2, 0);
    expectException(diagonalWinIllegalState, state, 1, 4);
    expectException(diagonalWinIllegalState, state, 1, 0);

    cout << "Testing diagonal right wins...\n\nbefore:" << endl;
    state.printGameState();
    state.applyMove(4);
    cout << "after moving 4:\n";
    state.printGameState();
    assert(state.isWin(2, 4));
    cout << "\n\n";

    // reset
    state.board[2][4] = 0;
    state.board[0][6] = 1;
    state.currentPlayer = 2;

    cout << "before:\n";
    state.printGameState();
    state.applyMove(2);
    cout << "after moving 2:\n";
    state.printGameState();
    cout << "\n\n";
    assert(!state.isWin(2, 2));

    state.currentPlayer = 2;
    state.applyMove(4);
    cout << "after moving 4:\n";
    state.printGameState();
    cout << "\n\n";
    assert(state.isWin(2, 4));
}

void illegalMove(GameState state, int player, int move)
{
    state.applyMove(move);
}

void testIllegalMoves()
{
    int board[ROWS][COLS] = {
        {0, 1, 2, 2, 1, 1, 2},
        {0, 1, 1, 2, 2, 2, 1},
        {0, 1, 1, 1, 0, 2, 2},
        {0, 1, 1, 2, 0, 1, 1},
        {0, 1, 0, 0, 0, 0, 2},
        {0, 2, 0, 0, 0, 0, 1},
    };
    GameState state = GameState(board, 2);

    assert(!state.isValid(1));
    expectException(illegalMove, state, 2, 1);

    assert(!state.isValid(6));
    expectException(illegalMove, state, 2, 6);

    state.currentPlayer = 1;
    assert(!state.isValid(1));
    expectException(illegalMove, state, 1, 1);

    assert(!state.isValid(6));
    expectException(illegalMove, state, 1, 6);
}

void testDeepCopy()
{
    GameState original = GameState();
    original.applyMoveNoSwitch(0);

    GameState deepCopy = GameState(original);
    deepCopy.applyMoveNoSwitch(0);

    assert(original.board[0][0] == deepCopy.board[0][0]);
    assert(original.board[1][0] == 0);
    assert(original.board[1][0] != deepCopy.board[1][0]);
}

/*
MCTS Node tests
*/

void testDefaultConstructor()
{
    MCTSNode node = MCTSNode();

    assert(node.move == -1);
    assert(node.parent == nullptr);
    assert(node.children.size() == 0);
}

void testConstructorWithParent()
{
    GameState state = GameState();
    MCTSNode parent = MCTSNode();
    MCTSNode child = MCTSNode(0, &parent);

    assert(child.move == 0);
    assert(child.parent == &parent);
    assert(child.children.size() == 0);
}

void testExpandAndDeconstructor()
{
    GameState state = GameState();
    MCTSNode parent = MCTSNode();
    parent.expand();

    assert(parent.children.size() == 7);

    MCTSNode *child1 = parent.children.at(0);
    assert(child1->parent == &parent);
    assert(child1->children.size() == 0);

    child1->expand();
    assert(child1->children.size() == 7);

    // MCTSNode* grandChild1 = child1->children.at(0);
    // assert(grandChild1->parent == child1);
    // assert(grandChild1->children.size() == 0);

    // grandChild1->expand();
    // assert(grandChild1 -> children.size() == 7);
    // MCTSNode* grandChild2 = grandChild1->children.at(0);

    // // test deconstructor
    // delete grandChild1;
    // assert(grandChild2==nullptr);
    // assert(grandChild1==nullptr);
    // assert(child1->children.size() == 6);
}

void testRemoveParentLink()
{
    GameState state = GameState();
    MCTSNode parent = MCTSNode();
    parent.expand();

    MCTSNode *child1 = parent.children.at(0);
    parent.removeFromParent(child1);
    assert(parent.children.size() == 6);
}

void MCTSNodeTests()
{
    // testDefaultConstructor();
    // testConstructorWithParent();
    // testExpandAndDeconstructor();
    testRemoveParentLink();

    // MCTSNode val = MCTSNode();
    // MCTSNode c = MCTSNode(0, &val);

    // delete val.children.at(0);
}

int main()
{
    if (STATE_TEST)
    {
        testMoves();
        testVerticalWin();
        testHorizontalWin();
        testDiagonalLeftWin();
        testDiagonalRighttWin();
        testIllegalMoves();

        testDeepCopy();
    }
    if (MCTS_NODE_TEST)
    {
        MCTSNodeTests();
    }

    return 0;
}