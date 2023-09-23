#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Pawn.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    using ID = Piece::ID;

    // See Pawn.h
    double Pawn::getValue() 
    {
        return 1.0;
    }

    // See Pawn.h
    ID Pawn::getID()
    {
        return PAWN_ID;
    }

    // See Pawn.h
    bool Pawn::canBoost()
    {
        return !alreadyMoved;
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