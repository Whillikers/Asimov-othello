#pragma once

#include <utility>
#include "bitboard.hpp"

//include tensorflow information

#include "tensorflow/cc/ops/const_op.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"

namespace asimov {

    /**
     * @brief an abstract class that represents a heuristic function.
     */
    class MLHeuristic {
    private:
        std::unique_ptr<tensorflow::Session> value_sess, policy_sess;
        //std::unique_ptr<tensorflow::GraphDef> tf_value, tf_policy;
    public:
        MLHeuristic(char * value_net_path, char * policy_net_path);
        virtual ~MLHeuristic();

        /**
         * @brief Provides an evaluation of a board, positive for black advantage
         * negative for white advantage.
         */
        virtual float evaluate(BitBoard &b);

        /**
         * @brief Attempts to order moves based on how how good they potentially
         * are by assigning weights to each index.
         */
        virtual void order_moves(BitBoard &bd, int n, Move *rmvs, std::pair<float, int> *mvs);
    };
}
