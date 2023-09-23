#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Bishop.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See Bishop.h
    double Bishop::getValue() 
    {
        return 3.0;
    }

    // See Bishop.h
    std::vector<Move> Bishop::generateMoves(GameState& gameState)
    {
        // Store for later use
        ChessBoard* chessBoard = static_cast<ChessBoard*>(gameState.getBoard());
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);

        // Generate the deltas for each direction
        std::vector<Move::position> upRightDeltas{};
        std::vector<Move::position> downRightDeltas{};
        std::vector<Move::position> upLeftDeltas{};
        std::vector<Move::position> downLeftDeltas{};
        for(int i = 1; i <= std::max(chessBoard->maxX(), chessBoard->maxY()) - std::min(chessBoard->minX(), chessBoard->minY()); i++) {
            upRightDeltas.push_back(  std::make_pair(i, i));
            downRightDeltas.push_back(std::make_pair(i, -i));
            upLeftDeltas.push_back(   std::make_pair(-i, i));
            downLeftDeltas.push_back( std::make_pair(-i, -i));
        }

        // Add related moves for each set of deltas
        std::vector<Move> results = addRelatedPositionsDeltas(upRightDeltas, chessState);
        addRelatedPositionsDeltas(downRightDeltas, results, chessState);
        addRelatedPositionsDeltas(upLeftDeltas,    results, chessState);
        addRelatedPositionsDeltas(downLeftDeltas,  results, chessState);

        return results;
    }

    // See Bishop.h
    std::vector<Move> Bishop::generateAttackingMoves(GameState& gameState)
    {
        return generateMoves(gameState);
    }
}