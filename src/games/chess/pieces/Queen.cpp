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
        // Store for later use
        ChessBoard* chessBoard = static_cast<ChessBoard*>(gameState.getBoard());
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);

        // Vectors storing deltas in each direction
        std::vector<Move::position> rightDeltas{};
        std::vector<Move::position> leftDeltas{};
        std::vector<Move::position> upDeltas{};
        std::vector<Move::position> downDeltas{};
        std::vector<Move::position> upRightDeltas{};
        std::vector<Move::position> downRightDeltas{};
        std::vector<Move::position> upLeftDeltas{};
        std::vector<Move::position> downLeftDeltas{};

        // Generate horizontal deltas 
        for(int x = 1; x <= chessBoard->maxX() - chessBoard->minX(); x++) {
            rightDeltas.push_back(std::make_pair(x, 0));
            leftDeltas.push_back(std::make_pair(-x, 0));
        }

        // Generate vertical deltas
        for(int y = 1; y <= chessBoard->maxY() - chessBoard->minY(); y++) {
            upDeltas.push_back(std::make_pair(0, y));
            downDeltas.push_back(std::make_pair(0, -y));
        }
        
        // Generate diagonal deltas
        for(int i = 1; i <= std::max(chessBoard->maxX(), chessBoard->maxY()) - std::min(chessBoard->minX(), chessBoard->minY()); i++) {
            upRightDeltas.push_back(  std::make_pair(i, i));
            downRightDeltas.push_back(std::make_pair(i, -i));
            upLeftDeltas.push_back(   std::make_pair(-i, i));
            downLeftDeltas.push_back( std::make_pair(-i, -i));
        }

        // Add related moves for each set of deltas
        std::vector<Move> results = addRelatedPositionsDeltas(rightDeltas, chessState);
        addRelatedPositionsDeltas(leftDeltas,      results, chessState);
        addRelatedPositionsDeltas(upDeltas,        results, chessState);
        addRelatedPositionsDeltas(downDeltas,      results, chessState);
        addRelatedPositionsDeltas(upRightDeltas,   results, chessState);
        addRelatedPositionsDeltas(downRightDeltas, results, chessState);
        addRelatedPositionsDeltas(upLeftDeltas,    results, chessState);
        addRelatedPositionsDeltas(downLeftDeltas,  results, chessState);

        return results;
    }

    // See Queen.h
    std::vector<Move> Queen::generateAttackingMoves(GameState& gameState)
    {
        return generateMoves(gameState);
    }
}