#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Knook.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    using ID = Piece::ID;

    // See Knook.h
    double Knook::getValue() 
    {
        return 9.0;
    }

    // See Knook.h
    ID Knook::getID()
    {
        return KNOOK_ID;
    }

    // See Knook.h
    std::vector<Move> Knook::generateMoves(GameState& gameState)
    {
        // Store for later use
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);
        ChessBoard* chessBoard = static_cast<ChessBoard*>(chessState.getBoard());
        
        // Generate knight moves
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
        std::vector<Move> moves = addUnrelatedPositionsDeltas(deltas, chessState);

        // Generate bishop moves
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
        addRelatedPositionsDeltas(upRightDeltas,   moves, chessState);
        addRelatedPositionsDeltas(downRightDeltas, moves, chessState);
        addRelatedPositionsDeltas(upLeftDeltas,    moves, chessState);
        addRelatedPositionsDeltas(downLeftDeltas,  moves, chessState);

        return moves;
    }

    // See Knook.h
    std::vector<Move> Knook::generateAttackingMoves(GameState& gameState)
    {
        return generateMoves(gameState);
    }
}