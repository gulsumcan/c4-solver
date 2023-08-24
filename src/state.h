using namespace std;

#include "config.h"

struct GameState
{
    int board[ROWS][COLS];
    int currentPlayer; // player 1 - opponent 2

    bool operator==(const GameState &other) const;
    bool isValid(int move) const;
    bool isWin(int player, int lastMove) const;
    void printGameState() const;

    void applyMove(int move);

private:
    bool isVerticalWin(int player, int rowPlayed, int lastMove) const;
    bool isHorizontalWin(int player, int rowPlayed, int lastMove) const;
    bool isDiagonalWinLeft(int player, int rowPlayed, int lastMove) const;
    bool isDiagonalWinRight(int player, int rowPlayed, int lastMove) const;
};