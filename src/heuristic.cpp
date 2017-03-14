
#include "heuristic.hpp"

using namespace asimov;

Heuristic::~Heuristic() {

}

void Heuristic::order_moves(BitBoard &bd, Side s, int n, Move *rmvs, std::pair<float, int> *mvs) {
    //do no ordering by default.
    return;
}
