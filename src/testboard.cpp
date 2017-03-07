#include <iostream>
#include "board.hpp"

using namespace std;

int main(int argc, char* argv) {

    Board b;
    vector<MoveResult> moves;

    Side turn = WHITE;

    b.display();

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
            if (b.checkMove(m, turn)) {
                moves.push_back(b.doMove(m, turn));
                turn = OTHER_SIDE(turn);
            }
            b.display();
            break;
        case 'u': case 'U':
            if (moves.size() > 0) {
                turn = OTHER_SIDE(turn);
                b.undoMove(moves.back(), turn);
                moves.pop_back();
            }
            b.display();
            break;
        case 'q': case 'Q':
            return 0;
        }
    }
}
