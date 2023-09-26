#include <vector>
#include <memory>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "King.h"
#include "MovePieceAction.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    using ID = Piece::ID;

    // See King.h
    double King::getValue() 
    {
        return 100.0;
    }

    // See King.h
    ID King::getID()
    {
        return KING_ID;
    }

    // See King.h
    std::vector<Move> King::generateMoves(GameState& gameState)
    {
        std::vector<Move> moves{};
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);
        addStandardMoves(moves, chessState);
        addKingsideCastle(moves, chessState);
        addQueensideCastle(moves, chessState);
        return moves;
    }

    // See King.h
    std::vector<Move> King::generateAttackingMoves(GameState& gameState)
    {
        std::vector<Move> moves{};
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);
        addStandardMoves(moves, chessState);
        return moves;
    }

    // See King.h
    void King::addStandardMoves(std::vector<Move>& moves, ChessGameState& chessState)
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
        addUnrelatedPositionsDeltas(deltas, moves, chessState);
    }

    // See King.h
    void King::addKingsideCastle(std::vector<Move>& moves, ChessGameState& chessState)
    {
        // A king cannot castly if it has already moved
        if(previouslyMoved()) {
            return;
        }

        // Store values for later use
        ChessBoard* board = static_cast<ChessBoard*>(chessState.getBoard());
        Move::position curPosition = getPosition();
        Player player = getPlayerAccess(Player::white) ? Player::white : Player::black;

        // Make sure the two spaces right of the king are empty and the space after is occupied
        if(!board->unoccupiedOnBoard(curPosition.first + 1, curPosition.second) 
        || !board->unoccupiedOnBoard(curPosition.first + 2, curPosition.second)
        || !board->occupiedOnBoard(curPosition.first + 3, curPosition.second)
        || chessState.isAttacked(player, std::make_pair(curPosition.first + 1, curPosition.second))) {
            return;
        }

        // Make sure the piece 3 spaces right of the king is a rook that hasn't moved
        Piece* rook = board->getPiece(curPosition.first + 3, curPosition.second);
        if(rook->previouslyMoved() || rook->getID() != ROOK_ID || !rook->getPlayerAccess(player)) {
            return;
        }

        // Add the move with a callback that moves the rook to its correct position
        addPosition(std::make_pair(curPosition.first + 2, curPosition.second), moves, chessState, 1, {
            make_action(new MovePieceAction(
                std::make_pair(1, 0),
                std::make_pair(-1, 0)
            ))
        });
    }

    // See King.h
    void King::addQueensideCastle(std::vector<Move>& moves, ChessGameState& chessState)
    {
        // A king cannot castly if it has already moved
        if(previouslyMoved()) {
            return;
        }

        // Store values for later use
        ChessBoard* board = static_cast<ChessBoard*>(chessState.getBoard());
        Move::position curPosition = getPosition();
        Player player = getPlayerAccess(Player::white) ? Player::white : Player::black;

        // Make sure the three spaces left of the king are empty and the space after is occupied
        if(!board->unoccupiedOnBoard(curPosition.first - 1, curPosition.second) 
        || !board->unoccupiedOnBoard(curPosition.first - 2, curPosition.second)
        || !board->unoccupiedOnBoard(curPosition.first - 3, curPosition.second)
        || !board->occupiedOnBoard(curPosition.first - 4, curPosition.second)
        || chessState.isAttacked(player, std::make_pair(curPosition.first - 1, curPosition.second))) {
            return;
        }

        // Make sure the piece 4 spaces left of the king is a rook that hasn't moved
        Piece* rook = board->getPiece(curPosition.first - 4, curPosition.second);
        if(rook->previouslyMoved() || rook->getID() != ROOK_ID || !rook->getPlayerAccess(player)) {
            return;
        }

        // Add the move with a callback that moves the rook to its correct position
        addPosition(std::make_pair(curPosition.first - 2, curPosition.second), moves, chessState, 1, {
            make_action(new MovePieceAction(
                std::make_pair(-2, 0),
                std::make_pair(1, 0)
            ))
        });
    }
}