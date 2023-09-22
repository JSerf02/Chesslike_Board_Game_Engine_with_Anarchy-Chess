#include "ChessPiece.h"
#include "ChessGameState.h"
#include "Piece.h"
#include "Move.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    
    // See ChessPiece.h
    bool ChessPiece::controlledByPlayer(GameState& gameState)
    {
        return false;
    }

    // See ChessPiece.h
    bool ChessPiece::addToMove(Move::position position, Move& move, ChessGameState& chessState)
    {
        // Get a reference to the game board
        GameBoard* board = chessState.getBoard();

        // Simulate moving the piece and see if it will put the player in check
        if (board->simulateMovePiece(getPosition(), position)) {
            return false; // The move is impossible so it cannot be added
        }
        bool isSafe = !chessState.isInCheck();
        board->revertSimulation();

        // Add the move only if it does not put the player in check
        if(isSafe) {
            move.addPosition(position);
        }
        return isSafe;
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addPosition(Move::position position, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        std::vector<Move> moves{};
        addPosition(position, moves, chessState, onMoveCallback);
        return moves;
    }
    bool ChessPiece::addPosition(Move::position position, std::vector<Move>& moves, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        return false;
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addUnrelatedPositions(std::vector<Move::position> positions, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        std::vector<Move> moves{};
        addUnrelatedPositions(positions, moves, chessState, onMoveCallback);
        return moves;
    }
    void ChessPiece::addUnrelatedPositions(std::vector<Move::position> positions, std::vector<Move>& moves, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        return;
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addRelatedPositions(std::vector<Move::position> positions, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        std::vector<Move> moves{};
        addRelatedPositions(positions, moves, chessState, onMoveCallback);
        return moves;
    }
    void ChessPiece::addRelatedPositions(std::vector<Move::position> positions, std::vector<Move>& moves, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        return;
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addUnrelatedPositionsDeltas(std::vector<Move::position> deltas, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        std::vector<Move> moves{};
        addUnrelatedPositionsDeltas(deltas, moves, chessState, onMoveCallback);
        return moves;
    }
    void ChessPiece::addUnrelatedPositionsDeltas(std::vector<Move::position> deltas, std::vector<Move>& moves, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        return;
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addRelatedPositionsDeltas(std::vector<Move::position> deltas, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        std::vector<Move> moves{};
        addRelatedPositionsDeltas(deltas, moves, chessState, onMoveCallback);
        return moves;
    }
    void ChessPiece::addRelatedPositionsDeltas(std::vector<Move::position> deltas, std::vector<Move>& moves, ChessGameState& chessState, void (*onMoveCallback)(GameState&))
    {
        return;
    }
}