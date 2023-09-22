#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Queen.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See Queen.h
    double Queen::getValue() 
    {
        return 9.0;
    }

    // See Queen.h
    std::vector<Move> Queen::generateMoves(GameState& gameState)
    {
        return {};
    }

    // See Queen.h
    std::vector<Move> Queen::generateAttackingMoves(GameState& gameState)
    {
        return {};
    }
}