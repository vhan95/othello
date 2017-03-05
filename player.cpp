#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    b = new Board();
    s_player = side;
    if (side == WHITE)
        s_enemy = Side::BLACK;
    else
        s_enemy = Side::WHITE;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete b;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    b->doMove(opponentsMove, s_enemy);
    if (!b->hasMoves(s_player))
        return nullptr;
    else {
        // Heuristic to beat simple player is to play as away from center as possible.
        int max_dist = -1;
        Move *m = new Move(0, 0);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move temp_move(i, j);
                if (b->checkMove(&temp_move, s_player)) {
                    int dist = abs(i-3) + abs(j-3); // 3 is approximately the center
                    if (dist > max_dist) {
                        max_dist = dist;
                        m->setX(i);
                        m->setY(j);
                    }
                }
            }
        }
        b->doMove(m, s_player);
        return m;
    }
    return nullptr;
}
