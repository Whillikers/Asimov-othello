#include "search_alg/mtdf.hpp"
#include "asimov_common.hpp"

#include <vector>

using namespace std;
using namespace asimov;

SearchMTDf::SearchMTDf(Heuristic *_h) : Search(_h) {
    table = new TranspositionTable(MAX_TRANSPOSITION_SIZE);
}

SearchMTDf::~SearchMTDf() {
    table->clear();
    delete table;
}

/**
 * @brief Implementation of iterative deepening MTDf algorithm.
 */
Move SearchMTDf::search(Board *b, int max_time, int max_depth, Side turn) {
    int d = 1;
    float sc, guess;
    time_t start, current;
    Move mbst(0,0);

    sc = TURN_MAX(turn);
    guess = 0.0;

    current = time(&start);
    do {
        pair<Move, float> r = this->mtdf(b, guess, d, turn);

        guess = r.second;
        mbst = r.first;

        time(&current);
        d++;
    } while (difftime(current, start) < max_time && d < max_depth);

    return mbst;
}

/**
 * @brief Implementation of fixed depth MTDf algorithm.
 */
pair<Move, float> SearchMTDf::mtdf(Board *b, float guess, int d, Side turn) {
    int mbst;
    float bst, low, high, beta, sc;


    low = bst = -(high = std::numeric_limits<float>::infinity());
    sc = TURN_MAX(turn);

    vector<Move> mvs = b->getMoves(turn);
    vector<Board*> next_b(mvs.size());

    table->clear();

    //compute and allocated moves once
    for (int i = 0; i < mvs.size(); i++) {
        Board* nb = b->copy();
        Move mv = mvs[i];
        nb->doMove(&mv, turn);
        next_b[i] = nb;
    }

    while (low < high) {
        bst = -std::numeric_limits<float>::infinity();

        beta = guess;
        if (guess == low) {
            beta += 1.0;
        }

        for (int i = 0; i < next_b.size(); i++) {

            float v = sc * alpha_beta_search(next_b[i], beta-1.0, beta, d, OTHER_SIDE(turn));

            if (v > bst) {
                bst = v;
                mbst = i;
            }
        }

        guess = bst;

        if (guess < beta) {
            high = guess;
        } else {
            low = guess;
        }
    }

    //delete next moves.
    for (int i = 0; i < next_b.size(); i++) {
        delete next_b[i];
    }

    return make_pair(mvs[mbst], guess);
}

/**
 * @brief Alpha beta minimax search "with memory" (a.k.a with a transposition
 * table).
 */
float SearchMTDf::alpha_beta_search(Board *bd, float a, float b, int d, Side turn) {

    //retrieve from transposition table
    TableValue entry;
    if (table != nullptr) {
        if (table->fetch(bd, &entry)) {
            if (entry.lowerbound <= b) {return entry.lowerbound;}
            if (entry.upperbound <= a) {return entry.upperbound;}
            a = max(a, entry.lowerbound);
            b = min(b, entry.upperbound);
        }
    }

    if (d == 0) {
        return this->h->evaluate(bd);
    }

    float sc, g;

    sc = TURN_MAX(turn);
    g = std::numeric_limits<float>::infinity();
    vector<Move> mvs = bd->getMoves(turn);

    if (turn == WHITE) {
        float alpha = a;
        for (int i = 0; i < mvs.size() && a < b; i++) {
            Board* nb = bd->copy();
            Move m = mvs[i];
            nb->doMove(&(mvs[i]), turn);

            g = max(g, this->alpha_beta_search(nb, alpha, b, d-1, OTHER_SIDE(turn)));
            alpha = max(alpha, g);

            delete nb;
        }
    } else {
        float beta = b;
        for (int i = 0; i < mvs.size() && a < b; i++) {
            Board* nb = bd->copy();
            nb->doMove(&(mvs[i]), turn);

            g = min(g, this->alpha_beta_search(nb, a, beta, d-1, OTHER_SIDE(turn)));
            beta = min(beta, g);

            delete nb;
        }
    }

    //update bounds
    if (g <= a) {entry.upperbound = g;}
    if (g >= b) {entry.lowerbound = g;}
    if (g > a && g < b) {
        entry.upperbound = g;
        entry.lowerbound = g;
    }

    if (table != nullptr) {
        table->store(bd, entry);
    }

    return g;
}
