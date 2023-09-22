#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Knight.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See Knight.h
    double Knight::getValue() 
    {
        return 3.0;
    }

    // See Knight.h
    std::vector<Move> Knight::generateMoves(GameState& gameState)
    {
        return {};
    }

    // See Knight.h
    std::vector<Move> Knight::generateAttackingMoves(GameState& gameState)
    {
        return {};
    }
}