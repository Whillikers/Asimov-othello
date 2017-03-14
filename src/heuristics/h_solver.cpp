#include "h_solver.hpp"

using namespace asimov;

SolverHeuristic::SolverHeuristic() {

}

SolverHeuristic::~SolverHeuristic() {

}

/**
 * @brief A heuristic which only cares which player has won the game.
 */
float SolverHeuristic::evaluate(BitBoard &b) {
    return (b.count_white() > b.count_black()) ? 1.0 : -1.0;
}
