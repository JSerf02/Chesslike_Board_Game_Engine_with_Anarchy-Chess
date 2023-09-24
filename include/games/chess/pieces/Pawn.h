#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"

namespace chess {
    using namespace logic;

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
            * The indices of the moves returned when promoting a pawn
            */
            enum class PromotionIdx
            {
                pawn   = 0,
                queen  = 1, // Explicitly writing values since tests rely on the order
                knight = 2,
                rook   = 3,
                bishop = 4,
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
            * Allows pawns to take an extra knight move after promoting to a knight
            */
            void addKnightBoost(std::vector<Move>& moves, ChessGameState& chessState);
    };
}
#endif