#pragma once

#include "../search.hpp"

namespace asimov {
    class SearchMonteCarlo : public Search {
    private:
    public:
        SearchMonteCarlo(Heuristic *h) : Search(h) {
            this->h = h;
        }

        virtual Move search(Board *b, int max_time, int max_depth, Side turn);

        int simulate(Board *b, Side turn);
    };
}
