#include <iostream>
#include <utility>
#include <set>

#include "Piece.h"

namespace logic {
    
    using Player = Piece::Player;

    // See Piece.h
    Piece::Piece(Move::position startPos, double pieceValue) 
        : piecePosition{ startPos }, value{ pieceValue } { }
    Piece::Piece(std::vector<Player> newPlayers, Move::position startPos, double pieceValue) 
        : piecePosition{ startPos }, value{ pieceValue } 
    {
        addPlayers(newPlayers);
    }
    Piece::Piece(Player newPlayer, Move::position startPos, double pieceValue) 
        : piecePosition{ startPos }, value{ pieceValue } 
    {
        addPlayer(newPlayer);
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
    int Piece::getMaxPriorityOfMoves(GameState& gameState)
    {
        // Get all of the piece's moves
        std::vector<Move> moves = generateMoves(gameState);

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
        std::vector<Move> attackMoves = generateAttackingMoves(gameState);
        if(attackMoves.size() == 0) {
            return {};
        }

        std::set<Move::position> attackPositionsSet{};
        for(Move attackMove : attackMoves) {
            const std::vector<Move::position>& curAttackPositions  = attackMove.getPositions();
            for(Move::position position : curAttackPositions) {
                attackPositionsSet.insert(position);
            }
        }

        return { attackPositionsSet.begin(), attackPositionsSet.end() };
    }

    // See Piece.h
    double Piece::getValue()
    {
        return value;
    }

    // See Piece.h
    void Piece::setValue(double newValue)
    {
        value = newValue;
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