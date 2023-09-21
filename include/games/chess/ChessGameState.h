#ifndef CHESSGAMESTATE_H
#define CHESSGAMESTATE_H

#include <unordered_map>

#include "GameState.h"
#include "ChessBoard.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    class ChessGameState : public GameState
    {
        private:
            std::unordered_map<Player, Piece*> kings{};

        public:
            /*
             * Initializes the GameState class' gameBoard* pointer to a new ChessBoard,
             * sets the players to White and Black, and stores references to each player's
             * king
            */
            ChessGameState() : GameState(new ChessBoard(), {Player::white, Player::black}) {
                kings[Player::white] = getBoard()->getPiece(std::make_pair(5, 1));
                kings[Player::black] = getBoard()->getPiece(std::make_pair(5, 8));
            }


            /*
             * Returns a reference to the inputted player's king
             * - Defaults to the current player when no player is provided
            */
            Piece* getKing(Player player);
            Piece* getKing();

            /*
             * Returns true if the inputted player is in check or false if they are not
             * - Defaults to the current player when no player is provided
            */
            bool isInCheck(Player player);
            bool isInCheck();

            /*
             * Returns whether or not the inputted move will result in a check for the current player
            */
            bool willMoveCauseCheck(int startX, int startY, int endX, int endY);
            bool willMoveCauseCheck(Move::position start, int endX, int endY);
            bool willMoveCauseCheck(int startX, int startY, Move::position end);
            bool willMoveCauseCheck(Move::position start, Move::position end);

            /*
             * Returns whether or not the inputted player is checkmated
             * - Defaults to the current player when no player is provided
            */
            bool isInCheckmate(Player player);
            bool isInCheckmate();
    };

}
#endif