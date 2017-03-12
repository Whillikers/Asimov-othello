#include "player.hpp"
#include <gperftools/profiler.h>

using namespace asimov;

int main(int argc, char const *argv[]) {
    Player *one = new Player(BLACK);
    Player *two = new Player(WHITE);

    Side s = BLACK;
    Move *m = new Move();
    BitBoard b;

    cout << "Playing self" << endl;

    b.display(s);

    ProfilerStart("./profile.prf");

    while (!b.is_done()) {
        m = one->doMove(m, 0);
        if (m != nullptr) {
            b.do_move(*m, s);
        }
        s = OTHER_SIDE(s);

        b.display(s);

        m = two->doMove(m, 0);
        if (m != nullptr) {
            b.do_move(*m, s);
        }
        s = OTHER_SIDE(s);

        b.display(s);
    }

    ProfilerStop();

    return 0;
}
