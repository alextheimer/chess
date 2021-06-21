#include "board/board.h"

using namespace board;

namespace game {

class Player {
 public:
    virtual void makeMove(const Board& board) = 0;
};

}
