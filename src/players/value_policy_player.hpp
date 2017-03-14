#pragma once
#include "common.hpp"
#include "bitboard.hpp"

namespace asimov {

    class ValuePolicyPlayer {
    private:
        tensorflow::Session tf_sess;
        tensorflow::Scope value_net;
        tensorflow::Scope policy_net;
    public:
        Move* do_move(Move* opponent_move, int ms_left);
    };
}
