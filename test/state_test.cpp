#include <iostream>
#include <cassert>

#include "../src/config.h"
#include "../src/state.h"

using namespace std;

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
    GameState state = {
        {
            {0, 1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0},
        },
        1};

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
    GameState state = {
        {
            {0, 1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
        },
        1};

    state.applyMove(1);
    assert(state.isWin(1, 1));
}

void testHorizontalWin()
{
    GameState state = {
        {
            {0, 1, 2, 2, 1, 1, 2},
            {0, 1, 0, 2, 2, 2, 1},
            {0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
        },
        2};

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
    GameState state = {
        {
            {0, 1, 2, 2, 1, 1, 2},
            {0, 1, 0, 2, 2, 2, 1},
            {0, 1, 0, 1, 1, 2, 2},
            {0, 0, 0, 0, 1, 2, 1},
            {0, 0, 0, 0, 0, 0, 2},
            {0, 0, 0, 0, 0, 0, 1},
        },
        1};

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
    GameState state = {
        {
            {0, 1, 2, 2, 1, 1, 2},
            {0, 1, 1, 2, 2, 2, 1},
            {0, 1, 1, 1, 0, 2, 2},
            {0, 1, 1, 2, 0, 1, 1},
            {0, 1, 0, 0, 0, 0, 2},
            {0, 2, 0, 0, 0, 0, 1},
        },
        2};

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


void illegalMove(GameState state, int player, int move) {
    state.applyMove(move);
}

void testIllegalMoves()
{
    GameState state = {
        {
            {0, 1, 2, 2, 1, 1, 2},
            {0, 1, 1, 2, 2, 2, 1},
            {0, 1, 1, 1, 0, 2, 2},
            {0, 1, 1, 2, 0, 1, 1},
            {0, 1, 0, 0, 0, 0, 2},
            {0, 2, 0, 0, 0, 0, 1},
        },
        2};

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

int main()
{
    testMoves();
    testVerticalWin();
    testHorizontalWin();
    testDiagonalLeftWin();
    testDiagonalRighttWin();
    testIllegalMoves();

    return 0;
}