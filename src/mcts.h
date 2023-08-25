#pragma once

#include <unordered_map>

#include "config.h"
#include "state.h"
#include "mcts_node.h"

struct MCTS
{
    MCTSNode root;
    int iterations;
    // unordered_map<GameState, MCTSNode *> cache:

    MCTS(GameState, int);
    ~MCTS();
    MCTSNode *mcts();
    MCTSNode *getBestChild() const;
};

// // Define a hash function for GameState
// namespace std
// {
//     template <>
//     struct hash<GameState>
//     {
//         size_t operator()(const GameState &state) const
//         {
//             // Start with a seed value for the hash
//             size_t hashValue = 0;

//             // Combine the hash values of the board elements and currentPlayer
//             for (int i = 0; i < ROWS; i++)
//             {
//                 for (int j = 0; j < COLS; j++)
//                 {
//                     // Use a prime number to combine hash values for board elements
//                     hashValue = hashValue * 31 + hash<int>()(state.board[i][j]);
//                 }
//             }

//             // Combine the hash value for currentPlayer
//             hashValue = hashValue * 31 + hash<int>()(state.currentPlayer);

//             return hashValue;
//         }
//     };
// }