# Asimov-othello

## Building

NOTE: Makefile was heavily modified.

 * `make asimov` build release version of the asimov othello ai.
 * `make asimov-debug` make the debug version.
 * `make opening` build release version of the opening book generated.
 * `make opening-debug` make the debug version.

 ## Developers

 Aidan developed the opening book, and the endgame solver, as well as providing testing and feedback for various other parts of the program.
 Sam developed the search algorithms and heuristics, and also efficient player, board, and move implementations.

 ## Improvements

 Asimov uses an opening book to look up up to ten moves in for good openings; it's often hard to tell based on a simple heuristic how opening moves can affect a game later on, so we might as well save some computation time and look them up earlier.
 Midgame, it uses a monte-carlo search with a hand-tuned heuristic and a very efficient board implementation that allows us to search much deeper than we otherwise could.
At endgame, we deterministically solve to a great depth using minimax and a basic heuristic that just aims to win the game.
