#include <unordered_map>

#include "ChessBoard.h"
#include "GameState.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See ChessBoard.h
    void ChessBoard::setup() {
        return;
    }

    // See ChessBoard.h
    bool ChessBoard::onBoard(int x, int y) {
        return true;
    }

}