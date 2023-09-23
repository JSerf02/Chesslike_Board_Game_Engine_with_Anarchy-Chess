#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <unordered_map>
#include <vector>
#include <string>

#include "HashPair.h"
#include "Piece.h"
#include "Move.h"
#include "GameBoard.h"

namespace logic {
    using Player = Piece::Player;
    class GameState
    {
        /*
        * A single instance of a given game 
        * - Controls all internal logic and provides a simple interface for accessing
        *   broad information about the current game and using that information to control
        *   the game
        */
        private:
            /*
            * The board representing the current game
            * - Memory for this board is managed by this class, so it MUST be established
            *   on the heap via new
            */
            GameBoard *gameBoard{ nullptr };

            /*
            * A list of all of the possible players in the game in the order they will
            * play
            */
            std::vector<Player> allPlayers{};

            /*
            * The current player
            */
            Player crntPlayer{};

            /*
            * The index of the current player in allPlayers
            */
            int crntPlayerIdx{};

            /*
            * Stores the maximum priority of all of the current player's possible moves
            */
            int crntMaxPriority{};

            /*
            * The current turn in the game, incremented whenever a move is made
            */
            int curTurn{0};

            /*
            * The minimum priority for any move on the current turn
            */
            int minPriority {0};

        public:
            /*
            * Constructor: Assigns gameBoard to pre-initialized board object and sets 
            * player information
            */
            GameState(GameBoard *board = nullptr, std::vector<Player> players = { Player::white });

            /*
            * Destructor: Frees gameBoard
            */
            ~GameState();

            /*
            * Returns the player idxOffset away from the current player
            * - If allPlayers has 0 length, returns Player::last
            */
            Player getPlayer(int idxOffset);

            /*
            * Returns the current player
            */
            Player getCrntPlayer();

            /*
            * Sets the current player to the player idxOffset spots away from the current
            * player in allPlayers
            * 
            * Returns:
            * - True if the next player is properly set, even if it is set to the same player
            * - False if there are 0 players and attempt to set the players is made
            */
            bool setCrntPlayer(int idxOffset = 1);

            /*
            * Sets crntPlayer and crntPlayerIdx to their corresponding values for the
            * next player in the game as determined by the player order of allPlayers
            * 
            * Also increments the turn counter when called. To set the next player
            * without incrementing the turn counter, use setCrntPlayer.
            * 
            * Returns:
            * - True if the next player is properly set
            * - False if there are 0 players and nothing is changed
            */
            bool setNextPlayer();

            /*
            * Returns a reference to the gameboard
            */
            GameBoard* getBoard();

            /*
            * Returns the board positions of all of the pieces controlled by the current player
            */
            std::vector<Move::position> getPiecesOfCrntPlayer();

            /*
            * Get the priority of a given player's possible moves
            * - Returns 0 if the player has no moves
            */
            int getPriorityOfPlayer(Player player);

            /*
            * Get the priority of the current player's possible moves
            * - Returns 0 if the player has no moves
            */
            int getPriority();

            /*
            * Returns all of the moves a player has that move a piece from start to end
            * - Ex: A pawn on (1, 7) may have multiple moves to (1, 8) for each type of promotion
            * - Note that moves of priority less than the player's max priority do not count
            * 
            * If a player is specefied, only counts moves of that player. Otherwise,
            * only counts moves of the default player
            */
            std::vector<Move> getMovesOfPiece(Player player, int startX, int startY, int endX, int endY);
            std::vector<Move> getMovesOfPiece(Player player, Move::position start, int endX, int endY);
            std::vector<Move> getMovesOfPiece(Player player, int startX, int startY, Move::position end);
            std::vector<Move> getMovesOfPiece(Player player, Move::position start, Move::position end);
            std::vector<Move> getMovesOfPiece(int startX, int startY, int endX, int endY);
            std::vector<Move> getMovesOfPiece(Move::position start, int endX, int endY);
            std::vector<Move> getMovesOfPiece(int startX, int startY, Move::position end);
            std::vector<Move> getMovesOfPiece(Move::position start, Move::position end);

            /*
            * Returns true if the inputted player can move a piece from the start position 
            * to the end position and false otherwise
            * - You can move a piece from start to end if:
            *     - There is a piece at start that is controlled by the current player
            *     - The piece at start has a move in its list of moves that allows 
            *       it to move to end
            *     - The priority of the move from start to end is equal to crntMaxPriority
            * - If no player is inputted, the current player is used
            */
            bool canMovePiece(int startX, int startY, int endX, int endY);
            bool canMovePiece(Move::position start, int endX, int endY);
            bool canMovePiece(int startX, int startY, Move::position end);
            bool canMovePiece(Move::position start, Move::position end);
            bool canMovePiece(Player player, int startX, int startY, int endX, int endY);
            bool canMovePiece(Player player, Move::position start, int endX, int endY);
            bool canMovePiece(Player player, int startX, int startY, Move::position end);
            bool canMovePiece(Player player, Move::position start, Move::position end);


            /*
            * Moves a piece from start to end. After moving the piece, calls the move's
            * onMove() callback function and sets the next player
            * 
            * Parameters:
            * - The starting position of the move
            * - The ending position of the move
            * - The index of the move in the vector returned by getMovesOfPiece()
            *   - Used to determine which move callback to use when there are multiple options
            *   - The choice of move should be determined by calling getMovesOfPiece()
            *     BEFORE calling this function
            * 
            * Returns:
            * - true if the piece was moved successfully
            * - false if the piece didn't move
            *   - This can happen if:
            *     - You cannot move the piece (see canMovePiece())
            *     - The index is not a possible index for the moves list generated by 
            *       getMovesOfPiece()
            */
            bool movePiece(int startX, int startY, int endX, int endY, int idx = 0);
            bool movePiece(Move::position start, int endX, int endY, int idx = 0);
            bool movePiece(int startX, int startY, Move::position end, int idx = 0);
            bool movePiece(Move::position start, Move::position end, int idx = 0);

            /*
            * Returns whether or not the inputted player has at least one valid move
            * - Defaults to the current player when no player is provided
            */
            bool canMove(Player player);
            bool canMove();
            
            /*
            * Returns a vector containing all of the spaces the inputted player attacks
            * - If no player is inputted, defaults to the current player
            */
            std::vector<Move::position> getSpacesAttackedByPlayer(Player player);
            std::vector<Move::position> getSpacesAttackedByPlayer();     

            /*
            * Returns a vector containing all of the spaces attacked by at least one of
            * the players in the inputted vector of players
            */
            std::vector<Move::position> getSpacesAttackedByPlayers(std::vector<Player> players);

            /*
            * Returns a vector containing all of the spaces that are currently under 
            * attack by the opponents of the inputted player (all players besides them)
            * - If no player is inputted, defaults to the current player
            */
            std::vector<Move::position> getAttackedSpaces(Player player);
            std::vector<Move::position> getAttackedSpaces();

            /*
            * Returns true if the inputted position is under attack by the inputted
            * player's opponents and false if not
            * - If no player is inputted, defaults to the current player
            */
            bool isAttacked(Player player, int x, int y);
            bool isAttacked(Player player, Move::position position);
            bool isAttacked(int x, int y);
            bool isAttacked(Move::position position);

            /*
            * Returns the current turn in the game
            */
            int getTurn();

            /*
             * Increments the turn count and updates the minimum priority for the current turn
             * - Note: The function is called automatically by setNextPlayer()
             *         so you will not need to call this in regular circumstances
            */
            void nextTurn();

        private:
            /* 
            * Returns the minimum priority of all moves for the inputted player
            */
            int getMinPriority(Player player);

            /* 
            * Updates the minimum priority based on current player
            * - Note: Called automatically in most circumstances, but make sure
            *         to call this whenever pieces are added in the middle of a turn,
            *         which should hopefully happen rarely if at all.
            */
            void updateMinPriority();
    };
}
#endif