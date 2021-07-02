// Copyright 2021 Alex Theimer

#include "player/computer/search.h"

#include <limits>
#include <random>
#include <cstdlib>

#include "util/buffer.h"

using board::Board;
using board::PieceColor;
using board::Piece;
using board::Square;

using game::Move;

/*
################################################################################
                         =========================
                         === Alpha-Beta Search ===
                         =========================

    (1) A Minimax Review

        Suppose two players, A and B, play a game of Chess. It's A's turn,
        and they mentally visualize each of the moves they might make.
        A is clever, though, and additionally visualizes how B might move
        according to A's move. Their thought "tree" is:

               current board -->         B1
                                       /    \       A moves
              after A's move -->     B2      B3
                                    / \      / \    B moves
              after B's move -->  B4   B5  B6   B7

        A knows that B4 is incredibly advantageous, but they also know B
        wouldn't be stupid enough to make the move to get there. B5 would
        be a terrible board for A, but B6 and B7 are acceptable; A opts
        to move to board B3.

        This is the main idea of a Minimax search: leaves of some
        search tree are evaluated according to some heuristic, then
        these values determine the value of parent nodes. For example,
        we could assign values according to the above case (brackets
        indicate heuristic values; parens indicate "aggregate" values):

                                      B1
                                    /    \           A moves
                           (-100) B2      B3 (5)
                                 / \      / \        B moves
                          [100] B4   B5  B6   B7 [5]
                                  [-100] [6]

        Notice the maximization / minimization occurring at each sequential
        depth. At the root node, A maximized their potential score from
        B's score-minimizing choices. In fact, you could classify nodes at
        each level as:

                    =========== B1 =========== <-- maximizing
                              /    \
                    ======= B2 ==== B3 ======= <-- minimizing
                           / \      / \
                    ==== B4 = B5  B6 = B7 ==== <-- leaf

        Additional turn lookahead simply adds additional levels that
        oscillate between "maximizing" and "minimizing."

    (2) The Alpha-Beta Difference

        The Minimax search space grows exponentially with turn lookahead.
        Alpha-Beta search is a more concrete implementation of Minimiax
        search that prunes away a (on average) substantial portion of
        the search space.

        Alpha-beta search evaluates the tree depth-first. When a node is
        first stepped into, it is passed two values: an "alpha" (the
        maximizing player's currently-known highest-possible score) and a
        "beta" (the maximizing player's currently-known smallest-possible
        score).

        When a maximizing node evaluates any one of its children as a
        value greater than the beta, then all search within that node's
        subtree is stopped; the minimizing player would never allow
        the maximizing player to reach that node.

        Similarly, search within a subtree is stopped when a minimizing
        node evaluates a child as a value less than the alpha; the
        maximizing player would never allow the game state to reach
        that subtree.

    (3) This Implementation

        alphaBetaSearch is the "root" call of the search tree. It will
        call alphaBetaSearchMin, which will call alphaBetaSearchMax,
        which calls alphaBetaSearchMin, etc. Each one of these calls
        acts as one of the nodes of the search tree.

        alphaBetaSearchMax and alphaBetaSearchMin are almost identical
        except for a few parameters. Therefore, these functions call
        the helper function alphaBetaSearchBase with their
        min/max-specific parameters.

################################################################################
*/

// evaluates `board` from the perspective of `color`
typedef int64_t (*BoardHeuristicFunc)(const Board& board, PieceColor color);

// signature of alphaBetaSearchMin and alphaBetaSearchMax
typedef int64_t (*AlphaBetaVariantFunc)(
                           Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic);

// These are passed as argument to alphaBetaSearchBase.
//    See definition for details.
typedef int64_t (*ScoreUpdateFunc)(int64_t score, int64_t child_score);
typedef bool (*ExitCondFunc)(int64_t alpha, int64_t beta, int64_t score);
typedef void (*BoundUpdateFunc)(int64_t* alpha, int64_t* beta, int64_t score);

// Forward-declare both of the variants because their definitions
//     reference each other.

/*
Maximizer variant of the search.

A "friendly" node in the search tree.

@param board: the board to evaluate.
@param color: the color of the maximizing player.
@param depth_remaining: must be >= 0
*/
int64_t alphaBetaSearchMax(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic);
/*
Minimizer variant of the search.

An "opponent" node of the search tree.

@param board: the board to evaluate.
@param color: the color of the maximizing player.
@param depth_remaining: must be >= 0
*/
int64_t alphaBetaSearchMin(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic);

/*
The "machinery" of the alphaBetaSearch variants.

@param board: the board to evaluate.
@param color: the color of the maximizing player.
@param depth_remaining: must be >= 0
@param heuristic_eval_color: the color "perspective" from which the board
           is evaluated. (i.e. if color == WHITE, the heuristic
           (should) assign greater value to WHITE-preferential boards.).
@param score_init: the initial value of the board's score.
@param child_eval_variant: either alphaBetaSearchMin or alphaBetaSearchMax.
           This is the function used to evaluate any child nodes.
@param score_update: given the current score and a child node's score,
           returns an updated score for the current node.
@param exit_cond: returns true iff children no longer need to be evaluated.
@param bound_update: updates alpha and/or beta.
*/
int64_t alphaBetaSearchBase(Board* board, PieceColor color,
                            std::size_t depth_remaining,
                            int64_t alpha, int64_t beta,
                            BoardHeuristicFunc board_heuristic,
                            PieceColor heuristic_eval_color,
                            int64_t score_init,
                            AlphaBetaVariantFunc child_eval_variant,
                            ScoreUpdateFunc score_update,
                            ExitCondFunc exit_cond,
                            BoundUpdateFunc bound_update) {
    ASSERT(depth_remaining >= 0,
            "must have non-negative depth_remaining; depth_remaining: "
            + std::to_string(depth_remaining));

    if (depth_remaining == 0) {
        // leaf node!
        return board_heuristic(*board, heuristic_eval_color);
    }

    util::Buffer<Move, game::MAX_NUM_MOVES_PLY> move_buffer;
    std::size_t num_moves =
            game::getAllMoves(*board, color, move_buffer.start());

    // TODO(theimer): unsure if this is actually needed
    if (num_moves == 0) {
        return board_heuristic(*board, heuristic_eval_color);
    }

    // start evaluating children...
    int64_t score = score_init;
    for (std::size_t i = 0; i < num_moves; ++i) {
        // Temporarily make a Move and store any "killed" opponent piece.
        Move move = move_buffer.get(i);
        std::optional<Piece> overwritten_piece_opt =
                game::makeMove(board, move);

        // Evaluate the Board that results from the Move.
        // Note: Children are evaluated with the opposite color
        //     and (depth_remaining - 1)
        int64_t child_score =
                child_eval_variant(board, board::oppositeColor(color),
                    depth_remaining - 1, alpha, beta, board_heuristic);

        // Update the current Board's score.
        score = score_update(score, child_score);

        // "unmake" the temporary move
        game::unmakeMove(board, move, overwritten_piece_opt);

        // check if an alpha/beta cutoff has been reached
        if (exit_cond(alpha, beta, score)) {
            break;
        }

        // no cutoff; simply update alpha and/or beta,
        //     then keep evaluating children
        bound_update(&alpha, &beta, score);
    }
    return score;
}


int64_t alphaBetaSearchMax(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic) {

    // Assume the worst-possible score.
    int64_t score_init = std::numeric_limits<int64_t>::min();

    // children are evaluated as minimizers (i.e. opponents)
    AlphaBetaVariantFunc child_eval_variant = alphaBetaSearchMin;

    // this node of the search wants to maximize its score
    ScoreUpdateFunc score_update = [](int64_t score, int64_t child_score){
        return std::max(score, child_score);
    };

    /*
    If the current node's score is at least beta (i.e. the minimum score at
    some minimizing parent node), then we're done evaluating this node and its
    children. The opponent can prevent the player from ever reaching this node,
    so we disregard it in the search.
    */
    ExitCondFunc exit_cond =  [](int64_t alpha, int64_t beta, int64_t score) {
        return score >= beta;
    };

    // store the "player's best-possible score"
    BoundUpdateFunc bound_update =
            [](int64_t* alpha, int64_t* beta, int64_t score) {
        *alpha = std::max(*alpha, score);
    };

    PieceColor heuristic_eval_color = color;

    return alphaBetaSearchBase(board, color, depth_remaining, alpha, beta,
                               board_heuristic, heuristic_eval_color,
                               score_init, child_eval_variant, score_update,
                               exit_cond, bound_update);
}


int64_t alphaBetaSearchMin(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic) {


    // Assume the opponent's worst-possible score (i.e. the max
    //     score possible for the player)
    int64_t score_init = std::numeric_limits<int64_t>::max();

    // children are evaluated as maximizers (i.e. the "player's" turn)
    AlphaBetaVariantFunc child_eval_variant = alphaBetaSearchMax;

    // these "opponent" nodes want to find the minimum-possible
    //     score for the player.
    ScoreUpdateFunc score_update = [](int64_t score, int64_t child_score){
        return std::min(score, child_score);
    };

    /*
    If the current node's score is less than alpha (i.e. the maximum score at
    some maximizing parent node), then we're done evaluating this node and its
    children. The maximizer (i.e. non-opponent player) can prevent the
    minimizer from ever reaching this node, so we disregard it in the search.
    */
    ExitCondFunc exit_cond =  [](int64_t alpha, int64_t beta, int64_t score) {
        return score <= alpha;
    };


    // store the "player's worst-possible score"
    BoundUpdateFunc bound_update =
            [](int64_t* alpha, int64_t* beta, int64_t score) {
        *beta = std::min(*beta, score);
    };

    // evaluate the board from the perspective of the maximizing player--
    //     this is the score the minimizer wants to minimize.
    PieceColor heuristic_eval_color = board::oppositeColor(color);

    return alphaBetaSearchBase(board, color, depth_remaining, alpha, beta,
                               board_heuristic, heuristic_eval_color,
                               score_init, child_eval_variant, score_update,
                               exit_cond, bound_update);
}

/*
Also see the documentation in the header file.

This is the "root" call of the search tree (i.e. a "friendly" node).
*/
Move player::computer::alphaBetaSearch(
                           const Board& board, PieceColor color,
                           std::size_t depth,
                           BoardHeuristicFunc board_heuristic) {

    // this implementation is different enough from the alphaBetaSearch
    //     variants that it isn't processed thru  alphaBetaSearchBase

    ASSERT(depth > 0,
            "must have positive depth; depth: " + std::to_string(depth));

    // Doesn't make sense to accept a Board* for a search function,
    //     so we copy-construct a non-const version here.
    Board board_copy(board);

    util::Buffer<Move, game::MAX_NUM_MOVES_PLY> move_buffer;
    std::size_t num_moves =
            game::getAllMoves(board_copy, color, move_buffer.start());

    // This is nearly the same implementation as alphaBetaSearchMax,
    //     but all highest-scoring moves are stored in a vector.
    std::vector<Move> best_moves;
    int64_t alpha = std::numeric_limits<int64_t>::min();
    for (std::size_t i = 0; i < num_moves; ++i) {
        Move move = move_buffer.get(i);
        std::optional<Piece> overwritten_opt =
                game::makeMove(&board_copy, move);
        int64_t score = alphaBetaSearchMin(
                             &board_copy, board::oppositeColor(color),
                             depth - 1,
                             alpha,
                             std::numeric_limits<int64_t>::max(),  // beta
                             board_heuristic);
        game::unmakeMove(&board_copy, move, overwritten_opt);
        if (score > alpha) {
            // new highest score found; clear out the others.
            alpha = score;
            best_moves.clear();
            best_moves.push_back(move);
        } else if (score == alpha) {
            best_moves.push_back(move);
        }
    }

    // choose randomly from the vector, since they're all equally good
    return best_moves[rand() % best_moves.size()];
}

int64_t player::computer::basicBoardHeuristic(const Board& board,
                                              PieceColor color) {
    // just the negative count of the opponent pieces
    util::Buffer<Square, Board::SIZE> dummy_buffer;
    return -board.getOccupiedSquares(board::oppositeColor(color),
                                     dummy_buffer.start());
}
