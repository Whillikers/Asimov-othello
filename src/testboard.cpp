#include <iostream>
#include "bitboard.hpp"

using namespace std;

int main(int argc, char* argv) {

    asimov::BitBoard b;
    vector<asimov::BitBoard> moves;

    Side turn = BLACK;

    b.display(turn);

    while (true) {
        char c;
        int x, y;
        Move m(-1,-1);
        cout << "m, u, q:" << endl << "> ";
        cin >> c;
        switch (c) {
        case 'm': case 'M':
            cout << "x,y:" << endl << "> ";
            cin >> x >> y;
            m = Move(x-1, y-1);
            if (b.check_move(m, turn)) {
                moves.push_back(b);
                b.do_move(m, turn);
                turn = OTHER_SIDE(turn);
                b.display(turn);
            }
            break;
        case 'u': case 'U':
            if (moves.size() > 0) {
                turn = OTHER_SIDE(turn);
                b = moves.back();
                moves.pop_back();
                b.display(turn);
            }
            break;
        case 'd': case 'D':
            b.display(turn);
            break;
        case 'q': case 'Q':
            return 0;
        }
    }
}
