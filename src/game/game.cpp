#include "game/game.h"

#include <cassert>
#include <string>
#include <unordered_map>
#include <sstream>

#include "util/buffer.h"
#include "game/move.h"

using namespace game;
using namespace board;

// TODO(theimer): move?
static const PieceColor START_COLOR = PieceColor::BLACK;

static const std::unordered_map<Piece, char> PIECE_CHAR_MAP = {
        { (Piece){ PieceType::BISHOP, PieceColor::BLACK }, 'B' },
        { (Piece){ PieceType::BISHOP, PieceColor::WHITE }, 'b' },
        { (Piece){ PieceType::KNIGHT, PieceColor::BLACK }, 'N' },
        { (Piece){ PieceType::KNIGHT, PieceColor::WHITE }, 'n' },
        { (Piece){ PieceType::ROOK,  PieceColor::BLACK }, 'R' },
        { (Piece){ PieceType::ROOK, PieceColor::WHITE }, 'r' },
        { (Piece){ PieceType::KING, PieceColor::BLACK }, 'K' },
        { (Piece){ PieceType::KING, PieceColor::WHITE }, 'k' },
        { (Piece){ PieceType::QUEEN, PieceColor::BLACK }, 'Q' },
        { (Piece){ PieceType::QUEEN, PieceColor::WHITE }, 'q' },
        { (Piece){ PieceType::PAWN, PieceColor::BLACK }, 'P' },
        { (Piece){ PieceType::PAWN, PieceColor::WHITE }, 'p' }
};

Game::Game(Board& board, Player& white_player, Player& black_player) :
        board_(board), white_player_(white_player),
        black_player_(black_player), next_player_(START_COLOR) {
    // intentionally blank
}

void Game::render(std::ostream& ostream) {
    std::string col_header = "  0 1 2 3 4 5 6 7\n";
    ostream << col_header;
    for (int irow = 0; irow < Board::WIDTH; ++irow) {
        ostream << irow << ' ';
        for (int icol = 0; icol < Board::WIDTH; ++icol) {
            Square square(irow, icol);
            if (board_.squareIsOccupied(square)) {
                Piece piece = board_.getPiece(square);
                ostream << PIECE_CHAR_MAP.at(piece) << ' ';
            }
            ostream << '\n';
        }
    }
    ostream.flush();
}

void Game::runPly() {
    assert(!isEnded());
    Player* player;
    switch (next_player_) {
    case PieceColor::BLACK:
        player = &black_player_;
        next_player_ = PieceColor::WHITE;
        break;
    case PieceColor::WHITE:
        player = &white_player_;
        next_player_ = PieceColor::BLACK;
        break;
    default:
        throw std::invalid_argument("unhandled PieceColor: " + toString(next_player_));
    }
    Move move = player->getMove(board_);
    Piece moved_piece = board_.getPiece(move.from);

    util::Buffer<Move, Board::SIZE> valid_moves;
    size_t num_moves = game::getPieceMoves(board_, moved_piece.color, move.from, valid_moves.start());
    for (int i = 0; i < num_moves; ++i) {
        if (valid_moves.get(i) == move) {
            game::makeMove(board_, move);
            return;
        }
    }

    std::stringstream error_msg;
    error_msg << "illegal Move: " << move;
    throw std::invalid_argument(error_msg.str());
}

bool Game::isEnded() {
    util::Buffer<Square, 2> buffer;
    std::size_t size = board_.getOccupiedSquares(PieceType::KING, buffer.start());
    return size < static_cast<int>(PieceColor::NUM_PIECE_COLORS);
}

Player& Game::getWinner() {
    assert(isEnded());
    util::Buffer<Square, 2> buffer;
    std::size_t size = board_.getOccupiedSquares(PieceType::KING, buffer.start());
    assert(size == 1);
    PieceColor color = board_.getPiece(buffer.get(0)).color;
    switch (color) {
    case PieceColor::BLACK:
        return black_player_;
    case PieceColor::WHITE:
        return white_player_;
    default:
        throw std::invalid_argument("unhandled PieceColor: " + toString(color));
    }
}
