#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <cmath>
#include <climits>
#include "common.hpp"
#include "board.hpp"

#define TREE_DEPTH 2

using namespace std;

class Player {
private:
    Board *b;
    Side s_player;
    Side s_enemy;

public:
    Player(Side side);
    ~Player();

    void setBoard(Board *board);
    int minimaxHelper(Board *temp_b, Side s, int depth, Move *m);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
