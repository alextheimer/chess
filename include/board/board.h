// Copyright 2021 Alex Theimer

#ifndef BOARD_BOARD_H_
#define BOARD_BOARD_H_

#include <cstdint>
#include <ostream>
#include <unordered_map>
#include <string>

#include "board/square.h"
#include "board/piece.h"
#include "board/zobhash.h"

namespace board {

// Stores a single bit of data for each of the 64 spaces on a Board.
typedef uint64_t Bitboard;

/*
Maintains an 8x8 chess board.
*/
class Board {
 public:
    // TODO(themer): do away with these in favor of just the Square constants?
    // size of either dimension of the Board (i.e. WIDTH rows and WIDTH columns)
    static const std::size_t WIDTH = Square::MAX_DIM_VALUE;
    // total number of spaces on the Board.
    static const std::size_t SIZE = Square::NUM_SQUARES;

    /*
    Constructs an empty board.
    */
    Board();

    /*
    Copy constructor.
    */
    explicit Board(const Board& other);

    /*
    Constructs a Board instance from a Square->Piece mapping.
    I.e. for each pair (square, piece), `piece` is stored at `square`.
    */
    explicit Board(const std::unordered_map<Square, Piece>& piece_map);

    std::string toString() const;
    std::ostream& operator<<(std::ostream& ostream) const;

    /*
     Returns true iff `square` is occupied on the Board.
    */
    bool squareIsOccupied(Square square) const;

    /*
    Returns true iff `square` is occupied on the Board by a piece with the specified PieceColor.
    */
    bool squareIsOccupiedColor(Square square, PieceColor color) const;

    /*
    Sets the piece described by `piece` at `square` on the Board.
    @param square: must be unoccupied.
    */
    void setPiece(Piece piece, Square square);

    /*
    Sets the piece described by `piece` at `square` on the Board.
    Note: may give better performance than Board::removePiece and Board::setPiece.
    */
    void setPieceOverwrite(Piece piece, Square square);

    /*
    Moves a piece from one square to another.
    @param from: must be occupied
    @param to: must be unoccupied
    */
    void movePiece(Square from, Square to);

    /*
    Moves a piece from one square to another.
    Note: may give better performance than Board::removePiece and Board::movePiece.
    @param from: must be occupied
    */
    void movePieceOverwrite(Square from, Square to);

    /*
    @note: might allow the implementation to return a result faster than Board::getPiece.
    @param square: must be occupied
    */
    PieceType getPieceType(Square square) const;

    /*
    @note: might allow the implementation to return a result faster than Board::getPiece.
    @param square: must be occupied
    */
    PieceColor getPieceColor(Square square) const;

    /*
    @note: use Board::getPieceType/getPieceColor if only partial Piece info is needed.
    @param square: must be occupied
    */
    Piece getPiece(Square square) const;

    // TODO(theimer): worth making an overwrite variant?
    /*
    @param square: must be occupied
    */
    void removePiece(Square square);

    /*
    Fills a buffer with all Squares that contain a piece with the specified PieceColor.
    @param buffer: an Iterator at the beginning of the buffer.
    @return: the number of Squares added to the buffer.
    */
    std::size_t getOccupiedSquares(PieceColor color, Square* buffer) const;

    /*
    Fills a buffer with all Squares that contain a piece with the specified PieceType.
    @param buffer: an Iterator at the beginning of the buffer.
    @return: the number of Squares added to the buffer.
    */
    std::size_t getOccupiedSquares(PieceType type, Square* buffer) const;

    /*
    Fills a buffer with all Squares that contain a piece.
    @param buffer: an Iterator at the beginning of the buffer.
    @return: the number of Squares added to the buffer.
    */
    std::size_t getOccupiedSquares(Square* buffer) const;

    /*
    Returns the Zobrist Hash of the board.
    */
    ZobHash getZobHash() const;

 private:
    /*
    Each Board is maintained by a series of "Bitboards", where each Bitboard
    stores one bit of data for each of the 64 Board spaces.
    
    One Bitboard is stored for each PieceColor and PieceType.
    */
    Bitboard piece_bitboards_[
                 static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES)
             ] = { 0 };
    Bitboard color_bitboards_[
                 static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS)
             ] = { 0 };

    // Zobrist Hash value.
    ZobHash hash_;

    // TODO(theimer): this is super ugly; PIMPL if not too much overhead
    bool squareIsOccupiedIndex(std::size_t index) const;
    bool squareIsOccupiedColorIndex(std::size_t index, PieceColor color) const;
    PieceColor getPieceColorIndex(std::size_t index) const;
    PieceType getPieceTypeIndex(std::size_t index) const;
    Piece getPieceIndex(std::size_t index) const;
    void setPieceIndex(Piece piece, std::size_t index);
    void movePieceIndex(std::size_t from_index, std::size_t to_index);
    void removePieceIndex(std::size_t index);
    void setPieceOverwriteIndex(Piece piece, std::size_t index);
    void movePieceOverwriteIndex(std::size_t from_index, std::size_t to_index);
};

}  // namespace board

namespace std {

// TODO(theimer): make friends with Board and remove getZobHash()
template <> struct hash<board::Board> {
    std::size_t operator()(const board::Board& board) const;
};

}  // namespace std

#endif  // BOARD_BOARD_H_
