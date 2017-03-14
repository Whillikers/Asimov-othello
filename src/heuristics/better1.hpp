#pragma once

#include "../heuristic.hpp"

namespace asimov {

    /**
     * @brief The most basic heuristic
     *
     * heuristic is based on the difference in number of stones on the board
     * at any given time.
     */
    class Better1Heuristic : public Heuristic {
    private:
    public:
        Better1Heuristic();
        virtual ~Better1Heuristic();

        virtual float evaluate(BitBoard &b, Side s);
    };
}
