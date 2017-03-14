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
    };

    /**
     * @brief Search class implementing the MTDf tree search algorithm.
     */
    class SearchAlphaBeta : public Search {
    private:
        float low, high;
    protected:
        TranspositionTable *table;
    public:
        SearchAlphaBeta(Heuristic *_h);
        SearchAlphaBeta(Heuristic *_h, float l, float h);
        ~SearchAlphaBeta();

        virtual Move search(BitBoard &b, int max_time, int max_depth, Side turn);

    };
}
