#pragma once

#include"heuristic.hpp"
#include"transposition.hpp"
#include"board.hpp"
#include <utility>

//When iterative deepening is about to quit on timeout, but a sudden drop in
//score is detected, we want to continue searching one more depth.
//currently an arbitrary value.
#define ID_DROP_THRESHOLD   10.5

namespace asimov {

    class Search {
    private:
    protected:
        Heuristic *h;
    public:
        Search(Heuristic * _h);
        virtual ~Search();

        virtual Move search(Board *b, int max_time, int max_depth, Side turn) = 0;
    };
}
