#pragma once

#include "search.hpp"
#include <utility>

using namespace std;

namespace asimov {

    /**
     * @brief Search class implementing the MTDf tree search algorithm.
     */
    class SearchMTDf : public Search {
    private:

    protected:
        TranspositionTable *table;
    public:
        SearchMTDf(Heuristic *_h);
        ~SearchMTDf();

        virtual Move search(Board *b, int max_time, int max_depth, Side turn);

        float alpha_beta_search(Board *bd, float a, float b, int d, Side turn);

        pair<Move, float> mtdf(Board *b, float guess, int d, Side turn);
        Move mtdf_id(Board *b, int max_time, int max_depth, Side side);
    };
}
