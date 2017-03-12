#pragma once

#include <vector>
#include "common.hpp"

using namespace std;

typedef unsigned long long u64;

namespace asimov {
    class BitBoard {
    private:
        union data {
            char rows[8];
            u64 bitmask;
        };
        data black, white, bmoves, wmoves;
    protected:
        bool onBoard(int x, int y);
        bool occupied(int x, int y);
        void set(Side s, int x, int y);
        void flip(int x, int y);
        void unset(int x, int y);
        u64  find_moves(u64 gen, u64 pro);
    public:
        bool get(Side s, int x, int y);

        BitBoard();

        bool is_done();
        void find_moves(Side s);
        bool has_moves(Side s);
        bool check_move(Move m, Side s);
        MoveResult do_move(Move m, Side s);
        void undo_move(MoveResult m, Side s);
        void get_moves(Side s, Move * arr, int * n);
        int count(Side s);
        int count_black();
        int count_white();

        int count_moves(Side s);

        BoardNormalForm to_normal_form();

        void display(Side s);
    };
}
