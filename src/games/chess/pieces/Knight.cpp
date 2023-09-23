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
        // All of the different L positions
        const std::vector<Move::position> deltas {
            std::make_pair(1, 2),
            std::make_pair(2, 1),
            std::make_pair(2, -1),
            std::make_pair(1, -2),
            std::make_pair(-1, -2),
            std::make_pair(-2, -1),
            std::make_pair(-2, 1),
            std::make_pair(-1, 2)
        };
        return addUnrelatedPositionsDeltas(deltas, static_cast<ChessGameState&>(gameState));
    }

    // See Knight.h
    std::vector<Move> Knight::generateAttackingMoves(GameState& gameState)
    {
        return generateMoves(gameState);
    }
}