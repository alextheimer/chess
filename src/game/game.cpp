// Copyright 2021 Alex Theimer

#include "game/game.h"

#include <string>
#include <unordered_map>
#include <sstream>

#include "util/buffer.h"
#include "game/move.h"
#include "util/assert.h"

using board::Square;
using board::Piece;
using board::PieceColor;
using board::PieceType;
using board::Board;

using game::Game;
using game::Player;

constexpr PieceColor START_COLOR = PieceColor::BLACK;

const std::unordered_map<Square, Piece> game::INIT_PIECE_MAP = {
        { Square(0, 0), (Piece){ PieceType::ROOK, PieceColor::BLACK } },
        { Square(0, 1), (Piece){ PieceType::KNIGHT, PieceColor::BLACK } },
        { Square(0, 2), (Piece){ PieceType::BISHOP, PieceColor::BLACK } },
        { Square(0, 3), (Piece){ PieceType::KING, PieceColor::BLACK } },
        { Square(0, 4), (Piece){ PieceType::QUEEN, PieceColor::BLACK } },
        { Square(0, 5), (Piece){ PieceType::BISHOP, PieceColor::BLACK } },
        { Square(0, 6), (Piece){ PieceType::KNIGHT, PieceColor::BLACK } },
        { Square(0, 7), (Piece){ PieceType::ROOK, PieceColor::BLACK } },


        { Square(1, 0), (Piece){ PieceType::PAWN, PieceColor::BLACK } },
        { Square(1, 1), (Piece){ PieceType::PAWN, PieceColor::BLACK } },
        { Square(1, 2), (Piece){ PieceType::PAWN, PieceColor::BLACK } },
        { Square(1, 3), (Piece){ PieceType::PAWN, PieceColor::BLACK } },
        { Square(1, 4), (Piece){ PieceType::PAWN, PieceColor::BLACK } },
        { Square(1, 5), (Piece){ PieceType::PAWN, PieceColor::BLACK } },
        { Square(1, 6), (Piece){ PieceType::PAWN, PieceColor::BLACK } },
        { Square(1, 7), (Piece){ PieceType::PAWN, PieceColor::BLACK } },

        { Square(7, 0), (Piece){ PieceType::ROOK, PieceColor::WHITE } },
        { Square(7, 1), (Piece){ PieceType::KNIGHT, PieceColor::WHITE } },
        { Square(7, 2), (Piece){ PieceType::BISHOP, PieceColor::WHITE } },
        { Square(7, 3), (Piece){ PieceType::KING, PieceColor::WHITE } },
        { Square(7, 4), (Piece){ PieceType::QUEEN, PieceColor::WHITE } },
        { Square(7, 5), (Piece){ PieceType::BISHOP, PieceColor::WHITE } },
        { Square(7, 6), (Piece){ PieceType::KNIGHT, PieceColor::WHITE } },
        { Square(7, 7), (Piece){ PieceType::ROOK, PieceColor::WHITE } },


        { Square(6, 0), (Piece){ PieceType::PAWN, PieceColor::WHITE } },
        { Square(6, 1), (Piece){ PieceType::PAWN, PieceColor::WHITE } },
        { Square(6, 2), (Piece){ PieceType::PAWN, PieceColor::WHITE } },
        { Square(6, 3), (Piece){ PieceType::PAWN, PieceColor::WHITE } },
        { Square(6, 4), (Piece){ PieceType::PAWN, PieceColor::WHITE } },
        { Square(6, 5), (Piece){ PieceType::PAWN, PieceColor::WHITE } },
        { Square(6, 6), (Piece){ PieceType::PAWN, PieceColor::WHITE } },
        { Square(6, 7), (Piece){ PieceType::PAWN, PieceColor::WHITE } },
};

Game::Game(Board* board, Player* white_player, Player* black_player) :
        board_(board), white_player_(white_player),
        black_player_(black_player), next_player_color_(START_COLOR) {
    // intentionally blank
}

void Game::renderBoard(std::ostream& ostream) const {
    ostream << board_->toString();
}

void Game::runPly() {
    ASSERT(!isEnded(), "game already ended");
    Player* player;
    switch (next_player_color_) {
    case PieceColor::BLACK:
        player = black_player_;
        next_player_color_ = PieceColor::WHITE;
        break;
    case PieceColor::WHITE:
        player = white_player_;
        next_player_color_ = PieceColor::BLACK;
        break;
    default:
        throw std::invalid_argument(
                "unhandled PieceColor: " + std::to_string(next_player_color_));
    }
    // get the move the player wants to make
    Move move = player->getMove(*board_);
    Piece moved_piece = board_->getPiece(move.from);
    // make sure the move is valid
    util::Buffer<Move, Board::SIZE> valid_moves;
    std::size_t num_moves = game::getPieceMoves(
            *board_, moved_piece.color, move.from, valid_moves.start());
    for (int i = 0; i < static_cast<int>(num_moves); ++i) {
        if (valid_moves.get(i) == move) {
            game::makeMove(board_, move);
            return;
        }
    }
    throw std::invalid_argument("illegal Move: " + std::to_string(move));
}

bool Game::isEnded() const {
    // game is over when fewer than two kings exist
    util::Buffer<Square, 2> buffer;
    std::size_t size = board_->getOccupiedSquares(
            PieceType::KING, buffer.start());
    return size < static_cast<int>(PieceColor::NUM_PIECE_COLORS);
}

Player& Game::getWinner() const {
    ASSERT(isEnded(), "game not yet ended");
    // get the color of the only remaining king; return that player.
    util::Buffer<Square, 2> buffer;
    std::size_t size =
            board_->getOccupiedSquares(PieceType::KING, buffer.start());
    ASSERT(size == 1, "size: " + std::to_string(1));
    PieceColor color = board_->getPiece(buffer.get(0)).color;
    switch (color) {
    case PieceColor::BLACK:
        return *black_player_;
    case PieceColor::WHITE:
        return *white_player_;
    default:
        throw std::invalid_argument(
                "unhandled PieceColor: " + std::to_string(color));
    }
}
