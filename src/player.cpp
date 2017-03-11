#include "player.hpp"
#include "heuristics/basic.hpp"
#include "heuristics/better1.hpp"
#include "search_alg/mtdf.hpp"
#include "search_alg/minimax.hpp"
#include "search_alg/monte.hpp"
#include "search_alg/alphabeta.hpp"

/**
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    this->side = side;
    ply = 20;
    h = new Better1Heuristic();
    s = new SearchAlphaBeta(h);
}
Player::Player(Side side, bool testingMinimax) {
    this->side = side;
    ply = 5;
    h = new BasicHeuristic();
    s = new SearchMinimax(h);
    if (testingMinimax) {
        ply = 2;
    } else {
    }
}

/**
 * Destructor for the player.
 */
Player::~Player() {
    if (s == nullptr) {delete s;}
    if (h == nullptr) {delete h;}
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

    if (s == nullptr) {
        return nullptr;
    }

    Move *m = new Move();

    if (opponentsMove != nullptr) {
        current.do_move(*opponentsMove, OTHER_SIDE(side));
    }

    *m = s->search(current, msLeft, ply, side);

    if (current.check_move(*m, side)) {
        current.do_move(*m,side);
    }

    return ((m->isPass())? nullptr :m);
}
