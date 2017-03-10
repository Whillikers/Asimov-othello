#include "transposition.hpp"

using namespace asimov;

TranspositionTable::TranspositionTable(int max) {
    current = numeric_limits<unsigned long long>::max();
}

/**
 * @brief Inserts an entry into the transposition table.
 *
 * If there is not space for the entry, we remove the oldest entry and replace
 * it.
 *
 * @param b The board state to try to find.
 * @param value The returned value is written to the pointer stored here, if
 *  not nullptr.
 *
 * @returns whether or not an entry was found for the board.
 */
void TranspositionTable::store(BitBoard &b, TableValue v) {
    TranspositionKey t = b.to_normal_form();
    if (entries.find(t) == entries.end()) {
        entries[t] = v;
        current--;
        max_size--;
    } else if (max_size <= 0){
        /// TODO: remove the old entry, insert new one in its place
    } else {
        entries[t] = v;
    }
}

/**
 * @brief Gets an entry from the transposition table.
 *
 * @param b The board state to try to find.
 * @param value The returned value is written to the pointer stored here, if
 *  not nullptr.
 *
 * @returns whether or not an entry was found for the board.
 */
bool TranspositionTable::fetch(BitBoard &b, TableValue * value) {
    TranspositionKey t = b.to_normal_form();
    if (entries.find(t) == entries.end()) {
        return false;
    } else if (value != nullptr){
        *value = entries[t];
    }
    return true;
}

/**
 * @brief Clears the transposition table.
 */
void TranspositionTable::clear() {
    entries.clear();
}
