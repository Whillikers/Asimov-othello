#include "value_policy_player.hpp"
#include <algorithm>

#define SEARCH_DEPTH    (7)
#define MONTE_ITERS     (3)
#define INF             (std::numeric_limits<float>::infinity())

#define SET_MAX(a, b, q, r)     {if (q > a) {a = q; b = r;}}
#define SET_MIN(a, b, q, r)     {if (q < a) {a = q; b = r;}}

using namespace std;
using namespace asimov;

bool pair_less(pair<float, int> a, pair<float, int> b) {
    return a.first < b.first;
}
bool pair_more(pair<float, int> a, pair<float, int> b) {
    return a.first > b.first;
}

namespace middle_game {
    Move search_middle_game(BitBoard &b, int max_time, int max_depth, Side turn);
}

Move* ValuePolicyPlayer::do_move(Move* opponent_move, int ms_left) {

}

namespace middle_game {
    int simulate(BitBoard &b, Side turn) {
        int n, i = 0;
        Move mvs[MAX_MOVES];

        BitBoard bt = b;

        while (!bt.is_done()) {

            bt.get_moves(turn, mvs, &n);
            if (n != 0) {
                int ri = abs(rand())%n;

                bt.do_move(mvs[ri], turn);
            }
            turn = OTHER_SIDE(turn);
            i++;
        }
        int cw = bt.count_white();
        int cb = bt.count_black();
        return cw-cb;
    }


    float alpha_beta_simple(BitBoard &bd, float a, float b, int d, Side turn) {
        int n = 0;
        Move mvs[MAX_MOVES];

        if (bd.is_done()) {
            return bd.count_white()-bd.count_black();
        }

        if (d == 0) {
            float total = 0;
            for (int i = 0; i < MONTE_ITERS; i++) {
                total += simulate(bd, turn);
            }
            return total/((float)MONTE_ITERS);
        }

        float g;

        bd.get_moves(turn, mvs, &n);
        if (n == 0) {
            //pass if there are no moves
            return alpha_beta_simple(bd, a, b, d-1, OTHER_SIDE(turn));
        }

        if (turn == WHITE) {
            g = -INF;
            float alpha = a;
            for (int i = 0; i < n && g < b; i++) {

                BitBoard bt = bd;
                bt.do_move(mvs[i], turn);

                float v = alpha_beta_simple(bt, alpha, b, d-1, OTHER_SIDE(turn));

                g = max(g, v);
                alpha = max(alpha, g);
            }
        } else {
            g = INF;
            float beta = b;
            for (int i = 0; i < n && a < g; i++) {

                BitBoard bt = bd;
                bt.do_move(mvs[i], turn);

                float v = alpha_beta_simple(bt, a, beta, d-1, OTHER_SIDE(turn));

                g = min(g, v);
                beta = min(beta, g);
            }
        }

        return g;
    }

    float alpha_beta_search(BitBoard &bd, float a, float b, int d, Side turn) {
        int n = 0;
        Move rmvs[MAX_MOVES];
        pair<float, int> mvs[MAX_MOVES];

        if (bd.is_done()) {
            return (bd.count_white()-bd.count_black());
        }

        if (d == 0) {
            float total = 0;
            for (int i = 0; i < MONTE_ITERS; i++) {
                total += simulate(bd, turn);
            }
            return total/((float)MONTE_ITERS);
        }

        float g;

        bd.get_moves(turn, rmvs, &n);
        if (n == 0) {
            //pass if there are no moves
            return alpha_beta_search(bd, a, b, d-1, OTHER_SIDE(turn));
        }

        //seed moves and sort them
        int test_depth = max(0, min(3, d-3));
        for (int i = 0; i < n; i++) {
            BitBoard bt = bd;
            bt.do_move(rmvs[i], turn);
            //run alpha beta on some depth to get an estimate
            float r;
            if (d % 2 == 0) {
                r = alpha_beta_simple(
                    bt, a, b, test_depth, OTHER_SIDE(turn)
                );
            } else {
                r = i;
            }
            mvs[i] = make_pair(r, i);
        }


        if (turn == WHITE) {
            g = -INF;
            //sort the moves by their estimated score
            sort(mvs, mvs+(n+1), pair_more);
            float alpha = a;
            for (int i = 0; i < n && alpha < b; i++) {
                BitBoard btmp = bd;
                btmp.do_move(rmvs[mvs[i].second], turn);

                float v = alpha_beta_search(btmp, alpha, b, d-1, OTHER_SIDE(turn));

                g = max(g, v);
                alpha = max(alpha, g);
            }
        } else {
            g = INF;
            //sort the moves by their estimated score
            sort(mvs, mvs+(n+1), pair_less);
            float beta = b;
            for (int i = 0; i < n && a < beta; i++) {
                BitBoard btmp = bd;
                btmp.do_move(rmvs[mvs[i].second], turn);

                float v = alpha_beta_search(btmp, a, beta, d-1, OTHER_SIDE(turn));

                g = min(g, v);
                beta = min(beta, g);
            }
        }

        return g;
    }


    /**
     * @brief Implementation of iterative deepening alpha-beta tree search algorithm.
     */
    Move search_middle_game(BitBoard &b, int max_time, int max_depth, Side turn) {

        int best = 0, n = 0;
        float g;
        Move mvs[MAX_MOVES];

        b.get_moves(turn, mvs, &n);
        if (n == 0) {
            //return pass if there are no moves
            return Move();
        }

        if (turn == WHITE) {

            float alpha = g = -INF;
            for (int i = 0; i < n; i++) {
                BitBoard btmp = b;
                btmp.do_move(mvs[i], turn);

                float v = alpha_beta_search(btmp, alpha, INF, SEARCH_DEPTH, OTHER_SIDE(turn));

                SET_MAX(g, best, v, i);
                alpha = max(alpha, g);
            }
        } else {
            float beta = g = INF;
            for (int i = 0; i < n; i++) {
                BitBoard btmp = b;
                btmp.do_move(mvs[i], turn);

                float v = alpha_beta_search(btmp, -INF, beta, SEARCH_DEPTH, OTHER_SIDE(turn));

                SET_MIN(g, best, v, i);
                beta = min(beta, g);
            }
        }
        return mvs[best];
    }
}
