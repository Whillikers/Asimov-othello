#include "h_solver.hpp"

using namespace asimov;

SolverHeuristic::SolverHeuristic() {
}

SolverHeuristic::~SolverHeuristic() {}

float SolverHeuristic::evaluate(BitBoard &b, Side s) {
	int dif = b.count_black()-b.count_white();

	if (dif == 0) {
		return dif;
	} else if (dif > 0) {
		return 1;
	} else {
		return -1;
	}
}
