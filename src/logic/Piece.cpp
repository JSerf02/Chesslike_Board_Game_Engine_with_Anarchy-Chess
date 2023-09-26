#include <iostream>
#include <utility>
#include <set>

#include "Piece.h"

namespace logic {
    
    using Player = Piece::Player;

    // Let the compiler know that these functions exist
    class GameBoard {
        public:
            bool inSimulation();
    };

    class GameState {
        public: 
            int getPriority();
            int getTurn();
            Player getCrntPlayer();
            int getPriorityOfPlayer(Player player);
            GameBoard* getBoard();
    };

    // See Piece.h
    Piece::Piece(Move::position startPos) : piecePosition{ startPos } 
    { 
        return;
    }
    Piece::Piece(std::vector<Player> newPlayers, Move::position startPos) : piecePosition{ startPos }
    {
        addPlayers(newPlayers);
    }
    Piece::Piece(Player newPlayer, Move::position startPos) : piecePosition{ startPos }
    {
        addPlayer(newPlayer);
    }

    // See Piece.h
    Piece* Piece::createNew(std::vector<Player> newPlayers, Move::position startPos)
    {
        return new Piece(newPlayers, startPos);
    }

    // See Piece.h
    bool Piece::getPlayerAccess(Player player)
    {
        return players[player];
    }

    // See Piece.h
    const std::unordered_map<Player, bool>& Piece::getAllPlayersAccess()
    {
        return players;
    }

    // See Piece.h
    void Piece::addPlayer(Player newPlayer)
    {
        players[newPlayer] = true;
    }

    // See Piece.h
    void Piece::addPlayers(std::vector<Player> newPlayers) 
    {
        for(Player player : newPlayers) {
            addPlayer(player);
        }
    }

    // See Piece.h
    void Piece::removePlayer(Player player)
    {
        players[player] = false;
    }

    // See Piece.h
    bool Piece::controlledByPlayer(GameState& gameState)
    {
        Player player = gameState.getCrntPlayer();
        return getPlayerAccess(player);
    }

    // See Piece.h
    Move::position Piece::getPosition()
    {
        return piecePosition;
    }

    // See Piece.h
    void Piece::changePosition(int newX, int newY)
    {
        piecePosition.first = newX;
        piecePosition.second = newY;
    }

    // See Piece.h
    void Piece::changePosition(Move::position newPosition)
    {
        changePosition(newPosition.first, newPosition.second);
    }
    
    // See Piece.h
    bool Piece::previouslyMoved()
    {
        return alreadyMoved;
    }

    // See Piece.h
    void Piece::validateMove()
    {
        alreadyMoved = true;
    }

    // See Piece.h
    void Piece::unValidateMove()
    {
        alreadyMoved = false;
    }

    // See Piece.h
    std::vector<Move>& Piece::getMoves(GameState& gameState, bool ignorePriority) 
    {
        // Store values for later
        int curTurn = gameState.getTurn();
        std::vector<Move>* cache;
        GameBoard* board = gameState.getBoard();

        // Get the simulated cache if you are in a simulation
        if(board && board->inSimulation()) {
            cache = &simulatedMoveCache;

            // Always updated the simulated cache
            *cache = generateMoves(gameState);
        }
        else { // Get the regular cache otherwise
            cache = &moveCache;

            // Update the cache if it has not been updated this turn
            if(moveCacheUpdateTurn != curTurn) {
                *cache = generateMoves(gameState);
                moveCacheUpdateTurn = curTurn;
            }
        }

        // Do not check priority if the current player does not control the piece
        // or if ignorePriority is enabled
        if(!controlledByPlayer(gameState) || ignorePriority) {
            return *cache;
        }

        // Filter out moves that don't have high enough priorities
        std::vector<Move> validMoves{};
        
        int priority = gameState.getPriority();
        if(priority == 0) { // 0 priority means there are no valid moves
            *cache = validMoves;
            return *cache;
        }
        for(Move move : *cache) {
            if(move.getPriority() < priority) {
                continue;
            }
            validMoves.push_back(move);
        }
        *cache = validMoves;
        return *cache;
    }

    // See Piece.h
    std::vector<Move>& Piece::getAttackingMoves(GameState& gameState, bool ignorePriority) 
    {
        // Store values for later
        int curTurn = gameState.getTurn();
        std::vector<Move>* cache;
        GameBoard* board = gameState.getBoard();

        // Get the simulated cache if you are in a simulation
        if(board && board->inSimulation()) {
            cache = &simulatedAttackMoveCache;

            // Always updated the simulated cache
            *cache = generateAttackingMoves(gameState);
        }
        else { // Get the regular cache otherwise
            cache = &attackMoveCache;

            // Update the cache if it has not been updated this turn
            if(attackMoveCacheUpdateTurn != curTurn) {
                *cache = generateAttackingMoves(gameState);
                attackMoveCacheUpdateTurn = curTurn;
            }
        }

        return *cache;
    }

    // See Piece.h
    int Piece::getMaxPriorityOfMoves(GameState& gameState)
    {
        // Get all of the piece's moves
        std::vector<Move> moves = getMoves(gameState, true);

        // Iterate through the moves to find the max priority
        int maxPriority = 0;
        for(Move move : moves) {
            int curPriority = move.getPriority();
            if(curPriority > maxPriority) {
                maxPriority = curPriority;
            }
        }
        return maxPriority;
    }

    // See Piece.h
    std::vector<Move::position> Piece::getAttackedSpaces(GameState& gameState)
    {
        std::vector<Move> attackMoves = getAttackingMoves(gameState);
        if(attackMoves.size() == 0) {
            return {};
        }

        std::set<Move::position> attackPositionsSet{};
        for(Move attackMove : attackMoves) {
            const std::vector<Move::position>& curAttackPositions = attackMove.getPositions();
            for(Move::position position : curAttackPositions) {
                attackPositionsSet.insert(position);
            }
        }

        return { attackPositionsSet.begin(), attackPositionsSet.end() };
    }

    // See Piece.h
    bool Piece::getOnBoard() 
    {
        return onBoard;
    }

    // See Piece.h
    void Piece::setOnBoard(bool newValue)
    {
        onBoard = newValue;
    }
}