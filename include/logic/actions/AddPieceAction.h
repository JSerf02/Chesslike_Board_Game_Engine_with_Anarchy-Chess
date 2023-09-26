#ifndef ADD_PIECE_ACTION_H
#define ADD_PIECE_ACTION_H

#include <stdlib.h>
#include <utility>
#include <vector>
#include <iostream>
#include <functional>

#include "GameBoard.h"
#include "Move.h"
#include "Piece.h"

namespace logic {
    /* An action that adds a new piece to the board */
    class AddPieceAction : public Action
    {
        public:
            using Player = Piece::Player;
            using PieceConstructor = std::function<Piece* (std::vector<Player>, Move::position)>;
            
        private:
            /*
            * The players assigned to the new piece when adding a new piece
            */
            std::vector<Player> players{};

            /*
            * The position relative to the end position of the move to spawn the
            * new piece
            */
            Move::position spawnPos{};

            /*
            * The function that creates a new piece of the specified type on the heap
            */
            PieceConstructor pieceConstructor{};

        public:
            /*
            * Create an action of this type with the inputted players, spawn position, and piece constructor
            * - The players inputted are the players that will be able to control
            *   the new piece
            * - The spawn position is the position relative to the end position of
            *   the move that the new piece should spawn at
            * - The piece constructor is the function that creates the new piece
            *   on the heap
            */
            AddPieceAction(PieceConstructor pieceConstructor = Piece::createNew, Move::position spawnPos = std::make_pair(0, 0)) :
                spawnPos{spawnPos}, pieceConstructor{pieceConstructor}{}
            AddPieceAction(Move::position spawnPos, PieceConstructor pieceConstructor = Piece::createNew) :
                spawnPos{spawnPos}, pieceConstructor{pieceConstructor}{}
            AddPieceAction(Player player, PieceConstructor pieceConstructor = Piece::createNew, Move::position spawnPos = std::make_pair(0, 0)) :
                players{{player}}, spawnPos{spawnPos}, pieceConstructor{pieceConstructor}{}
            AddPieceAction(std::vector<Player> players, PieceConstructor pieceConstructor = Piece::createNew, Move::position spawnPos = std::make_pair(0, 0)) :
                players{players}, spawnPos{spawnPos}, pieceConstructor{pieceConstructor}{}

            /*
            * Creates a piece at end + spawnPos using pieceConstructor
            * - Also calls validateMove() for the piece so it knows it was created
            *   during a game
            *
            * Returns:
            * - true if the new piece was successfully created and added to the board
            * - false if the piece could not be created or could not be added to the board
            */
            bool callAction(Move::position end, GameBoard* board) override;

            /*
            * Removes and frees the piece created during the most recent call 
            * of callAction()
            * - Note: The implementation of this function relies on the board
            *         having added no new pieces and moved no pieces since the
            *         callAction() call
            */
            bool reverseAction(GameBoard* board) override;
    };
}
#endif