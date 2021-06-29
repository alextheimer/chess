// Copyright 2021 Alex Theimer

#include "player/human.h"

#include <iostream>

using board::Board;
using board::Square;

using game::Move;

Move player::Human::getMove(const Board& board) {
    std::size_t row_from, col_from, row_to, col_to;
    std::cout << "Enter row_from col_from row_to col_to: ";
    std::cin >> row_from >> col_from >> row_to >> col_to;
    return (Move){ Square(row_from, col_from), Square(row_to, col_to) };
}
