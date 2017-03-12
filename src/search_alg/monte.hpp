#pragma once

#include "../search.hpp"

#define INFINITE_TIME 20000 // Backup timeLeft in case we have infinite time

namespace asimov {
    class SearchMonteCarlo : public Search {
    private:
    public:
        SearchMonteCarlo(Heuristic *h) : Search(h) {
            this->h = h;
        }

        virtual Move search(BitBoard &b, int max_time, int max_depth, Side turn);

        int simulate(BitBoard &b, Side turn);
    };
}
