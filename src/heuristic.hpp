#pragma once

#include "board.hpp"

namespace asimov {

    /**
     * @brief an abstract class that represents a heuristic function.
     */
    class Heuristic {
    private:

    public:
        virtual ~Heuristic();
        virtual float evaluate(Board *b) = 0;
    };
}
