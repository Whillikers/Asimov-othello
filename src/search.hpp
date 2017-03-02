#pragma once

#include "board.hpp"

namespace asimov {

    enum SearchType {
        MTDF
    };

    class Search {
    private:
    public:
        Search();
        ~Search();

        Move search(Board *b, SearchType type, int max_time, int max_depth);

        float alpha_beta_search(Board *b, float a, float b, int d);

        float mtdf(Board b*, float guess, int d);
        Move mtdf_id(Board *b, int max_time);
    };
}
