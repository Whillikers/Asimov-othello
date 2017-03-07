#pragma once

#include "search.hpp"

namespace asimov {
    class SearchMinimax : public Search {
    private:
    public:
        SearchMinimax(Heuristic *h) : Search(h) {
            this->h = h;
        }

        virtual Move search(Board *b, int max_time, int max_depth, Side turn);

        float minimax(Board *b, int d, Side turn);
    };
}
