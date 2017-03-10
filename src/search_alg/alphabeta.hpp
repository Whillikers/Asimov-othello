#pragma once

#include "../search.hpp"
#include <utility>
#include <iostream>

using namespace std;

namespace asimov {

    struct ABResult {
        Move m;
        float score;

        ABResult(Move _m, float s) {
            m = _m;
            score = s;
        }
    }

    /**
     * @brief Search class implementing the MTDf tree search algorithm.
     */
    class SearchAlphaBeta : public Search {
    private:

    protected:
        TranspositionTable *table;
    public:
        SearchAlphaBeta(Heuristic *_h);
        ~SearchAlphaBeta();

        virtual Move search(BitBoard &b, int max_time, int max_depth, Side turn);

        ABResult alpha_beta_search(BitBoard &bd, float a, float b, int d, Side turn);
    };
}
