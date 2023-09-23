#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Rook.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    using ID = Piece::ID;

    // See Rook.h
    double Rook::getValue() 
    {
        return 5.0;
    }

    // See Rook.h
    ID Rook::getID()
    {
        return ROOK_ID;
    }

    // See Rook.h
    std::vector<Move> Rook::generateMoves(GameState& gameState)
    {
        // Store for later use
        ChessBoard* chessBoard = static_cast<ChessBoard*>(gameState.getBoard());
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);

        // Generate the deltas for each direction
        std::vector<Move::position> rightDeltas{};
        std::vector<Move::position> leftDeltas{};
        std::vector<Move::position> upDeltas{};
        std::vector<Move::position> downDeltas{};
        for(int x = 1; x <= chessBoard->maxX() - chessBoard->minX(); x++) {
            rightDeltas.push_back(std::make_pair(x, 0));
            leftDeltas.push_back(std::make_pair(-x, 0));
        }
        for(int y = 1; y <= chessBoard->maxY() - chessBoard->minY(); y++) {
            upDeltas.push_back(std::make_pair(0, y));
            downDeltas.push_back(std::make_pair(0, -y));
        }

        // Add related moves for each set of deltas
        std::vector<Move> results = addRelatedPositionsDeltas(rightDeltas, chessState);
        addRelatedPositionsDeltas(leftDeltas, results, chessState);
        addRelatedPositionsDeltas(upDeltas,   results, chessState);
        addRelatedPositionsDeltas(downDeltas, results, chessState);

        return results;
    }

    // See Rook.h
    std::vector<Move> Rook::generateAttackingMoves(GameState& gameState)
    {
        return generateMoves(gameState);
    }
}