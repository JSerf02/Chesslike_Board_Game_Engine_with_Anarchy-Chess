#include <iostream>
#include <utility>

#include "Piece.h"

using Player = Piece::Player;

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
double Piece::getValue()
{
    return value;
}

// See Piece.h
void Piece::setValue(double newValue)
{
    value = newValue;
}