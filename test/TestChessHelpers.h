#ifndef TESTCHESSHELPERS_H
#define TESTCHESSHELPERS_H

#include <vector>
#include <iostream>
#include <set>

#include "ChessGameState.h"
#include "GameBoard.h"
#include "Move.h"
#include "Piece.h"
#include "doctest.h"

using namespace logic;
using namespace chess;

namespace testing {
    // A collection of functions for helping when testing Chess pieces
    class TestChessHelpers
    {
        public:
            static void testPiecePositions(ChessGameState* gameState, Move::position piecePosition, std::set<Move::position>& validPositions) 
            {
                ChessBoard* board = static_cast<ChessBoard*>(gameState->getBoard());
                Piece* piece = board->getPiece(piecePosition);
                REQUIRE(piece != nullptr);
                Player player = piece->getPlayerAccess(Player::white) ? Player::white : Player::black;
                for(int i = board->minX(); i <= board->maxX(); i++) {
                    for(int j = board->minY(); j <= board->maxY(); j++) {
                        Move::position curPosition = std::make_pair(i, j);
                        if(validPositions.find(curPosition) != validPositions.end()) {
                            CHECK(gameState->canMovePiece(player, piecePosition, curPosition));
                        }
                        else {
                            CHECK(gameState->canMovePiece(player, piecePosition, curPosition) == false);
                        }
                    }
                }
            }
            
            static void testPieceAttacks(ChessGameState* gameState, Player player, std::set<Move::position>& attackedPositions) 
            {
                ChessBoard* board = static_cast<ChessBoard*>(gameState->getBoard());
                for(int i = board->minX(); i <= board->maxX(); i++) {
                    for(int j = board->minY(); j <= board->maxY(); j++) {
                        Move::position curPosition = std::make_pair(i, j);
                        if(attackedPositions.find(curPosition) != attackedPositions.end()) {
                            CHECK(gameState->isAttacked(player, curPosition));
                        }
                        else {
                            CHECK(gameState->isAttacked(player, curPosition) == false);
                        }
                    }
                }
            }
    };
}
#endif