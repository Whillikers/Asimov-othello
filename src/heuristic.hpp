#pragma once

#include <utility>
#include "bitboard.hpp"

namespace asimov {

    /**
     * @brief an abstract class that represents a heuristic function.
     */
    class Heuristic {
    private:

    public:
        virtual ~Heuristic();

        /**
         * @brief Provides an evaluation of a board, positive for black advantage
         * negative for white advantage.
         */
        virtual float evaluate(BitBoard &b, Side s) = 0;

        /**
         * @brief Attempts to order moves based on how how good they potentially
         * are by assigning weights to each index.
         */
        virtual void order_moves(BitBoard &bd, Side s, int n, Move *rmvs, std::pair<float, int> *mvs);
    };
}
