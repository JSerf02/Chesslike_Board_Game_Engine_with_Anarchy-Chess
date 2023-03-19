#include <vector>
#include <iostream>
#include <algorithm>

#include "doctest.h"
#include "ChessBoard.h"
#include "Move.h"
#include "Piece.h"
#include "GameBoard.h"
#include "GameState.h"

using namespace logic;
using namespace chess;
using Player = Piece::Player;

TEST_CASE("Chess Board - On Board")
{
    // Initialize the Chess Board
    ChessBoard chessBoard{};

    // Check every position that should be on the board is
    for(int i = 1; i <= 8; i++) {
        for(int j = 1; j <= 8; j++) {
            CHECK(chessBoard.onBoard(i, j));
        }
    }

    // Check a variety of possible positions that aren't on the board to ensure that
    // they are not
    CHECK(chessBoard.onBoard(0, 0) == false);
    CHECK(chessBoard.onBoard(-1, 5) == false);
    CHECK(chessBoard.onBoard(6, -1) == false);
    CHECK(chessBoard.onBoard(8, 9) == false);
    CHECK(chessBoard.onBoard(9, 2) == false);
}

TEST_CASE("Chess Board - Setup") 
{
    // Initialize the Chess Board
    ChessBoard chessBoard{};

    // Ensure the chess board has the proper pieces setup
    for(int i = 1; i <= 8; i++) {
        CHECK(chessBoard.occupiedOnBoard(i, 1));
        CHECK(chessBoard.occupiedOnBoard(i, 2));
        for(int j = 3; j <=6; j++){
            CHECK(chessBoard.unoccupiedOnBoard(i, j));
        }
        CHECK(chessBoard.occupiedOnBoard(i, 7));
        CHECK(chessBoard.occupiedOnBoard(i, 8));
    }
}