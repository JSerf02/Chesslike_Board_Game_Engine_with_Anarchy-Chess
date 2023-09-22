#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Bishop.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See Bishop.h
    double Bishop::getValue() 
    {
        return 3.0;
    }

    // See Bishop.h
    std::vector<Move> Bishop::generateMoves(GameState& gameState)
    {
        return {};
    }

    // See Bishop.h
    std::vector<Move> Bishop::generateAttackingMoves(GameState& gameState)
    {
        return {};
    }
}