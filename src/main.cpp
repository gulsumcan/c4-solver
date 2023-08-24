#include <iostream>

#include "config.h"
#include "state.h"

using namespace std;

// Helper function to initialize a GameState object with an empty board
GameState createEmptyGameState2()
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

// int main()
// {
//     GameState state = createEmptyGameState();

//     state.isValid(0);
//     cout << "hello" << endl;
//     return 0;
// }