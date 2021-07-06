# Chess!

It's bit hacks all the way down!

This is an ongoing project that emphasizes performance.

Most of the fun stuff happens in `{src, include}/player/Computer/`, where Alpha-Beta search governs the Computer player's decision process (take a look at `src/player/computer/search.cpp` for a Minimax/Alpha-Beta review).

---

## Directories

The sub-directories of `include`, `src`, and `test` all mirror each other. The first-level directories can be summarized as follows:

`board`: Contains everything needed to play the game; Square, Piece, Board, etc. are all maintained here.

`game`: Move generation and rule enforcement. Moves executed through the functions here are subject to the rules of chess.

`player`: Contains a basic Human player and the brains of a Computer player.

`util`: Various tools used throughout the codebase.
