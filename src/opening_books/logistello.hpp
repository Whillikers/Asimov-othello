#pragma once

#include "../opening.hpp"
#include <unordered_map>

/**
 * Generates a hash from a pair of numbers (in this case, a BoardNormalForm)
 * for use with a hash table.
 *
 * Using pair-hash solution from http://stackoverflow.com/questions/20590656/error-for-hash-function-of-pair-of-ints
 * and hash-combination solution from http://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes
 */
namespace std {
    struct boardhash {
    public:
        template <typename T, typename U>
        std::size_t operator()(const std::pair<T, U> &x) const
        {
            size_t first = std::hash<T>()(x.first);
            size_t second = std::hash<T>()(x.second);
            return (first<<1) + first + second;
        }
    };
}

namespace asimov {
    /**
     * @brief The opening book used by the Logistello Othello AI.
     * Taken from https://skatgame.net/mburo/log.html.
     *
     * File stores a list of games and their outcomes in delta-pieces at
     * endgame from Black's perspective. Games may be followed until a
     * deviation is found.
     */
    class BookLogistello : public OpeningBook {
        private:
            std::unordered_map<BoardNormalForm, Move, boardhash> table;
        public:
            BookLogistello();
            ~BookLogistello();
            Move nextMove(Board* b);
    };
}
