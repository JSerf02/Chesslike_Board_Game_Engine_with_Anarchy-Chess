#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "ChessGameState.h"
#include "Piece.h"
#include "Move.h"

/*
* IDs for each piece type
*/
#define KING_ID 1
#define BISHOP_ID 2
#define KNIGHT_ID 3
#define PAWN_ID 4
#define QUEEN_ID 5
#define ROOK_ID 6
namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    class ChessPiece : public Piece
    {
        public:
            /* Inherit the constructure */
            using Piece::Piece;

            /*
            * Returns whether this piece is a king
            */
            bool isKing();

            /*
             * Checks if moving to the position will put the current player into 
             * check and, if it does not, adds the position to the move
             * - Note: If the target position is occupied, this funciton will 
             *         simulate capturing it before checking for check if the 
             *         piece does not belong to the current player and will
             *         return false if it does belong to the current player
             * 
             * Returns:
             * - true if the position was added to the move
             * - false if the position was not added to the move
            */
            bool addToMove(Move::position position, Move& move, ChessGameState& chessState);

            /*
            * Adds a move containing this position to the movelist if the player controls
            * this piece and the position is not occupied by a piece of the same 
            * color
            * 
            * If no vector of moves is inputted, returns:
            * - A vector of moves containing a new move that goes to the target position
            *   if the target position is unoccupied and the player controls the piece
            * - An empty vector of moves otherwise
            * 
            * If a vector of moves is inputted, returns:
            * - true if a new move to the target position was successfully added
            *   to the vector of moves
            * - false if the target position is invalid and a move to that position
            *   cannot be added
            */
            std::vector<Move> addPosition(Move::position position, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);
            bool addPosition(Move::position position, std::vector<Move>& moves, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);
            
            /*
            * Adds moves containing each position in the positions vector to the 
            * movelist if the player controls this piece and the position is not 
            * occupied by a piece of the same color
            * - Each move will contain exactly one position
            * - If a position is not valid, it will be skipped and future positions
            *   will still be added
            * 
            * If no vector of moves is inputted, returns:
            * - A vector of moves containing the new moves that go to the target positions
            *   if the target positions are unoccupied and the player controls the piece
            * - An empty vector of moves otherwise
            */
            std::vector<Move> addUnrelatedPositions(std::vector<Move::position> positions, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);
            void addUnrelatedPositions(std::vector<Move::position> positions, std::vector<Move>& moves, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);
            
            /*
            * Adds a move containing all valid positions in the positions vector 
            * to the movelist if the player controls this piece
            * - Positions are valid if they are not occupied by pieces of the same
            *   color and if no previously added position was inhabited by a piece
            *   of any color
            * - This function will generate exactly one move that contains all 
            *   of the valid positions, the positions will not be divided among
            *   different moves
            * - If a position is not valid, it, along with all future positions
            *   will be skipped
            * 
            * If no vector of moves is inputted, returns:
            * - A vector of moves containing the new move that goes to the target positions
            *   if the target positions are unoccupied and the player controls the piece
            * - An empty vector of moves otherwise
            */
            std::vector<Move> addRelatedPositions(std::vector<Move::position> positions, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);
            void addRelatedPositions(std::vector<Move::position> positions, std::vector<Move>& moves, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);

            /*
            * Finds the target positions by adding all of the deltas in the vector 
            * of deltas to this piece's current position, then adds all the moves
            * as with addUnrelatedPositions()
            * 
            * If no vector of moves is inputted, returns:
            * - A vector of moves containing the new moves that go to the target positions
            *   if the target positions are unoccupied and the player controls the piece
            * - An empty vector of moves otherwise
            */
            std::vector<Move> addUnrelatedPositionsDeltas(std::vector<Move::position> deltas, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);
            void addUnrelatedPositionsDeltas(std::vector<Move::position> deltas, std::vector<Move>& moves, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);

            /*
            * Finds the target positions by adding all of the deltas in the vector 
            * of deltas to this piece's current position, then adds all the moves
            * as with addRelatedPositions()
            * 
            * If no vector of moves is inputted, returns:
            * - A vector of moves containing the new move that goes to the target positions
            *   if the target positions are unoccupied and the player controls the piece
            * - An empty vector of moves otherwise
            */
            std::vector<Move> addRelatedPositionsDeltas(std::vector<Move::position> deltas, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);
            void addRelatedPositionsDeltas(std::vector<Move::position> deltas, std::vector<Move>& moves, ChessGameState& chessState, int priority = 1, void (*onMoveCallback)(Move::position, Move::position, GameState&) = &captureCallback);
    
            static void captureCallback(Move::position start, Move::position end, GameState& gameState);
    };

}
#endif