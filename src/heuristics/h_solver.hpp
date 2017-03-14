#pragma once

#include "../heuristic.hpp"

namespace asimov {

    /**
     * @brief A binary heuristic used for the solver.
     *
     * Heuristic returns whether or not we've won on a given board state.
     */
    class SolverHeuristic : public Heuristic {
    private:
    public:
        SolverHeuristic();
        virtual ~SolverHeuristic();

        virtual float evaluate(BitBoard &b);
    };
}
