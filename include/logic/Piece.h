#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <unordered_map>

#include "Move.h"
class GameBoard;
class Piece
{
    /*
     * A data structure for a piece on the board
    */
    public:
        /* 
         * The possible players (update as needed)
        */
        enum class Player
        {
            white = 0,
            black,
            silver,
            gold,
            last // Here for iteration, do not use as a player!!
        };
    
    private:
        /*
         * The current position of the piece
        */
        Move::position piecePosition{};
        /*
         * The players that can move this piece
         * - Typically, in Chess, only one player can move a given piece. However,
         *   since this is Anarchy Chess, it is possible that multiple people
         *   or perhaps no one at all will be given control of a piece. 
         * - Using an unordered map to store the player will allow pieces to belong
         *   to any number of people.
         * 
         * True means the player can move the piece, False means they cannot
         *   
        */
        std::unordered_map<Player, bool> players{};

        /*
         * The value of the piece, used to calculate each player's score
        */
        int value{};

    public:
        /*
         * Constructor: Set piecePosition
        */
        Piece(Move::position startPos = std::make_pair(0, 0), int pieceValue = 0) 
            : piecePosition{ startPos }, value{ pieceValue } {}
        
        /*
         * Returns true if the player can move this piece and false otherwise
        */
        bool getPlayerAccess(Player player);

        /*
         * Adds a player to the piece, meaning that player can now control the piece
        */
        void addPlayer(Player newPlayer);

        /*
         * Removes a player from the piece, meaning that player can no longer control 
         * the piece
        */
        void removePlayer(Player player);

        /*
         * Returns the position of the piece
        */
        Move::position getPosition();

        /*
         * Updates the piece's internal location
        */
        void changePosition(int newX, int newY);
        void changePosition(Move::position newPosition);

        /*
         * Returns a vector containing all of this piece's valid moves given the 
         * current board position
         * 
         * - Implementation is specific to each type of piece
        */
        // Default initialization for testing only
        virtual std::vector<Move> GenerateMoves(const GameBoard& gameBoard) { return {}; } 

        /*
         * Returns a vector containing every position this piece is attacking
         *
         * - Implementation is specific to each type of piece
        */
        // Default initialization for testing
        virtual std::vector<Move::position> GenerateAttackingSpaces(const GameBoard& gameBoard) { return {}; }

        /*
         * Returns the piece's value
        */
        int getValue();

        /*
         * Sets the piece's value to the new value
        */
        void setValue(int newValue);
};

#endif