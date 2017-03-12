#include "minimax.hpp"
#include <iostream>
using namespace std;

using namespace asimov;

/**
 * @brief Searches for the best move using the minimax algorithm.
 *
 * @param b The board to start the search at.
 * @param max_time The maximum time in ms to search for (unused)
 * @param max_depth The maximum game tree depth to search to.
 * @param turn Which player is currently moving.
 *
 * @returns The best move found in the search time
 */
Move SearchMinimax::search(BitBoard &b, int max_time, int max_depth, Side turn) {
    int bst = 0;
    float g;
    int n = 0;
    Move mvs[MAX_MOVES];

    b.get_moves(turn, mvs, &n);
    if (n == 0) {
        return Move();
    }

    if (turn == WHITE) {
        g = -std::numeric_limits<float>::infinity();
        for (int i = 0; i < n; i++) {
            BitBoard bt = b;
            bt.do_move(mvs[i], turn);

            float v = minimax(bt, max_depth-1, OTHER_SIDE(turn));
            if (v > g) {
                g = v;
                bst = i;
            }
        }
    } else {
        g = std::numeric_limits<float>::infinity();
        for (int i = 0; i < n; i++) {
            BitBoard bt = b;
            bt.do_move(mvs[i], turn);

            float v = minimax(bt, max_depth-1, OTHER_SIDE(turn));
            if (v < g) {
                g = v;
                bst = i;
            }
        }
    }

    return mvs[bst];
}

/**
 * @brief Internal function used to search the game tree
 *
 * @param b The board to start the search at.
 * @param d The how many more layers to search.
 * @param turn Which player is currently moving.
 *
 * @returns The best minimax score of of the given board on the given turn.
 */
float SearchMinimax::minimax(BitBoard &b, int d, Side turn) {
    int n = 0;
    Move mvs[MAX_MOVES];

    if (d == 0 || b.is_done()) {
        return h->evaluate(b);
    }

    float g;

    b.get_moves(turn, mvs, &n);
    if (n == 0) {
        return minimax(b, d-1, OTHER_SIDE(turn));
    }

    if (turn == WHITE) {
        g = -std::numeric_limits<float>::infinity();
        for (int i = 0; i < n; i++) {
            BitBoard bt = b;
            bt.do_move(mvs[i], turn);

            g = max(g, minimax(bt, d-1, OTHER_SIDE(turn)));
        }
    } else {
        g = std::numeric_limits<float>::infinity();
        for (int i = 0; i < n; i++) {
            BitBoard bt = b;
            bt.do_move(mvs[i], turn);

            g = min(g, minimax(bt, d-1, OTHER_SIDE(turn)));
        }
    }

    return g;

}
