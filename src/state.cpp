#include <cmath>
#include <iostream>
#include <unordered_map>
#include <cassert>
#include <random>
#include <string>

#include "config.h"
#include "state.h"

using namespace std;

GameState::GameState()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = 0;
        }
    }
    currentPlayer = 1;
}

GameState::GameState(int newBoard[ROWS][COLS], int player)
{
    assert(player == 1 || player == 2);

    currentPlayer = player;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = newBoard[i][j];
        }
    }
}

GameState::GameState(GameState &state)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = state.board[i][j];
        }
    }

    currentPlayer = state.currentPlayer;
}

// Define an operator== for GameState objects
bool GameState::operator==(const GameState &other) const
{
    // Compare the board and currentPlayer for equality
    if (currentPlayer != other.currentPlayer)
        return false;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] != other.board[i][j])
                return false;
        }
    }

    return true;
}

bool GameState::isValid(int move) const
{
    if (move < 0 || move >= COLS)
        return false;

    return board[ROWS-1][move] == 0;
    // // there should be empty row for the given column
    // for (int row = 0; row < ROWS; row++)
    // {
    //     if (board[row][move] == 0)
    //     {
    //         return true;
    //     }
    // }

    // return false; // there is no more empty rows
}

bool GameState::isVerticalWin(int player, int rowPlayed, int lastMove) const
{
    if (rowPlayed > 2 && board[rowPlayed - 1][lastMove] == player && board[rowPlayed - 2][lastMove] == player && board[rowPlayed - 3][lastMove] == player)
        return true;

    return false;
}

bool GameState::isHorizontalWin(int player, int rowPlayed, int lastMove) const
{
    int playerMoves = 0;
    for (int i = 0; i < COLS; i++)
    {
        if (board[rowPlayed][i] == player)
            playerMoves++;
        else
            playerMoves = 0;

        if (playerMoves >= 4)
            return true;
    }

    return false;
}

bool GameState::isDiagonalWinLeft(int player, int rowPlayed, int lastMove) const
{
    // go to first row
    while (rowPlayed > 0 && lastMove > 0 && board[rowPlayed][lastMove] == player)
    {
        rowPlayed--;
        lastMove--;
    }

    if (board[rowPlayed][lastMove] != player)
    {
        if (rowPlayed < ROWS - 1 && lastMove < COLS - 1)
        {
            rowPlayed++;
            lastMove++;
        }
        else
            throw invalid_argument("invalid move!");
    }

    int playerMoves = 0;
    while (rowPlayed < ROWS && lastMove < COLS && board[rowPlayed][lastMove] == player)
    {
        rowPlayed++;
        lastMove++;
        playerMoves++;
    }

    return playerMoves >= 4;
}

bool GameState::isDiagonalWinRight(int player, int rowPlayed, int lastMove) const
{
    // go to first row
    while (rowPlayed > 0 && lastMove < COLS - 1 && board[rowPlayed][lastMove] == player)
    {
        rowPlayed--;
        lastMove++;
    }

    if (board[rowPlayed][lastMove] != player)
    {
        if (rowPlayed < ROWS - 1 && lastMove > 0)
        {
            rowPlayed++;
            lastMove--;
        }
        else
            throw invalid_argument("invalid move!");
    }

    int playerMoves = 0;
    while (rowPlayed < ROWS && lastMove >= 0 && board[rowPlayed][lastMove] == player)
    {
        rowPlayed++;
        lastMove--;
        playerMoves++;
    }

    return playerMoves >= 4;
}

bool GameState::isWin(int player, int lastMove) const
{
    int rowPlayed = ROWS - 1;
    while (rowPlayed >= 0 && board[rowPlayed][lastMove] == 0)
    {
        rowPlayed--;
    }

    if (rowPlayed < 0)
        throw invalid_argument("no play for last row!");

    if (board[rowPlayed][lastMove] != player)
        throw invalid_argument("last move belongs to other player!");

    return isVerticalWin(player, rowPlayed, lastMove) || isHorizontalWin(player, rowPlayed, lastMove) || isDiagonalWinLeft(player, rowPlayed, lastMove) || isDiagonalWinRight(player, rowPlayed, lastMove);
}

bool GameState::isDraw() const
{
    int moves[7] = {0, 1, 2, 3, 4, 5, 6};

    for (int move : moves)
    {
        if (isValid(move))
            return false;
    }

    return true;
}

void GameState::printGameState() const
{
    // Print the game board
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            // Print the content of each cell on the board
            if (board[row][col] == 0)
            {
                cout << "."; // Empty cell
            }
            else if (board[row][col] == 1)
            {
                cout << "X"; // Player 1's piece
            }
            else if (board[row][col] == 2)
            {
                cout << "O"; // Player 2's piece
            }
            cout << " "; // Separate cells with space
        }
        cout << "\n"; // Move to the next row
    }

    // Print other information (e.g., current player, visits, unexplored moves)
    cout << "Current Player: " << currentPlayer << "\n";
    cout << "\n";
}

void GameState::applyMove(int move)
{
    // there should be empty row for the given column
    for (int row = 0; row < ROWS; row++)
    {
        if (board[row][move] == 0)
        {
            board[row][move] = currentPlayer;
            currentPlayer = currentPlayer == 1 ? 2 : 1;
            return;
        }
    }

    printGameState();
    throw invalid_argument("!" + to_string(move) + " is an invalid move!");
}

void GameState::applyMoveNoSwitch(int move)
{
    // there should be empty row for the given column
    for (int row = 0; row < ROWS; row++)
    {
        if (board[row][move] == 0)
        {
            board[row][move] = currentPlayer;
            return;
        }
    }

    printGameState();
    throw invalid_argument(move + " is an invalid move!");
}

int GameState::getValidMove() const
{
    int moves[7] = {0, 1, 2, 3, 4, 5, 6};
    vector<int> validMoves;

    for (int move : moves)
    {
        if (isValid(move))
            validMoves.push_back(move);
    }

    if (validMoves.size() == 0)
    {
        throw invalid_argument("no more valid moves left");
    }

    int randomIndex = rand() % validMoves.size();
    return validMoves.at(randomIndex);
}

double GameState::simulate(bool verbose)
{
    GameState start = GameState(board, currentPlayer);
    int move, player;
    while (!start.isDraw())
    {
        move = start.getValidMove();
        player = start.currentPlayer;
        start.applyMove(move);

        if (verbose)
        {
            cout << "After applying move " << move << " for player " << player << endl;
            start.printGameState();
        }

        if (start.isWin(player, move))
        {
            if (verbose)
                cout << "Player " << player << " won!" << endl;
            return player == 1 ? 1.0 : 0.0;
        }
    }

    if (verbose)
        cout << "It's a draw!" << endl;
    return 0.5;
}