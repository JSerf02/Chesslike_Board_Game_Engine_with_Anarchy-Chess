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
        // All of the different distances away from the starting position that the king can move to
        const std::vector<Move::position> deltas {
            std::make_pair(1, 0),
            std::make_pair(1, 1),
            std::make_pair(0, 1),
            std::make_pair(-1, 1),
            std::make_pair(-1, 0),
            std::make_pair(-1, -1),
            std::make_pair(0, -1),
            std::make_pair(1, -1)
        };
        return addUnrelatedPositionsDeltas(deltas, static_cast<ChessGameState&>(gameState));
    }

    // See King.h
    std::vector<Move> King::generateAttackingMoves(GameState& gameState)
    {
        return generateMoves(gameState);
    }
}