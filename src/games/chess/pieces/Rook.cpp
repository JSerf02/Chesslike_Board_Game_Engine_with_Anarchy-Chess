#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Rook.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See Rook.h
    double Rook::getValue() 
    {
        return 5.0;
    }

    // See Rook.h
    std::vector<Move> Rook::generateMoves(GameState& gameState)
    {
        return {};
    }

    // See Rook.h
    std::vector<Move> Rook::generateAttackingMoves(GameState& gameState)
    {
        return {};
    }
}