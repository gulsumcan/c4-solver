#include <iostream>
#include <chrono>
#include <random>

#include "config.h"
#include "state.h"
#include "mcts.h"
#include "mcts_node.h"

using namespace std;
using namespace std::chrono;

const bool VERBOSE = true;

double competeAgainstHuman(GameState state)
{
    int lastMove, lastPlayer;
    int totalMoves = 0;
    nanoseconds totalTimeForMoves;
    if (VERBOSE)
    {
        cout << "Starting a game..." << endl;
        state.printGameState();
    }
    while (!state.isDraw())
    {
        if (state.currentPlayer == 2)
        {
            cin >> lastMove;
            while (!state.isValid(lastMove))
            {
                cout << "You entered an invalis input! Please enter again... ";
                cin >> lastMove;
            }
            state.applyMove(lastMove);
            lastPlayer = 2;
        }
        else
        {
            time_point moveStartTime = steady_clock().now();
            MCTS mcts = MCTS(state, 10000);
            MCTSNode *bestMove = mcts.mcts();
            time_point moveEndTime = steady_clock().now();

            totalTimeForMoves += duration_cast<nanoseconds>(moveEndTime - moveStartTime);
            state.applyMove(bestMove->move);
            lastMove = bestMove->move;
            lastPlayer = 1;

            totalMoves++;
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

            cout << "Total moves: " << totalMoves << endl;
            cout << "Move time ns: " << duration_cast<milliseconds>(totalTimeForMoves / totalMoves) << endl;
            return lastPlayer == 1 ? 1.0 : 0.0;
        }
    }

    if (VERBOSE)
        cout << "It's a draw!" << endl
             << endl;

    cout << "Total moves: " << totalMoves << endl;
    cout << "Move time ns: " << totalTimeForMoves / totalMoves << endl;
    return 0.5;
}

double competeAgainstSelf(GameState state)
{
    int lastMove, lastPlayer;
    int totalMoves = 0;
    nanoseconds totalTimeForMoves;
    if (VERBOSE)
    {
        cout << "Starting a game..." << endl;
        state.printGameState();
    }
    while (!state.isDraw())
    {
        if (state.currentPlayer == 2)
        {
            MCTS mcts = MCTS(state, 500);
            MCTSNode *bestMove = mcts.mcts();
            state.applyMove(bestMove->move);
            lastMove = bestMove->move;
            lastPlayer = 2;
        }
        else
        {
            time_point moveStartTime = steady_clock().now();
            MCTS mcts = MCTS(state, 1000);
            MCTSNode *bestMove = mcts.mcts();
            time_point moveEndTime = steady_clock().now();

            totalTimeForMoves += duration_cast<nanoseconds>(moveEndTime - moveStartTime);
            state.applyMove(bestMove->move);
            lastMove = bestMove->move;
            lastPlayer = 1;

            totalMoves++;
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

            cout << "Total moves: " << totalMoves << endl;
            cout << "Move time ns: " << duration_cast<milliseconds>(totalTimeForMoves / totalMoves) << endl;
            return lastPlayer == 1 ? 1.0 : 0.0;
        }
    }

    if (VERBOSE)
        cout << "It's a draw!" << endl
             << endl;

    cout << "Total moves: " << totalMoves << endl;
    cout << "Move time ns: " << totalTimeForMoves / totalMoves << endl;
    return 0.5;
}

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
            MCTS mcts = MCTS(state, 500);
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
    int games = 1;
    time_point startTime = steady_clock().now();
    for (int i = 1; i <= games; i++)
    {
        time_point gameStartTime = steady_clock().now();
        double result = competeAgainstHuman(initialState);
        time_point gameEndTime = steady_clock().now();

        nanoseconds timeTaken = duration_cast<nanoseconds>(gameEndTime - gameStartTime);

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
        cout << "Time taken:" << timeTaken << endl;
        cout << "WIN: " << win << ", WIN RATE: " << static_cast<double>(win) / i << endl;
        cout << "DRAW: " << draw << ", DRAW RATE: " << static_cast<double>(draw) / i << endl;
        cout << "LOSE: " << lose << ", LOSE RATE: " << static_cast<double>(lose) / i << endl
             << endl;
    }

    time_point endTime = steady_clock().now();
    nanoseconds totalTimeTaken = duration_cast<nanoseconds>(endTime - startTime);
    cout << "Time taken in nanos: " << totalTimeTaken << endl;
    cout << "Time taken in millis: " << duration_cast<milliseconds>(totalTimeTaken) << endl;
    cout << "Time taken in seconds: " << duration_cast<seconds>(totalTimeTaken) << endl;
    // cout << "WIN: " << win << ", WIN RATE: " << static_cast<double>(win) / games << endl;
    // cout << "DRAW: " << draw << ", DRAW RATE: " << static_cast<double>(draw) / games << endl;
    // cout << "LOSE: " << lose << ", LOSE RATE: " << static_cast<double>(lose) / games << endl;
}