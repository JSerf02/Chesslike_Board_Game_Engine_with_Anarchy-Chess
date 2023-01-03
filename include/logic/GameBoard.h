#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <unordered_map>
#include <vector>
#include <string>

#include "Piece.h"
#include "Move.h"

class GameBoard {
    /*
     * The data structure for the game's board
    */
    private:
        /*
         * The board, which uses an unordered_map for O(1) access times for a 
         * given position
         * - The string key is meant to be a pair, but pairs keys are not supported
         * 
         * A null piece means that space is unoccupied
        */
        std::unordered_map<std::string, Piece*> board{};

        /*
         * All of the pieces that have been captured so far
        */
        std::unordered_map<Piece::Player, std::vector<Piece*>> capturedPieces{};

        /*
         * Adds starting pieces to the board
         * - Called only on initial startup
        */
        virtual void setup() {};

    public:
        /*
         * Constructor: Sets up the board
        */
        GameBoard();

        /*
         * Determines whether or not a location is on the board
         * - Implementation is game-specific, but should always be fast as this 
         *   is called frequently
         * 
         * Parameters: 
         * - The position on the board
         * 
         * Returns:
         * - true if the position is located on the board, regardless of 
         *   whether that position is occupied
         * - false if the position is not on the board
         * 
        */
        virtual bool onBoard(int x, int y) { return false; };
        bool onBoard(Move::position position);

        /*
         * Determines whether or not a position is occupied on the board
         * 
         * Parameters:
         * - The position on the board
         * 
         * Returns:
         * - true if the position is unoccupied
         * - false if the position is occupied or not on the board
         * 
        */
        bool occupiedOnBoard(int x, int y);
        bool occupiedOnBoard(Move::position position);

        /*
         * Adds a piece to the board
         * - Pieces MUST be on the heap!
         * 
         * Returns:
         * - true if successfully added
         * - false if there was an error adding, such as with the piece's position
        */
        bool addPiece(Piece* piece);

        /*
         * Retrieves a piece from a position
         * 
         * Parameters:
         * - The board position of the piece
         * 
         * Returns:
         * - A pointer to the piece at that position
         * - nullptr if there is no piece at that position or the position is invalid
        */
        Piece* getPiece(int x, int y);
        Piece* getPiece(Move::position position);

        /*
         * Removes a piece from a position, freeing it afterwards
         * 
         * Parameters:
         * - The board position of the piece
         * 
         * Returns:
         * - true if the piece was successfully removed
         * - false if something went wrong and no piece was removed
        */
        bool removePiece(int x, int y);
        bool removePiece(Move::position position);

        /*
         * Captures a piece by removing it from the board and adding it to the 
         * captured pieces structure
         * 
         * Parameters:
         * - The board position of the piece
         * 
         * Returns:
         * - true if the piece was successfully removed
         * - false if something went wrong and no piece was removed
        */
        bool capturePiece(int x, int y);
        bool capturePieece(Move::position position);
        
        /*
         * Returns a reference to the vector containing the player's captured pieces
        */
        const std::vector<Piece*>& getPlayerCaptures(Piece::Player player);

        /*
         * Returns the player's current score
        */
        int getPlayerScore(Piece::Player player);
};

#endif