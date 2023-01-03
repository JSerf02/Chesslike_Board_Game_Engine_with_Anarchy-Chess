#include <iostream>
#include <utility>

#include "Piece.h"

using Player = Piece::Player;
// See Piece.h
bool getPlayerAccess(Player player)
{
    return true;
}

// See Piece.h
void addPlayer(Player newPlayer)
{
    return;
}

// See Piece.h
void removePlayer(Player player)
{
    return;
}

// See Piece.h
Move::position getPosition()
{
    return { };
}

// See Piece.h
bool changePosition(const GameBoard& gameBoard, int newX, int newY)
{
    return true;
}

// See Piece.h
bool changePosition(const GameBoard& gameBoard, Move::position newPosition)
{
    return changePosition(gameBoard, newPosition);
}

// See Piece.h
int getValue()
{
    return 0;
}

// See Piece.h
void setValue(int newValue)
{
    return;
}