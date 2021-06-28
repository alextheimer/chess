#include "player/human.h"

#include <iostream>

using board::Board;
using board::Square;

using game::Move;

Move player::Human::getMove(const Board& board) {
    std::size_t row1, col1, row2, col2;
    std::cout << "Enter row1 col1 row2 col2: ";
    std::cin >> row1 >> col1 >> row2 >> col2;
    return (Move){ Square(row1, col1), Square(row2, col2) };
}
