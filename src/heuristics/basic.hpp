#pragma once

#include "../heuristic.hpp"

namespace asimov {

    /**
     * @brief The most basic heuristic
     *
     * heuristic is based on the difference in number of stones on the board
     * at any given time.
     */
    class BasicHeuristic : public Heuristic {
    private:
    public:
        BasicHeuristic();
        virtual ~BasicHeuristic();

        virtual float evaluate(BitBoard &b);
    };
}
