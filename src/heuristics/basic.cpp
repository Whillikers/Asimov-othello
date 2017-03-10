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
float BasicHeuristic::evaluate(BitBoard &b) {
    return (float)(b.count_white() - b.count_black());
}
