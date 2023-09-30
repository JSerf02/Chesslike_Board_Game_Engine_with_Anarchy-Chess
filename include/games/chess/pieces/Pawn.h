#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"
#include "Action.h"

namespace chess {
    using namespace logic;

    /* Forward declaration (actual class later in the file)*/
    class PawnBoostAction;

    /* A chess piece with a value of 1.0 that moves straight, attacks on diagonals,
       can promote to other pieces, and can boost an extra space forwards on its 
       first turn */
    class Pawn : public ChessPiece
    {
        public:
            /* 
            * Inherit the ChessPiece constructors 
            */
            using ChessPiece::ChessPiece;

            /*
            * Give PawnBoostAction access to all methods of Pawn
            */
            friend class PawnBoostAction;

            /* 
            * The indices of the moves returned when promoting a pawn
            */
            enum class PromotionIdx
            {
                pawn   = 0,
                queen  = 1, // Explicitly writing values since tests rely on the order
                knight = 2,
                rook   = 3,
                bishop = 4,
                knook  = 5,
                length // Stores the size of the enum
            };

            /*
            * Returns the value of the piece, 1.0
            */
            double getValue() override;

            /*
            * Returns the pawn's ID
            */
            ID getID() override;

            /*
            * Returns whether the pawn can boost
            * - The pawn can boost if it has not moved previously
            */
            bool canBoost(GameState& gameState);

            /*
            * Sets the boost turn to the current turn
            */
            void setBoostTurn(GameState& gameState);

            /*
            * Override priority to 10 if En Passant is a possibility
            */
            int getMinPriority(GameState& gameState) override;

        private:
            /*
            * The turn that the pawn boosted
            * - Used for determining whether En Passant is possible
            */
            int boostTurn{};

            /*
            * Returns moves that are one space ahead, diagonal when the space contains
            * an enemy piece, and two spaces ahead if the pawn has not yet moved
            */
            std::vector<Move> generateMoves(GameState& gameState) override;

            /*
            * Returns moves that are one space ahead, diagonal when the space contains
            * an enemy piece, and two spaces ahead if the pawn has not yet moved
            */
            std::vector<Move> generateAttackingMoves(GameState& gameState) override;

            /*
            * Returns whether the pawn boosted on the previous turn
            */
            bool justBoosted(GameState& gameState);

            /*
            * Adds moves to the move list to allow the pawn to move forward once
            * or twice if the pawn can boost
            */
            void addForwardMoves(std::vector<Move>& moves, ChessGameState& chessState);

            /*
            * Adds moves to the move list to allow the pawn to attack on diagonals
            * - When attackOnly is not enabled, moves will only be allowed if they 
            *   are explicitly attacking a piece (pawns can only move to attacked 
            *   spaces if they are explicitly capturing a piece)
            */
            void addStandardAttacks(std::vector<Move>& moves, ChessGameState& chessState, bool attackOnly = false);

            /*
            * Adds moves to the move list to allow the pawn to En Passant, meaning
            * the pawn can attack the space an enemy pawn just boosted past if the
            * enemy boosted on the previous turn
            * - En passant has a priority of 10, essentially making it forced
            * - When attackOnly is enabled, the enemy piece positions will be
            *   returned instead of the uninhabited En Passant positions
            */
            void addEnPassant(std::vector<Move>& moves, ChessGameState& chessState, bool attackOnly = false);

            /*
            * Allows pawns to promote if they move to the opposite end of the board
            * - The order of indices in the move list for promotion is given by 
            *   the PromotionIdx enum
            */
            void addPromotion(std::vector<Move>& moves, ChessGameState& chessState);

            /*
            * Adds promotion moves at promotePosition without performing any validation
            */
            void forceAddPromotion(std::vector<Move>& moves, ChessGameState& chessState, 
                Move::position promotePosition, int priority = 1, std::vector<std::shared_ptr<Action>> extraPreMoves = {});

            /*
            * Allows pawns to take an extra knight move after promoting to a knight
            */
            void addKnightBoost(std::vector<Move>& moves, ChessGameState& chessState);

            /*
            * Adds knight boost moves at boostPosition without performing any validation
            */
            void forceAddKnightBoost(std::vector<Move>& moves, ChessGameState& chessState, 
                Move::position boostPosition, int priority = 1, std::vector<std::shared_ptr<Action>> extraPreMoves = {});
    };

    /* An action that changes the pawn's justBoosted flag to true */
    class PawnBoostAction : public Action 
    {
        private:
            /*
            * The pawn that will boost
            */
            Pawn& pawn;

            /*
            * The GameState object that tracks the current turns of the game
            */
            GameState& gameState;
        public:
            /*
            * Create an action of this type that tracks the inputted pawn
            */
            PawnBoostAction(Pawn& pawn, GameState& gameState) : 
                pawn{pawn}, gameState{gameState}{}

            /*
            * Change the pawn's boost turn
            */
            void applySymptomaticEffects(GameBoard* board) override;
    };

    /* An action that captures all pieces along a diagonal */
    class EnPassantCaptureAction : public Action
    {
        private:
            /*
            * The starting position of the En Passant diagonal
            */
            const Move::position captureStart;

            /*
            * The ending position of the En Passant diagonal
            */
            const Move::position captureEnd;

            /*
            * The pieces that were removed from the board
            */
            std::vector<Piece*> removedPieces{};

        public:
            /* 
            * Creates a new En Passant Capture Action that captures all pieces
            * along the diagonal from captureStart to captureEnd
            */
            EnPassantCaptureAction(Move::position captureStart, Move::position captureEnd) : 
                captureStart{captureStart}, captureEnd{captureEnd}{}

            /*
            * Removes all pieces along the diagonal from captureStart to captureEnd (inclusive)
            * 
            * Returns:
            * - true if all of the pieces were removed successfully
            * - false if at least one of the removals failed
            *   - Reverts all removals in this case
            */
            bool callAction(Move::position end, GameBoard* board) override;

            /*
            * Unremoves all previously removed pieces
            */
            bool reverseAction(GameBoard* board) override;

            /*
            * Captures all previously removed pieces
            */
            void applySymptomaticEffects(GameBoard* board) override;
    };
}
#endif