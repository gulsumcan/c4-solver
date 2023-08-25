#include <iostream>
#include <random>

#include "config.h"
#include "state.h"
#include "mcts.h"
#include "mcts_node.h"

using namespace std;

const bool VERBOSE = false;

double competeAgainstRandom(GameState state)
{
    int lastMove, lastPlayer;
    if (VERBOSE)
    {
        cout << "Starting a game..." << endl;
        state.printGameState();
    }
    while (!state.isDraw())
    {
        if (state.currentPlayer == 2)
        {
            lastMove = state.getValidMove();
            state.applyMove(lastMove);
            lastPlayer = 2;
        }
        else
        {
            MCTS mcts = MCTS(state, 100);
            MCTSNode *bestMove = mcts.mcts();
            state.applyMove(bestMove->move);
            lastMove = bestMove->move;
            lastPlayer = 1;
            // delete bestMove;
        }

        if (VERBOSE)
        {
            cout << "After applying move " << lastMove << " for player " << lastPlayer << endl;
            state.printGameState();
        }

        if (state.isWin(lastPlayer, lastMove))
        {
            if (VERBOSE)
                cout << "Player " << lastPlayer << " won!" << endl
                     << endl;
            return lastPlayer == 1 ? 1.0 : 0.0;
        }
    }

    if (VERBOSE)
        cout << "It's a draw!" << endl
             << endl;

    return 0.5;
}

int main()
{
    srand(1);

    // GameState initialState = {
    //     {
    //         {0, 0, 2, 1, 2, 1, 2}, // Row 1
    //         {0, 0, 1, 1, 1, 1, 0}, // Row 2
    //         {0, 0, 0, 2, 0, 2, 0}, // Row 3
    //         {0, 0, 0, 0, 0, 0, 0}, // Row 4
    //         {0, 0, 0, 0, 0, 0, 0}, // Row 5
    //         {0, 0, 0, 0, 0, 0, 0}  // Row 6
    //     },
    //     1
    // };

    // GameState initialState = {
    //     {
    //         {1, 2, 2, 1, 2, 1, 1}, // Row 1
    //         {2, 1, 2, 1, 1, 2, 1}, // Row 2
    //         {1, 1, 1, 2, 2, 2, 1}, // Row 3
    //         {2, 1, 2, 0, 1, 2, 0}, // Row 4
    //         {2, 2, 2, 0, 2, 0, 0}, // Row 5
    //         {1, 1, 0, 0, 1, 0, 0}  // Row 6
    //     },
    //     1
    // };

    // cout << result(initialState, 2) << endl;

    int board[ROWS][COLS] = {
        {0, 0, 0, 0, 0, 0, 0}, // Row 1
        {0, 0, 0, 0, 0, 0, 0}, // Row 2
        {0, 0, 0, 0, 0, 0, 0}, // Row 3
        {0, 0, 0, 0, 0, 0, 0}, // Row 4
        {0, 0, 0, 0, 0, 0, 0}, // Row 5
        {0, 0, 0, 0, 0, 0, 0}  // Row 6
    };

    GameState initialState = GameState(board, 1);

    int win = 0;
    int lose = 0;
    int draw = 0;
    int games = 1000;
    for (int i = 1; i <= games; i++)
    {
        double result = competeAgainstRandom(initialState);
        if (result == 0.0)
        {
            lose += 1;
        }
        if (result == 0.5)
        {
            draw += 1;
        }
        if (result == 1.0)
        {
            win += 1;
        }
        cout << "GAME #" << i << endl;
        cout << "WIN: " << win << ", WIN RATE: " << static_cast<double>(win) / i << endl;
        cout << "DRAW: " << draw << ", DRAW RATE: " << static_cast<double>(draw) / i << endl;
        cout << "LOSE: " << lose << ", LOSE RATE: " << static_cast<double>(lose) / i << endl
             << endl;
    }

    // cout << "WIN: " << win << ", WIN RATE: " << static_cast<double>(win) / games << endl;
    // cout << "DRAW: " << draw << ", DRAW RATE: " << static_cast<double>(draw) / games << endl;
    // cout << "LOSE: " << lose << ", LOSE RATE: " << static_cast<double>(lose) / games << endl;
}