#include "player/human.h"

#include <iostream>

Move player::Human::getMove(const Board& board) {
    size_t row1, col1, row2, col2;
    std::cout << "Enter row1 col1 row2 col2: ";
    std::cin >> row1 >> col1 >> row2 >> col2;
    return (game::Move){ Square(row1, col1), Square(row2, col2) };
}
