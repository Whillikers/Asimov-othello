#include "transposition.hpp"

using namespace asimov;

TranspositionTable::TranspositionTable(int max);

/**
 * @brief Gets an entry from the transposition table.
 *
 * @param b The board state to try to find.
 * @param value The returned value is written to the pointer stored here, if
 *  not nullptr.
 *
 * @returns whether or not an entry was found for the board.
 */
bool TranspositionTable::fetch(Board * b, float * value) {
    return false;
}

/**
 * @brief Clears the transposition table.
 */
void TranspositionTable::clear() {

}
