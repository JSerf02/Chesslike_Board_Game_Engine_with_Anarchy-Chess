#include <set>

#include "Move.h"
#include "Piece.h"
#include "GameBoard.h"
#include "GameState.h"

namespace logic {

    using Player = Piece::Player;

    // See GameState.h
    GameState::GameState(GameBoard *board, std::vector<Player> players) : 
        gameBoard{ board }, allPlayers{ players }
    {
        if(allPlayers.size() > 0) {
            crntPlayer = allPlayers[0];
        }
        else {
            crntPlayer = Player::last;
        }
        
        // Make sure gameBoard is never null because that could be really annoying
        if(!gameBoard) {
            gameBoard = new GameBoard(players);
        }
        nextTurn();
    }

    // See GameState.h
    GameState::~GameState() {
        if(gameBoard != nullptr){
            delete gameBoard;
        }
    }

    // See GameState.h
    Player GameState::getPlayer(int idxOffset)
    {
        // Make sure there is at least 1 player
        if(allPlayers.size() == 0) {
            return Player::last;
        }

        // Return the requested player
        return allPlayers[(crntPlayerIdx + idxOffset) % allPlayers.size()];
    }

    // See GameState.h
    Player GameState::getCrntPlayer()
    {
        return crntPlayer;
    }

    // See GameState.h
    bool GameState::setCrntPlayer(int idxOffset)
    {
        // Make sure there is at least 1 player
        if(allPlayers.size() == 0) {
            return false;
        }

        // Move to the requested player and set crntPlayer
        crntPlayerIdx = (crntPlayerIdx + idxOffset) % allPlayers.size();
        crntPlayer = allPlayers[crntPlayerIdx];
        return true;
    }

    // See GameState.h
    bool GameState::setNextPlayer() 
    { 
        bool result = setCrntPlayer(1); 
        if(result) {
            nextTurn();
        }
        return result;
    }

    // See GameState.h
    GameBoard* GameState::getBoard()
    {
        return gameBoard;
    }

    // See GameState.h
    std::vector<Move::position> GameState::getPiecesOfCrntPlayer()
    {
        return gameBoard->getPiecesOfPlayer(crntPlayer);
    }

    // See GameState.h
    int GameState::getPriorityOfPlayer(Player player)
    {
        // Get the player's pieces
        std::vector<Move::position> playerPiecePositions = gameBoard->getPiecesOfPlayer(player);
        
        // Priorities below this boundary are not allowed
        int bound;
        if(player == crntPlayer) {
            bound = minPriority;
        }
        else {
            bound = getMinPriority(player);
        }

        // Iterate through the positions to find the maxPriority
        int maxPriority = 0;
        for(Move::position position : playerPiecePositions) {
            Piece* piece = gameBoard->getPiece(position);
            if(!piece) {
                continue;
            }
            int crntMax = piece->getMaxPriorityOfMoves(*this);
            if(crntMax > maxPriority && crntMax >= bound) {
                maxPriority = crntMax;
            }
        }
        return maxPriority;
    }

    // See GameState.h
    int GameState::getPriority() 
    {
        return getPriorityOfPlayer(crntPlayer);
    }

    // See GameState.h
    std::vector<Move> GameState::getMovesOfPiece(Player player, Move::position start, Move::position end)
    {
        Piece* piece = gameBoard->getPiece(start);
        if(!piece || !piece->getPlayerAccess(player))
        {
            return {};
        }

        int priority = getPriorityOfPlayer(player);

        // If the player's move priority is below the minimum allowed priority,
        // then the player has no possible moves
        int bound;
        if(player == crntPlayer) {
            bound = minPriority;
        }
        else {
            bound = getMinPriority(player);
        }
        if(priority < bound) {
            return {};
        }

        std::vector<Move>& allPieceMoves = piece->getMoves(*this);
        std::vector<Move> movesToPosition{};

        for(Move move : allPieceMoves) {
            if(move.getPriority() < priority) {
                continue;
            }
            const std::vector<Move::position>& positions = move.getPositions();
            if(std::find(positions.begin(), positions.end(), end) != positions.end()) {
                movesToPosition.push_back(move);
            }
        }
        return movesToPosition;
    }
    std::vector<Move> GameState::getMovesOfPiece(Player player, int startX, int startY, int endX, int endY)
    {
        return getMovesOfPiece(player, std::make_pair(startX, startY), std::make_pair(endX, endY));
    }
    std::vector<Move> GameState::getMovesOfPiece(Player player, Move::position start, int endX, int endY)
    {
        return getMovesOfPiece(player, start, std::make_pair(endX, endY));
    }
    std::vector<Move> GameState::getMovesOfPiece(Player player, int startX, int startY, Move::position end)
    {
        return getMovesOfPiece(player, std::make_pair(startX, startY), end);
    }
    std::vector<Move> GameState::getMovesOfPiece(int startX, int startY, int endX, int endY)
    {
        return getMovesOfPiece(crntPlayer, std::make_pair(startX, startY), std::make_pair(endX, endY));
    }
    std::vector<Move> GameState::getMovesOfPiece(Move::position start, int endX, int endY)
    {
        return getMovesOfPiece(crntPlayer, start, std::make_pair(endX, endY));
    }
    std::vector<Move> GameState::getMovesOfPiece(int startX, int startY, Move::position end)
    {
        return getMovesOfPiece(crntPlayer, std::make_pair(startX, startY), end);
    }
    std::vector<Move> GameState::getMovesOfPiece(Move::position start, Move::position end)
    {
        return getMovesOfPiece(crntPlayer, start, end);
    }


    // See GameState.h
    bool GameState::canMovePiece(Player player, Move::position start, Move::position end)
    {
        return getMovesOfPiece(player, start, end).size() > 0;
    }
    bool GameState::canMovePiece(Player player, int startX, int startY, int endX, int endY)
    {
        return canMovePiece(player, std::make_pair(startX, startY), std::make_pair(endX, endY));
    }
    bool GameState::canMovePiece(Player player, Move::position start, int endX, int endY)
    {
        return canMovePiece(player, start, std::make_pair(endX, endY));
    }
    bool GameState::canMovePiece(Player player, int startX, int startY, Move::position end)
    {
        return canMovePiece(player, std::make_pair(startX, startY), end);
    }
    bool GameState::canMovePiece(int startX, int startY, int endX, int endY)
    {
        return canMovePiece(crntPlayer, std::make_pair(startX, startY), std::make_pair(endX, endY));
    }
    bool GameState::canMovePiece(Move::position start, int endX, int endY)
    {
        return canMovePiece(crntPlayer, start, std::make_pair(endX, endY));
    }
    bool GameState::canMovePiece(int startX, int startY, Move::position end)
    {   
        return canMovePiece(crntPlayer, std::make_pair(startX, startY), end);
    }
    bool GameState::canMovePiece(Move::position start, Move::position end)
    {
        return canMovePiece(crntPlayer, start, end);
    }

    // See GameState.h
    bool GameState::movePiece(Move::position start, Move::position end, int idx)
    {
        std::vector<Move> movesToEnd = getMovesOfPiece(start, end);
        if(idx >= movesToEnd.size()) {
            return false;
        }

        // Clear all planned moves so they do not have unexpected affects on the
        // current move
        gameBoard->clearFutureMoves();

        // Call the onMove callback function
        Move crntMove = movesToEnd[idx];
        crntMove.callOnMove(start, end, *this);
        
        // Move the piece
        if(!gameBoard->movePiece(start, end)) {
            return false;
        }
        gameBoard->getPiece(end)->validateMove();

        // Apply any planned moves
        gameBoard->applyFutureMoves();

        // Set the next player
        setNextPlayer();
        return true;
    }
    bool GameState::movePiece(int startX, int startY, int endX, int endY, int idx)
    {
        return movePiece(std::make_pair(startX, startY), std::make_pair(endX, endY), idx);
    }
    bool GameState::movePiece(Move::position start, int endX, int endY, int idx)
    {
        return movePiece(start, std::make_pair(endX, endY), idx);
    }
    bool GameState::movePiece(int startX, int startY, Move::position end, int idx)
    {
        return movePiece(std::make_pair(startX, startY), end, idx);
    }

    //See GameState.h
    bool GameState::canMove(Player player)
    {
        GameBoard* board = getBoard();
        if(!board) {
            return false;
        }
        for(Move::position position : board->getPiecesOfPlayer(player)) {
            Piece* piece = board->getPiece(position);
            if(piece->getMoves(*this).size() > 0) {
                return true;
            }
        }
        return false;
    }

    bool GameState::canMove()
    {
        return canMove(getCrntPlayer());
    }

    // See GameState.h
    std::vector<Move::position> GameState::getSpacesAttackedByPlayer(Player player)
    {
        // Get all of the pieces controlled by the player
        std::vector<Move::position> piecePositions = gameBoard->getPiecesOfPlayer(player);

        // Return immediately if the player controls no pieces
        if(piecePositions.size() == 0) {
            return piecePositions;
        }

        // Create a set to store attack positions
        // Using a set to remove duplicate positions
        std::set<Move::position> attackedPositionsSet{};

        // Iterate through each piece's attacking moves and add them all to the set
        for(Move::position piecePosition : piecePositions) {
            Piece* piece = gameBoard->getPiece(piecePosition);
            if(!piece) { // If something went wrong and a piece that was there at the start of the function is gone, abort
                return {};
            }
            // Get current piece's attacks and iterates through them, adding each one to the set
            std::vector<Move::position> pieceAttacks = piece->getAttackedSpaces(*this); 
            for(Move::position attackPosition : pieceAttacks) {
                attackedPositionsSet.insert(attackPosition);
            }
        }
        // Return a vector created by spanning from the beginning to the end of the set
        return { attackedPositionsSet.begin(), attackedPositionsSet.end() };
    }
    std::vector<Move::position> GameState::getSpacesAttackedByPlayer()
    {
        return getSpacesAttackedByPlayer(crntPlayer);
    }

    // See GameState.h
    std::vector<Move::position> GameState::getSpacesAttackedByPlayers(std::vector<Player> players)
    {
        // End immediately if no players are inputted
        if(players.size() == 0) {
            return {};
        }

        // Create a set to store attack positions
        // Using a set to remove duplicate positions
        std::set<Move::position> attackedPositionsSet{};

        // Iterate through each piece's attacking moves and add them all to the set
        for(Player player : players) {
            // Get current piece's attacks and iterates through them, adding each one to the set
            std::vector<Move::position> playerAttacks = getSpacesAttackedByPlayer(player); 
            for(Move::position attackPosition : playerAttacks) {
                attackedPositionsSet.insert(attackPosition);
            }
        }
        // Return a vector created by spanning from the beginning to the end of the set
        return { attackedPositionsSet.begin(), attackedPositionsSet.end() };
    }

    // See GameState.h
    std::vector<Move::position> GameState::getAttackedSpaces(Player player)
    {
        // Create a set to store players
        // Using a set to remove duplicate players
        std::set<Player> opponents{};

        // Iterate through players in the game to add them to the set
        for(Player gamePlayer : allPlayers) {
            if(gamePlayer != player) {
                opponents.insert(gamePlayer);
            }
        }

        // Return spaces that every other player is attacking
        return getSpacesAttackedByPlayers({ opponents.begin(), opponents.end() });
    }
    std::vector<Move::position> GameState::getAttackedSpaces()
    {
        return getAttackedSpaces(crntPlayer);
    }

    // See GameState.h
    bool GameState::isAttacked(Player player, Move::position position)
    {
        std::vector<Move::position> attackedSpaces { getAttackedSpaces(player) };
        return std::find(attackedSpaces.begin(), attackedSpaces.end(), position) != attackedSpaces.end();
    }
    bool GameState::isAttacked(Player player, int x, int y)
    {
        return isAttacked(player, std::make_pair(x, y));
    }
    bool GameState::isAttacked(int x, int y)
    {
        return isAttacked(crntPlayer, std::make_pair(x, y));
    }
    bool GameState::isAttacked(Move::position position)
    {
        return isAttacked(crntPlayer, position);
    }

    // See GameState.h
    int GameState::getTurn() 
    {
        return curTurn;
    }

    // See GameState.h
    void GameState::nextTurn() 
    {
        curTurn++;
        updateMinPriority();
    }

    // See GameState.h
    int GameState::getMinPriority(Player player) 
    {
        // Get the current player's pieces
        std::vector<Move::position> playerPiecePositions = gameBoard->getPiecesOfPlayer(player);

        // Set minPriority to the maximum of each piece's minimum priorities
        int result = 0;
        for(Move::position piecePosition : playerPiecePositions) {
            result = std::max(result, gameBoard->getPiece(piecePosition)->getMinPriority(*this));
        }
        return result;
    }

    // See GameState.h
    void GameState::updateMinPriority() 
    {
        minPriority = getMinPriority(crntPlayer);
    }
}