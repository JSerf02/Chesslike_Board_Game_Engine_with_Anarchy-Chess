#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "King.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See King.h
    double King::getValue() 
    {
        return 100.0;
    }

    // See King.h
    bool King::isKing()
    {
        return true;
    }

    // See King.h
    std::vector<Move> King::generateMoves(GameState& gameState)
    {
        return {};
    }

    // See King.h
    std::vector<Move> King::generateAttackingMoves(GameState& gameState)
    {
        return {};
    }
}