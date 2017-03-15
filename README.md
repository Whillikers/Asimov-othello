# Asimov-othello

## Building

Building depends on having tensorflow installed, so I have prebuilt the program
and the dependencies. 

All one has to do is copy the script `asimov_player.sh` and the folder `asimov-ml_final_build`
to the same place, then the bot is run with `./asimov_player.sh`. 
 
## Developers

Aidan developed the opening book, and the endgame solver, as well as providing testing and feedback for various other parts of the program.
Sam developed the search algorithms and heuristics, and also efficient player, board, and move implementations. Sam also made an improved heuristic function using
rudimentary machine learning (with TensorFlow).

## Improvements

Asimov uses an opening book to look up up to ten moves in for good openings; it's often hard to tell based on a simple heuristic how opening moves can affect a game later on, so we might as well save some computation time and look them up earlier.
Midgame uses alpha-beta search with a trained neural net as a heuristic, and random move ordering.
At endgame, we deterministically solve to a great depth using minimax and a basic heuristic that just aims to win the game.
Other miscellaneous improvements include profiling and optimizing the bitboard implementation to use only bitwise operators to make traversing the gametree much faster.
