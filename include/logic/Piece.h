#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <unordered_map>

#include "Move.h"
class GameState;
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
         * true means the player can move the piece, false means they cannot
         *   
        */
        std::unordered_map<Player, bool> players{};

        /*
         * The value of the piece, used to calculate each player's score
        */
        double value{};

    public:
        /*
         * Constructor: Set piecePosition
        */
        Piece(Move::position startPos = std::make_pair(0, 0), double pieceValue = 0) 
            : piecePosition{ startPos }, value{ pieceValue } {}
        
        /*
         * Returns true if the player can move this piece and false otherwise
        */
        bool getPlayerAccess(Player player);
        
        /*
         * Returns the entire player access map
        */
        const std::unordered_map<Player, bool>& getAllPlayersAccess();

        /*
         * Adds a player to the piece, meaning that player can now control the piece
        */
        void addPlayer(Player newPlayer);

        /*
         * Adds all players in the inputted vector to the piece
        */
        void addPlayers(std::vector<Player> newPlayers);

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
        virtual std::vector<Move> GenerateMoves(const GameState& gameState) { return {}; } 

        /*
         * Returns a vector all of the moves that attack spaces. 
         * - In Chess, there is an invariant where every position in the move is 
         *   dependent on all previous positions in the same move. This is necessary
         *   for calculating check-preventing moves
         *   - Ex: A rook on (1, 1) may have an attacking Move containing 
         *         {(1, 2), (1, 3), (1, 4), (1, 5), (1, 6), (1, 7), (1, 8)}.
         *         In this case, if a piece moves to (1, 5), all pieces in the following 
         *         set are no longer considered attacked: {(1, 6), (1, 7), (1, 8)}
         *
         * - Implementation is specific to each type of piece
        */
        // Default initialization for testing only
        virtual std::vector<Move> GenerateAttackingMoves(const GameState& gameState) { return {}; }

        /*
         * Returns the piece's value
        */
        double getValue();

        /*
         * Sets the piece's value to the new value
        */
        void setValue(double newValue);
};

#endif