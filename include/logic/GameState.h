#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <unordered_map>
#include <vector>
#include <string>

#include "HashPair.h"
#include "Piece.h"
#include "Move.h"
#include "GameBoard.h"

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
        */
        GameBoard gameBoard {};

        /*
         * A list of all of the possible players in the game in the order they will
         * play
        */
        std::vector<Player> allPlayers {};

        /*
         * The current player
        */
        Player crntPlayer {};

        /*
         * The index of the current player in allPlayers
        */
        int crntPlayerIdx{};

        /*
         * Stores the maximum priority of all of the current player's possible moves
        */
        int crntMaxPriority{};

    public:
        /*
         * Constructor: Assigns gameBoard to pre-initialized board object and sets 
         * player information
        */
        GameState(GameBoard *board, std::vector<Player> players = { Player::white }) : 
            gameBoard{ *board }, allPlayers{ players }
        {
            if(allPlayers.size() > 0) {
                crntPlayer = allPlayers[0];
            }
        }

        /*
         * Returns the player idxOffset away from the current player
         * - If allPlayers has 0 length, returns Player::last
        */
        Player getPlayer(int idxOffset);

        /*
         * Returns the current player
        */
        Player getCrntPlayer() { return getPlayer(0); }

        /*
         * Sets the current player to the player idxOffset spots away from the current
         * player in allPlayers
         * 
         * Returns:
         * - True if the next player is properly set, even if it is set to the same player
         * - False if there are 0 players and attempt to set the players is made
        */
        bool setCrntPlayer(int idxOffset = 0);

        /*
         * Sets crntPlayer and crntPlayerIdx to their corresponding values for the
         * next player in the game as determined by the player order of allPlayers
         * 
         * Returns:
         * - True if the next player is properly set
         * - False if there are 0 players and nothing is changed
        */
        bool setNextPlayer() { return setCrntPlayer(1); }

        /*
         * Returns a reference to the gameboard
        */
        const GameBoard& getBoard();

        /*
         * Returns the board positions of all of the pieces controlled by a given player
        */
        std::vector<Move::position> getPiecesOfPlayer(Player player);

        /*
         * Returns the board positions of all of the pieces controlled by the current player
        */
        std::vector<Move::position> getPiecesOfCrntPlayer();

        /*
         * Get the max priority of a given player's possible moves
         * - Returns 0 if the player has no moves
        */
        int getMaxPriorityOfPlayer(Player player);

        /*
         * Get the max priority of the current player's possible moves
         * - Returns 0 if the player has no moves
        */
        int getMaxPriorityOfCrntPlayer();

        /*
         * Returns all of the moves a player has that move a piece from start to end
         * - Ex: A pawn on (1, 7) may have multiple moves to (1, 8) for each type of promotion
        */
        std::vector<Move> getPlayerMovesOfPiece(Player player, int startX, int startY, int endX, int endY);
        std::vector<Move> getPlayerMovesOfPiece(Player player, Move::position start, int endX, int endY);
        std::vector<Move> getPlayerMovesOfPiece(Player player, int startX, int startY, Move::position end);
        std::vector<Move> getPlayerMovesOfPiece(Player player, Move::position start, Move::position end);

        /*
         * Returns all of the moves tje current player has that move a piece from start to end
         * - Ex: A pawn on (1, 7) may have multiple moves to (1, 8) for each type of promotion
        */
        std::vector<Move> getMovesOfPiece(int startX, int startY, int endX, int endY);
        std::vector<Move> getMovesOfPiece(Move::position start, int endX, int endY);
        std::vector<Move> getMovesOfPiece(int startX, int startY, Move::position end);
        std::vector<Move> getMovesOfPiece(Move::position start, Move::position end);

        /*
         * Returns true if the inputed player can move a piece from the start 
         * position to the end position and false otherwise
         * - You can move a piece from start to end if:
         *     - There is a piece at start that is controlled by the inputted player
         *     - The piece at start has a move in its list of moves that allows 
         *       it to move to end
         *     - The priority of the move from start to end is equal to crntMaxPriority
        */
        bool canPlayerMovePiece(Player player, int startX, int startY, int endX, int endY);
        bool canPlayerMovePiece(Player player, Move::position start, int endX, int endY);
        bool canPlayerMovePiece(Player player, int startX, int startY, Move::position end);
        bool canPlayerMovePiece(Player player, Move::position start, Move::position end);

        /*
         * Returns true if the current player can move a piece from the start position 
         * to the end position and false otherwise
         * - You can move a piece from start to end if:
         *     - There is a piece at start that is controlled by the current player
         *     - The piece at start has a move in its list of moves that allows 
         *       it to move to end
         *     - The priority of the move from start to end is equal to crntMaxPriority
        */
        bool canMovePiece(int startX, int startY, int endX, int endY);
        bool canMovePiece(Move::position start, int endX, int endY);
        bool canMovePiece(int startX, int startY, Move::position end);
        bool canMovePiece(Move::position start, Move::position end);


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
         * Returns a vector containing all of the spaces the inputted piece attacks
        */
        std::vector<Move::position> getSpacesAttackedByPiece(int x, int y);
        std::vector<Move::position> getSpacesAttackedByPiece(Move::position position);
        
        /*
         * Returns a vector containing all of the spaces the inputted player attacks
        */
        std::vector<Move::position> getSpacesAttackedByPlayer(Player player);

        /*
         * Returns a vector containing all of the spaces the current player attacks
        */
        std::vector<Move::position> getSpacesAttackedByCrntPlayer();

        /*
         * Returns a vector containing all of the spaces attacked by at least one of
         * the players in the inputted vector of players
        */
        std::vector<Move::position> getSpacesAttackedByPlayers(std::vector<Player> players);

        /*
         * Returns a vector containing all of the spaces that are currently under 
         * attack by the opponents (all players besides the current player)
        */
        std::vector<Move::position> getAttackedSpaces();

        /*
         * Returns true if the inputted position is under attack and false otherwise
        */
        bool isAttacked(int x, int y);
        bool isAttacked(Move::position);

        /*
         * Returns a vector containing the positions of all of the pieces an opponent
         * controls that are attacking a given space
        */
        std::vector<Move::position> getPiecesAttackingSpace(int x, int y);
        std::vector<Move::position> getPiecesAttackingSpace(Move::position position);

        /*
         * Returns a vector containing all of the moves of the pieces an opponent
         * controls that are attacking a given space
        */
        std::vector<Move> getMovesAttackingSpace(int x, int y);
        std::vector<Move> getMovesAttackingSpace(Move::position position);

        /*
         * Returns a vector of unoccupied positions which, if moved to, will stop the 
         * inputted space from being attacked
         * 
         * If the inputted position is not being attacked, returns an empty vector
        */
        std::vector<Move::position> getBlocksProtectingSpace(int x, int y);
        std::vector<Move::position> getBlocksProtectingSpace(Move::position position);

        /*
         * Returns a vector of positions which, if moved to, will stop the 
         * inputted space from being attacked
         * 
         * If the inputted position is not being attacked, returns an empty vector
        */
        std::vector<Move::position> getSpacesProtectingSpace(int x, int y);
        std::vector<Move::position> getSpacesProtectingSpace(Move::position position);
};

#endif