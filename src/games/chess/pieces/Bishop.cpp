#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Bishop.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    using ID = Piece::ID;

    // See Bishop.h
    double Bishop::getValue() 
    {
        return 3.0;
    }

    // See Bishop.h
    ID Bishop::getID()
    {
        return BISHOP_ID;
    }

    // See Bishop.h
    std::vector<Move> Bishop::generateMoves(GameState& gameState)
    {
        std::vector<Move> moves{};
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);
        addStandardMoves(moves, chessState);
        addIlVaticano(moves, chessState);
        return moves;
    }

    // See Bishop.h
    std::vector<Move> Bishop::generateAttackingMoves(GameState& gameState)
    {
        std::vector<Move> moves{};
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);
        addStandardMoves(moves, chessState);
        addIlVaticano(moves, chessState, true);
        return moves;
    }

    // See Bishop.h
    void Bishop::addStandardMoves(std::vector<Move>& moves, ChessGameState& chessState)
    {
        // Store for later use
        ChessBoard* chessBoard = static_cast<ChessBoard*>(chessState.getBoard());

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
        addRelatedPositionsDeltas(upRightDeltas,   moves, chessState);
        addRelatedPositionsDeltas(downRightDeltas, moves, chessState);
        addRelatedPositionsDeltas(upLeftDeltas,    moves, chessState);
        addRelatedPositionsDeltas(downLeftDeltas,  moves, chessState);
    }
    // See Bishop.h
    void Bishop::addIlVaticano(std::vector<Move>& moves, ChessGameState& chessState, bool attackOnly)
    {
        // Store info for later
        GameBoard* board = chessState.getBoard();
        Move::position curPosition = getPosition();

        // Check for Il Vaticano in 4 direction (right, left, up, down)
        std::vector<Move::position> checkDirections {
            std::make_pair(1, 0),
            std::make_pair(-1, 0),
            std::make_pair(0, 1),
            std::make_pair(0, -1)
        };
        for(Move::position direction : checkDirections) {
            // Make sure the other piece 3 spaces away in the direction is a bishop
            Move::position otherBishop = std::make_pair(curPosition.first + 3 * direction.first, curPosition.second + 3 * direction.second);
            if(!board->occupiedOnBoard(otherBishop) || board->getPiece(otherBishop)->getID() != BISHOP_ID) {
                continue;
            }
            
            // Make sure the bishops surround 2 pawns
            bool canProcede = true;
            for(int i = 1; i <= 2; i++) {
                Move::position pawn = std::make_pair(curPosition.first + i * direction.first, curPosition.second + i * direction.second);
                if(!board->occupiedOnBoard(pawn) || board->getPiece(pawn)->getID() != PAWN_ID) {
                    canProcede = false;
                    break;
                }
            }
            if(!canProcede) {
                break;
            }

            // Add the pawn positions if this is for attacks
            if(attackOnly) {
                addUnrelatedPositions({
                    std::make_pair(curPosition.first +     direction.first, curPosition.second +     direction.second),
                    std::make_pair(curPosition.first + 2 * direction.first, curPosition.second + 2 * direction.second)
                }, moves, chessState, 3);
                continue;
            }

            // Add the swap move with a callback that swaps the bishops and captures the pawns
            Move newMove = Move({}, 3, 
            [](Move::position start, Move::position end, GameState& gameState, bool simulation) {
                GameBoard* board = gameState.getBoard();
                Move::position direction = std::make_pair(
                    static_cast<int>((end.first - start.first) / 3), 
                    static_cast<int>((end.second - start.second) / 3)
                );
                Move::position pawn1 = std::make_pair(
                    start.first + direction.first, 
                    start.second + direction.second
                );
                Move::position pawn2 = std::make_pair(
                    start.first + 2 * direction.first, 
                    start.second + 2 * direction.second
                );
                if(simulation) {
                    board->simulateRemovePiece(pawn1);
                    board->simulateRemovePiece(pawn2);

                    // Temporarily put the second bishop at pawn1 so the first bishop can move to end
                    board->simulateMovePiece(end, pawn1); 
                }
                else {
                    board->capturePiece(pawn1);
                    board->capturePiece(pawn2);
                    
                    // Temporarily put the second bishop at pawn1 so the first bishop can move to end
                    board->movePiece(end, pawn1);
                }
                board->queueFutureMove(pawn1, start);
            });
            if(addToMove(otherBishop, newMove, chessState)) {
                moves.push_back(newMove); // Add the new move if successful
            }
        }
    }
}