#include "search.hpp"
#include "heuristic.hpp"
#include <limits>

using namespace asimov;

Search::Search() {

}

/**
 * @brief Searches for the best move.
 *
 * @param b The board to search from.
 * @param type The type of search algorithm to use.
 *
 * @returns The best move found.
 */
Move Search::search(Board *b, SearchType type, int max_time, int max_depth) {
    switch(type) {
    case MTDF:
        break;
    default:
        break;
    }
    return Move(0,0);
}

/**
 * @brief Searches using MTD(f)
 *
 * @param b The board to search from.
 *
 * @returns The best move found.
 */
Move Search::mtdf_id(Board *b, int max_time, int max_depth, Side side) {
    return Move(0,0);
}

float Search::mtdf(Board *b, float guess, int d) {
    return 0.0;
}

float Search::alpha_beta_search(Board *b, float a, float b) {
    return 0.0;
}
