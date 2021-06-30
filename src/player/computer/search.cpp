// Copyright 2021 Alex Theimer

#include "player/computer/search.h"

#include <limits>
#include <random>

#include "util/buffer.h"

using board::Board;
using board::PieceColor;
using board::Piece;
using board::Square;

using game::Move;

// forward-declare both of these because their definitions
//     reference each other
int64_t alphaBetaSearchMax(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           int64_t (*board_heuristic)(const Board&,
                                                      PieceColor));
int64_t alphaBetaSearchMin(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           int64_t (*board_heuristic)(const Board&,
                                                      PieceColor));

int64_t alphaBetaSearchMax(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           int64_t (*board_heuristic)(const Board&,
                                                      PieceColor)) {
    if (depth_remaining == 0) {
        return board_heuristic(*board, color);
    }

    util::Buffer<Move, game::MAX_NUM_MOVES_PLY> move_buffer;
    std::size_t num_moves =
            game::getAllMoves(*board, color, move_buffer.start());

    if (num_moves == 0) {
        return board_heuristic(*board, color);
    }

    int64_t score = std::numeric_limits<int64_t>::min();
    for (int i = 0; i < static_cast<int>(num_moves); ++i) {
        Move move = move_buffer.get(i);

        std::optional<Piece> overwritten_opt =
                game::makeMove(board, move);

        score = std::max(score,
                         alphaBetaSearchMin(board, board::oppositeColor(color),
                            depth_remaining - 1, alpha, beta, board_heuristic));

        game::unmakeMove(board, move, overwritten_opt);

        /*
        TODO(theimer): reword
        If the current score is at least beta (i.e. the minimum at a parent
        minimizing node), then additional children can be ignored. The
        current node cannot improve the alpha because parent minimizers
        would never select this node as a minimum.
        */
        if (score >= beta) {
            break;
        }

        alpha = std::max(alpha, score);
    }
    return score;
}

int64_t alphaBetaSearchMin(Board* board, PieceColor color,
                           std::size_t depth_remaining,
                           int64_t alpha, int64_t beta,
                           int64_t (*board_heuristic)(const Board&,
                                                      PieceColor)) {
    if (depth_remaining == 0) {
        return board_heuristic(*board, board::oppositeColor(color));
    }

    util::Buffer<Move, game::MAX_NUM_MOVES_PLY> move_buffer;
    std::size_t num_moves =
            game::getAllMoves(*board, color, move_buffer.start());

    if (num_moves == 0) {
        return board_heuristic(*board, board::oppositeColor(color));
    }

    int64_t score = std::numeric_limits<int64_t>::max();
    for (int i = 0; i < static_cast<int>(num_moves); ++i) {
        Move move = move_buffer.get(i);
        std::optional<Piece> overwritten_opt =
                game::makeMove(board, move);
        score = std::min(score,
                         alphaBetaSearchMax(board, board::oppositeColor(color),
                            depth_remaining - 1, alpha, beta, board_heuristic));
        game::unmakeMove(board, move, overwritten_opt);
        /*
        TODO(theimer): describe
        */
        if (score <= alpha) {
            break;
        }

        beta = std::min(beta, score);
    }
    return score;
}

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


    Move best_move = move_buffer.get(0);
    std::optional<Piece> overwritten_opt =
            game::makeMove(&board_copy, best_move);
    int64_t alpha = alphaBetaSearchMin(
                        &board_copy, board::oppositeColor(color),
                        depth - 1,
                        std::numeric_limits<int64_t>::min(),  // alpha
                        std::numeric_limits<int64_t>::max(),  // beta
                        board_heuristic);
    game::unmakeMove(&board_copy, best_move, overwritten_opt);

    // Note: starting index 1
    for (int i = 1; i < static_cast<int>(num_moves); ++i) {
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
            best_move = move_buffer.get(i);
        }
    }

    return best_move;
}

int64_t player::computer::basicBoardHeuristic(const Board& board,
                                              PieceColor color) {
    // TODO(theimer)
    util::Buffer<Square, Board::SIZE> dummy_buffer;
    return -board.getOccupiedSquares(board::oppositeColor(color),
                                     dummy_buffer.start());
}
