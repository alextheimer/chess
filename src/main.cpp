#include <unordered_map>
#include <iostream>

#include "board/board.h"
#include "game/game.h"
#include "player/human.h"
#include "util/assert.h"

int main(int argc, char *argv[]) {
    board::Board board(std::unordered_map<board::Square, board::Piece>{
        {board::Square(1, 1), (board::Piece){board::PieceType::KING, board::PieceColor::BLACK}},
        {board::Square(5, 5), (board::Piece){board::PieceType::KING, board::PieceColor::WHITE}}
    });

    player::Human player1, player2;

    game::Game game(board, player1, player2);

    game.render(std::cout);

    game.runPly();

    ASSERT(false, "poop");

    game.render(std::cout);
}
