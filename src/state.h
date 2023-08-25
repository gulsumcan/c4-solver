#pragma once

#include "config.h"

using namespace std;

struct GameState
{
    int board[ROWS][COLS];
    int currentPlayer; // player 1 - opponent 2

    GameState();
    GameState(int b[ROWS][COLS], int p);
    GameState(GameState &other);
    bool operator==(const GameState &other) const;
    bool isValid(int move) const;
    bool isDraw() const;

    bool isWin(int player, int lastMove) const;
    int getValidMove() const;
    void printGameState() const;

    void applyMove(int move);
    void applyMoveNoSwitch(int move);
    double simulate(int, bool);

private:
    bool isVerticalWin(int player, int rowPlayed, int lastMove) const;
    bool isHorizontalWin(int player, int rowPlayed, int lastMove) const;
    bool isDiagonalWinLeft(int player, int rowPlayed, int lastMove) const;
    bool isDiagonalWinRight(int player, int rowPlayed, int lastMove) const;
};