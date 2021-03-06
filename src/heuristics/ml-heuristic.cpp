// #include "ml-heuristic.hpp"

// #define U64BIT_TO_FLOAT(u,n)        ((float)((u>>n)&1))

// #include <fstream>
// #include <vector>
// #include <string>

// #include "tensorflow/core/framework/graph.pb.h"
// #include "tensorflow/core/framework/tensor.h"
// #include "tensorflow/core/framework/tensor.pb.h"
// #include "tensorflow/core/graph/default_device.h"
// #include "tensorflow/core/graph/graph_def_builder.h"
// #include "tensorflow/core/lib/core/errors.h"
// #include "tensorflow/core/lib/core/stringpiece.h"
// #include "tensorflow/core/lib/core/threadpool.h"
// #include "tensorflow/core/lib/io/path.h"
// #include "tensorflow/core/lib/strings/stringprintf.h"
// #include "tensorflow/core/platform/init_main.h"
// #include "tensorflow/core/platform/logging.h"
// #include "tensorflow/core/platform/types.h"
// #include "tensorflow/core/public/session.h"

// using namespace asimov;

// using tensorflow::Tensor;
// using tensorflow::Status;
// using tensorflow::string;
// using tensorflow::int32;


// [>
 // * Code to load the NNETs, based on example code at:
// https://github.com/tensorflow/tensorflow/blob/master/tensorflow/examples/label_image/main.cc
 // */

// Status load_graph(string graph_file_name,
                // std::unique_ptr<tensorflow::Session>* session) {
    // tensorflow::GraphDef graph_def;
    // Status load_graph_status =
    // ReadBinaryProto(tensorflow::Env::Default(), graph_file_name, &graph_def);
    // if (!load_graph_status.ok()) {
        // return tensorflow::errors::NotFound("Failed to load compute graph at '",
                                            // graph_file_name, "'");
    // }
    // session->reset(tensorflow::NewSession(tensorflow::SessionOptions()));
    // Status session_create_status = (*session)->Create(graph_def);
    // if (!session_create_status.ok()) {
        // return session_create_status;
    // }
    // return Status::OK();
// }

// MLHeuristic::MLHeuristic(std::string value_net_path, std::string policy_net_path) {
    // //load graph
    // Status rt = load_graph("./Asimov_value_net.meta", &value_sess);
    // if (!rt.ok()) {
        // LOG(ERROR) << rt;
    // }

    // rt = load_graph("./Asimov_policy_net.meta", &policy_sess);
    // if (!rt.ok()) {
        // LOG(ERROR) << rt;
    // }
// }

// MLHeuristic::~MLHeuristic() {
    // value_sess->Close();
    // policy_sess->Close();
// }

/**
 * @brief Provides an evaluation of a board, positive for black advantage
 * negative for white advantage.
 */
// float MLHeuristic::evaluate(BitBoard &b, Side s) {
    // Tensor bb(tensorflow::DT_FLOAT, tensorflow::TensorShape({1,192}));

    // u64 bb1 = b.pieces(BLACK),
        // bb2 = b.pieces(WHITE),
        // bb3 = b.mobility(s);

    // float buf[192];
    // for (size_t i = 0; i < 64; i++) {
        // buf[i]     = U64BIT_TO_FLOAT(bb1,i);
        // buf[i+64]  = U64BIT_TO_FLOAT(bb2,i);
        // buf[i+128] = U64BIT_TO_FLOAT(bb3,i);
    // }

    // //bb.FromProto(buf);

    // std::vector<Tensor> outputs;
    // value_sess->Run({{"Placeholder:0", bb}}, {"Tanh_2:0"}, {}, &outputs);

    // return outputs[0].scalar<float>()();
// }

/**
 * @brief Attempts to order moves based on how how good they potentially
 * are by assigning weights to each index.
 */
// void MLHeuristic::order_moves(BitBoard &b, Side s, int n, Move *rmvs, std::pair<float, int> *mvs) {
    // Tensor bb(tensorflow::DT_FLOAT, tensorflow::TensorShape({1,8,8,4}));

    // float buf[256];
    // u64 bp = b.pieces(BLACK),
        // wp = b.pieces(WHITE),
        // bs = b.stability(BLACK),
        // ws = b.stability(WHITE);

    // for (size_t i = 0; i < 64; i++) {
        // buf[4*i]   = U64BIT_TO_FLOAT(bp,i);
        // buf[4*i+1] = U64BIT_TO_FLOAT(wp,i);
        // buf[4*i+2] = U64BIT_TO_FLOAT(bs,i);
        // buf[4*i+3] = U64BIT_TO_FLOAT(ws,i);
    // }

    // //bb.FromProto(buf);

    // std::vector<Tensor> outputs;
    // value_sess->Run({{"Placeholder:0", bb}}, {"Tanh_2:0"}, {}, &outputs);

    // //TODO: need to add weights to mvs
// }
