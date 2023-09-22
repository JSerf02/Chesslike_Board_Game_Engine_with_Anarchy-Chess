#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Pawn.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See Pawn.h
    double Pawn::getValue() 
    {
        return 1.0;
    }

    // See Pawn.h
    bool Pawn::canBoost()
    {
        return false;
    }

    // See Pawn.h
    std::vector<Move> Pawn::generateMoves(GameState& gameState)
    {
        return {};
    }

    // See Pawn.h
    std::vector<Move> Pawn::generateAttackingMoves(GameState& gameState)
    {
        return {};
    }

    // See Pawn.h
    bool Pawn::justBoosted()
    {
        return false;
    }
}