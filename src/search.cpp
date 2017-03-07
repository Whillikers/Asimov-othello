#include "search.hpp"

using namespace asimov;

Search::Search(Heuristic * _h) {
    h = _h;
}

Search::~Search() {
}
