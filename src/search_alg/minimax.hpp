#pragma once

#include "../search.hpp"

namespace asimov {
    class SearchMinimax : public Search {
    private:
    public:
        SearchMinimax(Heuristic *h) : Search(h) {
            this->h = h;
        }

        virtual Move search(BitBoard &b, int max_time, int max_depth, Side turn);

        float minimax(BitBoard &b, int d, Side turn);
    };
}
