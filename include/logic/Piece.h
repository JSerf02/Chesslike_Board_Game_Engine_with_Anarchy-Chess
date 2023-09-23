#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <unordered_map>

#include "Move.h"

namespace logic {
    class GameState;
    class Piece
    {
        /*
        * A data structure for a piece on the board
        */
        public:
            using ID = int;

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
            * Stores whether or not the piece is currently on a board
            * 
            * It is possible for a piece to be managed by a board object but not be 
            * on the board
            */
            bool onBoard{ false };

            /* 
            * A cache that stores all of the possible moves for this turn 
            */
            std::vector<Move> moveCache{};

            /* 
            * The most recent turn when the move cache was updated
            */
            int moveCacheUpdateTurn{-1};

            /* 
            * A cache that stores all fo the possible attacking moves for this turn
            */
            std::vector<Move> attackMoveCache{};

            /* 
             * The most recent turn when the attack move cache was updated
            */
            int attackMoveCacheUpdateTurn{-1};

            /*
            * An empty list of moves for when a piece is not controlled by the 
            * current player
            */
            std::vector<Move> emptyMoves{};

        public:
            /*
            * Constructor: Set piecePosition
            */
            Piece(Move::position startPos = std::make_pair(0, 0)); 
            Piece(std::vector<Player> newPlayers, Move::position startPos = std::make_pair(0, 0));
            Piece(Player newPlayer, Move::position startPos = std::make_pair(0, 0)); 
            
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
            * Returns whether the current player controls this piece
            */
            bool controlledByPlayer(GameState& gameState);
            
            /*
            * Returns the position of the piece
            */
            Move::position getPosition();

            /*
            * Updates the piece's internal location
            */
            void changePosition(int newX, int newY);
            void changePosition(Move::position newPosition);

        private:
            /*
            * Returns a vector containing all of this piece's valid moves given the 
            * current board position
            * 
            * - Implementation is specific to each type of piece
            */
            virtual std::vector<Move> generateMoves(GameState& gameState) { return {}; } 

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
            virtual std::vector<Move> generateAttackingMoves(GameState& gameState) { return {}; }

        public:
            /*
             * Returns the ID of the piece
            */
            // Default implementation for testing purposes
            virtual ID getID() { return 0; }

            /* 
            * Updates the move cache if necessary and then returns a reference
            * to the move cache which contains the current possible moves
            */
            std::vector<Move>& getMoves(GameState& gameState);

            /*
            * Updates the attack moves cache if neccessary and then returns a 
            * reference to the attack move cache which contains the current
            * possible attack moves
            */
            std::vector<Move>& getAttackingMoves(GameState& gameState);
            
            /*
            * Returns the maximum priority of all of the moves generated by GenerateMoves()
            */
            int getMaxPriorityOfMoves(GameState& gameState);


            /*
            * Returns a vector containing all of the spaces this piece is attacking
            */
            std::vector<Move::position> getAttackedSpaces(GameState& gameState);

            /*
            * Returns the piece's value
            */
            virtual double getValue() { return 0.0; }

            /*
            * Returns whether or not this piece is on a board
            */
            bool getOnBoard();

            /*
            * Sets whether or not this piece is on a board
            */
            void setOnBoard(bool newValue);

            /* 
            * Gets the minimum priority of moves allowed for this turn.
            * - This behavior allows exclusion of moves based on priority even
            *   if there are no existing moves that meet the priority threshold
            *   - Ex: If a player in check has the possibility to do En Passant,
            *         which has very high priority, even if En Passant is not a
            *         valid move (it will not move the player out of check), the
            *         player still cannot do any moves that have less priority 
            *         than En Passant, thus defining the trademark behavior that
            *         En Passant is forced.
            */
            virtual int getMinPriority(GameState& gameState) { return 0; }
    };
}
#endif