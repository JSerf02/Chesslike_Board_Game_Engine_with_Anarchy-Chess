#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <unordered_map>
#include <vector>
#include <string>

#include "HashPair.h"
#include "Piece.h"
#include "Move.h"

class GameBoard 
{
    /*
     * The data structure for the game's board
    */
    private:
        /*
         * The board, which uses an unordered_map for O(1) access times for a 
         * given position
         * - hash_tuple::pair_hash is necessary in the template becase by default,
         *   C++ does not support the hashing of pairs. That struct contains a 
         *   hash function definition that works for all pairs to work around this
         * 
         * A null piece means that space is unoccupied
         * All pieces MUST be on the heap
        */
        std::unordered_map<Move::position, Piece*, hash_tuple::pair_hash> board{};

        /*
         * All of the pieces that have been captured so far
        */
        std::unordered_map<Piece::Player, std::vector<Piece*>*> capturedPieces{};
        
        /*
         * Stores every piece associated with the board, even if the piece has 
         * been removed or captured
         * - Necessary for memory management
        */
        std::vector<Piece*> allPieces{};
        /*
         * Adds starting pieces to the board
         * - Called only on initial startup
        */
        virtual void setup() {};

    public:
        /*
         * Constructor: Initialize captured piece vectors and set up the board 
        */
        GameBoard(std::vector<Piece::Player> players = {});

        /*
         * Destructor: Frees all pieces and captured piece vectors
        */
        ~GameBoard();

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
         * Default:
         * - Infinite board, always returns true
        */
        virtual bool onBoard(int x, int y) { return true; };
        bool onBoard(Move::position position);

        /*
         * Determines whether or not a position is occupied on the board
         * 
         * Parameters:
         * - The position on the board
         * 
         * Returns:
         * - true if the position is occupied
         * - false if the position is unoccupied or not on the board
         * 
        */
        bool occupiedOnBoard(int x, int y);
        bool occupiedOnBoard(Move::position position);

        /*
         * Determines whether or not a position is unoccupied and on the board
         * 
         * Parameters:
         * - The position on the board
         * 
         * Returns:
         * - true if the position is unoccupied
         * - false if the position is occupied or not on the board
         * 
         * Note:
         * - This will return the opposite of occupiedOnBoard() if the position
         *   is on the board and the same as occupiedOnBoard() if the position
         *   is not on the board
        */
        bool unoccupiedOnBoard(int x, int y);
        bool unoccupiedOnBoard(Move::position position);

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
         * Removes a piece from the board without freeing it
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
         * captured pieces structure for each player that doesn't control the piece.
         * 
         * Parameters:
         * - The board position of the piece
         * 
         * Returns:
         * - true if the piece was successfully removed
         * - false if something went wrong and no piece was removed
        */
        bool capturePiece(int x, int y);
        bool capturePiece(Move::position position);

        /*
         * Moves a piece to a new position
         * 
         * Parameters:
         * - The starting position of the piece, used to identify which piece to move
         * - The new position of the peice
         * 
         * Returns:
         * - true if the piece was successfully moved
         * - false if the piece was not moved. This can happen if:
         *   - The prev position inputted does not contain a piece
         *   - The new space is occupied
         *   - The new space is not on the board
        */
        bool movePiece(int prevX, int prevY, int newX, int newY);
        bool movePiece(int prevX, int prevY, Move::position newPosition);
        bool movePiece(Move::position prevPosition, int newX, int newY);
        bool movePiece(Move::position prevPosition, Move::position newPosition);
        
        /*
         * Returns a pointer to the vector containing the player's captured pieces
        */
        const std::vector<Piece*>* getPlayerCaptures(Piece::Player player);

        /*
         * Returns the player's current score
        */
        double getPlayerScore(Piece::Player player);
};

#endif