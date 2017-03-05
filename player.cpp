#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = true;

    
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
 * Sets the current game board.
 */
void Player::setBoard(Board *board) {
    b = board->copy();
}

/*
 * Helper function for computing the minimax decision tree. Given a board, 
 * recursively finds the move with the largest minimum score.
 * Also takes in a parameter depth, telling it how far to recurse.
 * Returns with the proper minimax move in the passed in parameter m.
 */
int Player::minimaxHelper(Board *temp_b, Side s, int depth, Move *m) {
    if (depth == 0) {
        // We are done recursing.
        if (s == Side::WHITE) {
            // Last move was black.
            return temp_b->countBlack() - temp_b->countWhite();
        }
        else {
            // Last move was white.
            return temp_b->countWhite() - temp_b->countBlack();
        }
    }
    else {
        int minimax = INT_MIN;           // Final minimax value.
        int min = INT_MIN;               // Min from each branch.
        Move *final_m = new Move(0, 0);  // Final move that results in minimax.
        Move unused_move(0, 0);          // Move that next player would choose is
                                         //   to be set in here, but we don't care.
        
        // Iterate through all possible moves, making the branches.
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move temp_move(i, j);
                if (temp_b->checkMove(&temp_move, s)) {
                    Board *temp_board = temp_b->copy();
                    temp_board->doMove(&temp_move, s);
                    
                    // Do the resursion with the other side and one less depth.
                    if (s == Side::WHITE)
                        min = minimaxHelper(temp_board, Side::BLACK, depth-1, &unused_move);
                    else
                        min = minimaxHelper(temp_board, Side::WHITE, depth-1, &unused_move);
                    
                    // Other player will choose the min for us. Now we choose our max.
                    if (min > minimax) {
                        minimax = min;
                        final_m->setX(temp_move.getX());
                        final_m->setY(temp_move.getY());
                    }
                    delete temp_board;
                }
            }
        }
        
        // Set the chosen move.
        m->setX(final_m->getX());
        m->setY(final_m->getY());
        
        // We also need to catch the case where we don't have any moves.
        if (s == Side::WHITE)
            min = minimaxHelper(temp_b, Side::BLACK, depth-1, &unused_move);
        else
            min = minimaxHelper(temp_b, Side::WHITE, depth-1, &unused_move);
        
        // In this case, the best course of action is to pass.
        if (min > minimax) {
            minimax = min;
            delete final_m;
            final_m = nullptr;
        }
        
        // Return the minimax score for player.
        return -1*minimax;        // Negate because its the opposite score for
                                  //   the other player.
    }
    
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
    
    if (testingMinimax) {
        b->doMove(opponentsMove, s_enemy);
        if (!b->hasMoves(s_player))
            return nullptr;
        else {
            Move *m = new Move(0, 0);
            minimaxHelper(b, s_player, TREE_DEPTH, m);
            b->doMove(m, s_player);
            return m;
        }
    }
    else {
        // Non tree version
        
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
    }
    return nullptr;
}

