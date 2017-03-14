#pragma once

#include "heuristic.hpp"

namespace asimov {

	class SolverHeuristic : public Heuristic {
	private:
	public:
		SolverHeuristic();
		virtual float evaluate(BitBoard &b, Side s);
	};

}
