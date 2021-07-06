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


game::InvalidMoveEx::InvalidMoveEx(Move move) : move_(move) {
    // intentionally blank
}

const char* game::InvalidMoveEx::what() const throw() {
    std::stringstream ss;
    ss << "invalid move: " << move_;
    return ss.str().c_str();
}

game::Move game::InvalidMoveEx::getMove() const {
    return move_;
}

game::Player::Player(std::string name) : name_(name) {
    // intentionally blank
}

std::string game::Player::getName() {
    return name_;
}

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
    PieceColor current_color = next_player_color_;
    Player* player;
    switch (current_color) {
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
                "unhandled PieceColor: " + std::to_string(current_color));
    }
    // get the move the player wants to make
    Move move = player->getMove(*board_, current_color);
    // make sure the move is valid
    if (game::isValidMove(*board_, current_color, move)) {
        game::makeMove(board_, move);
    } else {
        throw InvalidMoveEx(move);
    }
}

bool Game::isEnded() const {
    // game is over when fewer than two kings exist
    util::Buffer<Square, 2> buffer;
    std::size_t size = board_->getOccupiedSquares(
            PieceType::KING, buffer.start());
    return size < static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS);
}

Player& Game::getWinner() const {
    ASSERT(isEnded(), "game not yet ended");
    // get the color of the only remaining king; return that player.
    util::Buffer<Square, 2> buffer;

    // prevents "unused variable" compiler warning
 #ifdef DEBUG
    std::size_t size =
            board_->getOccupiedSquares(PieceType::KING, buffer.start());
    ASSERT(size == 1, "size: " + std::to_string(size));
 #else
    board_->getOccupiedSquares(PieceType::KING, buffer.start());
 #endif

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
