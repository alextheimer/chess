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
TODO(theimer): explain; also note how similar these are
################################################################################
*/

typedef int64_t (*BoardHeuristicFunc)(const Board& board, PieceColor color);

typedef int64_t (*AlphaBetaVariantFunc)(
                           Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic);

typedef int64_t (*ScoreUpdateFunc)(int64_t score, int64_t child_score);

typedef bool (*ExitCondFunc)(int64_t alpha, int64_t beta, int64_t score);

typedef void (*BoundUpdateFunc)(int64_t* alpha, int64_t* beta, int64_t score);

// forward-declare both of these because their definitions
//     reference each other
int64_t alphaBetaSearchMax(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic);
int64_t alphaBetaSearchMin(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic);

/* TODO(theimer) */
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
    if (depth_remaining == 0) {
        return board_heuristic(*board, heuristic_eval_color);
    }

    util::Buffer<Move, game::MAX_NUM_MOVES_PLY> move_buffer;
    std::size_t num_moves =
            game::getAllMoves(*board, color, move_buffer.start());

    // note that this might happen if TODO
    if (num_moves == 0) {
        return board_heuristic(*board, heuristic_eval_color);
    }

    int64_t score = score_init;
    for (int i = 0; i < static_cast<int>(num_moves); ++i) {
        Move move = move_buffer.get(i);

        std::optional<Piece> overwritten_opt =
                game::makeMove(board, move);

        int64_t child_score =
                child_eval_variant(board, board::oppositeColor(color),
                    depth_remaining - 1, alpha, beta, board_heuristic);
        score = score_update(score, child_score);

        game::unmakeMove(board, move, overwritten_opt);

        if (exit_cond(alpha, beta, score)) {
            break;
        }

        bound_update(&alpha, &beta, score);
    }
    return score;
}

/*
Maximizer variant of the search.

A "friendly" node in the tree.
*/
int64_t alphaBetaSearchMax(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic) {

    int64_t score_init = std::numeric_limits<int64_t>::min();

    AlphaBetaVariantFunc child_eval_variant = alphaBetaSearchMin;

    ScoreUpdateFunc score_update = [](int64_t score, int64_t child_score){
        return std::max(score, child_score);
    };

    ExitCondFunc exit_cond =  [](int64_t alpha, int64_t beta, int64_t score) {
        return score >= beta;
    };

    BoundUpdateFunc bound_update =
            [](int64_t* alpha, int64_t* beta, int64_t score) {
        *alpha = std::max(*alpha, score);
    };

    PieceColor heuristic_eval_color = color;

    /*
    TODO(theimer): reword
    If the current score is at least beta (i.e. the minimum at a parent
    minimizing node), then additional children can be ignored. The
    current node cannot improve the alpha because parent minimizers
    would never select this node as a minimum.
    */

    return alphaBetaSearchBase(board, color, depth_remaining, alpha, beta,
                               board_heuristic, heuristic_eval_color,
                               score_init, child_eval_variant, score_update,
                               exit_cond, bound_update);
}

/*
Minimizer variant of the search.

An "opponent" node of the tree.
*/
int64_t alphaBetaSearchMin(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           BoardHeuristicFunc board_heuristic) {


    int64_t score_init = std::numeric_limits<int64_t>::max();

    AlphaBetaVariantFunc child_eval_variant = alphaBetaSearchMax;

    ScoreUpdateFunc score_update = [](int64_t score, int64_t child_score){
        return std::min(score, child_score);
    };

    ExitCondFunc exit_cond =  [](int64_t alpha, int64_t beta, int64_t score) {
        return score <= alpha;
    };

    BoundUpdateFunc bound_update =
            [](int64_t* alpha, int64_t* beta, int64_t score) {
        *beta = std::min(*beta, score);
    };

    PieceColor heuristic_eval_color = board::oppositeColor(color);

    /*
    TODO(theimer)
    */

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
                           int64_t (*board_heuristic)(const Board&,
                                                      PieceColor)) {
    ASSERT(depth > 0,
            "must have positive depth; depth: " + std::to_string(depth));

    Board board_copy(board);
    util::Buffer<Move, game::MAX_NUM_MOVES_PLY> move_buffer;
    std::size_t num_moves =
            game::getAllMoves(board_copy, color, move_buffer.start());

    std::vector<Move> best_moves;
    int64_t alpha = std::numeric_limits<int64_t>::min();
    for (int i = 0; i < static_cast<int>(num_moves); ++i) {
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
            alpha = score;
            best_moves.clear();
            best_moves.push_back(move);
        } else if (score == alpha) {
            best_moves.push_back(move);
        }
    }

    return best_moves[rand() % best_moves.size()];
}

int64_t player::computer::basicBoardHeuristic(const Board& board,
                                              PieceColor color) {
    // just the negative count of the opponent pieces
    util::Buffer<Square, Board::SIZE> dummy_buffer;
    return -board.getOccupiedSquares(board::oppositeColor(color),
                                     dummy_buffer.start());
}
