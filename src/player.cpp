#include "player.hpp"
#include "heuristics/basic.hpp"
#include "search_alg/minimax.hpp"

/**
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    this->side = side;
}
Player::Player(Side side, bool testingMinimax) {
    this->side = side;
    ply = 20;
    if (testingMinimax) {
        h = new BasicHeuristic();
        s = new SearchMinimax(h);
        ply = 2;
    }
}

/**
 * Destructor for the player.
 */
Player::~Player() {
}

/**
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

    Move *m = new Move(0,0);

    current->doMove(((opponentsMove==nullptr)?Move(-1,-1):*opponentsMove), OTHER_SIDE(side));

    *m = s->search(current, msLeft, ply, side);

    if (current->checkMove(*m, side)) {
        current->doMove(*m,side);
    }

    return (m->isPass()?nullptr:m);
}
