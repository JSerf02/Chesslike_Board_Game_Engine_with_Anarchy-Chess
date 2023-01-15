#include "GameState.h"

using Player = Piece::Player;

// See GameState.h
GameState:: ~GameState() {
    if(gameBoard != nullptr){
        delete gameBoard;
    }
}

// See GameState.h
Player GameState::getPlayer(int idxOffset)
{
    return Player::last;
}

// See GameState.h
bool GameState::setCrntPlayer(int idxOffset)
{
    return true;
}

// See GameState.h
GameBoard* GameState::getBoard()
{
    return {};
}

// See GameState.h
std::vector<Move::position> GameState::getPiecesOfPlayer(Player player)
{
    return {};
}

// See GameState.h
std::vector<Move::position> GameState::getPiecesOfCrntPlayer()
{
    return {};
}

// See GameState.h
int GameState::getMaxPriorityOfPlayer(Player player)
{
    return 0;
}

// See GameState.h
int getMaxPriorityOfCrntPlayer()
{
    return 0;
}

// See GameState.h
std::vector<Move> GameState::getPlayerMovesOfPiece(Player player, int startX, int startY, int endX, int endY)
{
    return {};
}
std::vector<Move> GameState::getPlayerMovesOfPiece(Player player, Move::position start, int endX, int endY)
{
    return {};
}
std::vector<Move> GameState::getPlayerMovesOfPiece(Player player, int startX, int startY, Move::position end)
{
    return {};
}
std::vector<Move> GameState::getPlayerMovesOfPiece(Player player, Move::position start, Move::position end)
{
    return {};
}

// See GameState.h
std::vector<Move> GameState::getMovesOfPiece(int startX, int startY, int endX, int endY)
{
    return {};
}
std::vector<Move> GameState::getMovesOfPiece(Move::position start, int endX, int endY)
{
    return {};
}
std::vector<Move> GameState::getMovesOfPiece(int startX, int startY, Move::position end)
{
    return {};
}
std::vector<Move> GameState::getMovesOfPiece(Move::position start, Move::position end)
{
    return {};
}

// See GameState.h
bool GameState::canPlayerMovePiece(Player player, int startX, int startY, int endX, int endY)
{
    return true;
}
bool GameState::canPlayerMovePiece(Player player, Move::position start, int endX, int endY)
{
    return true;
}
bool GameState::canPlayerMovePiece(Player player, int startX, int startY, Move::position end)
{
    return true;
}
bool GameState::canPlayerMovePiece(Player player, Move::position start, Move::position end)
{
    return true;
}

// See GameState.h
bool GameState::canMovePiece(int startX, int startY, int endX, int endY)
{
    return true;
}
bool GameState::canMovePiece(Move::position start, int endX, int endY)
{
    return true;
}
bool GameState::canMovePiece(int startX, int startY, Move::position end)
{   
    return true;
}
bool GameState::canMovePiece(Move::position start, Move::position end)
{
    return true;
}

// See GameState.h
bool GameState::movePiece(int startX, int startY, int endX, int endY, int idx)
{
    return true;
}
bool GameState::movePiece(Move::position start, int endX, int endY, int idx)
{
    return true;
}
bool GameState::movePiece(int startX, int startY, Move::position end, int idx)
{
    return true;
}
bool GameState::movePiece(Move::position start, Move::position end, int idx)
{
    return true;
}

// See GameState.h
std::vector<Move::position> GameState::getSpacesAttackedByPiece(int x, int y)
{
    return {};
}
std::vector<Move::position> GameState::getSpacesAttackedByPiece(Move::position position)
{
    return {};
}

// See GameState.h
std::vector<Move::position> GameState::getSpacesAttackedByPlayer(Player player)
{
    return {};
}

// See GameState.h
std::vector<Move::position> GameState::getSpacesAttackedByCrntPlayer()
{
    return {};
}

// See GameState.h
std::vector<Move::position> GameState::getSpacesAttackedByPlayers(std::vector<Player> players)
{
    return {};
}

// See GameState.h
std::vector<Move::position> GameState::getAttackedSpaces()
{
    return {};
}

// See GameState.h
bool GameState::isAttacked(int x, int y)
{
    return true;
}
bool GameState::isAttacked(Move::position)
{
    return true;
}

// See GameState.h
std::vector<Move::position> GameState::getPiecesAttackingSpace(int x, int y)
{
    return {};
}
std::vector<Move::position> GameState::getPiecesAttackingSpace(Move::position position)
{
    return {};
}

// See GameState.h
std::vector<Move> GameState::getMovesAttackingSpace(int x, int y)
{
    return {};
}
std::vector<Move> GameState::getMovesAttackingSpace(Move::position position)
{
    return {};
}

// See GameState.h
std::vector<Move::position> GameState::getBlocksProtectingSpace(int x, int y)
{
    return {};
}
std::vector<Move::position> GameState::getBlocksProtectingSpace(Move::position position)
{
    return {};
}

// See GameState.h
std::vector<Move::position> GameState::getSpacesProtectingSpace(int x, int y)
{
    return {};
}
std::vector<Move::position> GameState::getSpacesProtectingSpace(Move::position position)
{
    return {};
}