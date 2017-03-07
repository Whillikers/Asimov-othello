#include "basic.hpp"

using namespace asimov;

BasicHeuristic::BasicHeuristic() {

}

BasicHeuristic::~BasicHeuristic() {

}

/**
 * @brief BasicHeuristic is very basic and returns the difference in the pieces
 *  white has on the board and the pieces black has on the board.
 */
float BasicHeuristic::evaluate(Board *b) {
    return (float)(b->countWhite() - b->countBlack());
}
