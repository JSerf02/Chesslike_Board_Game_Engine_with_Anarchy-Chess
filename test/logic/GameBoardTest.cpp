#include <vector>
#include <iostream>

#include "doctest.h"
#include "GameBoard.h"
#include "Move.h"
#include "Piece.h"
#include "TestBoard.h"
#include "TestPieces.h"

using namespace testing;
using namespace logic;

TEST_CASE("Game Board: On board for GameBoard and PositiveXBoard classes") 
{
    // Create positions for testing
    Move::position correctPosition = std::make_pair(5, 5); 
    Move::position incorrectPosition = std::make_pair(-5, 5);

    // Ensure the correct output from the default board implementation of OnBoard
    GameBoard defaultBoard{};
    CHECK(defaultBoard.onBoard(5, 5)); // Using the int version
    CHECK(defaultBoard.onBoard(incorrectPosition)); // Using the position version 

    

    PositiveXBoard xBoard{};
    CHECK(xBoard.onBoard(5, 5)); // Using the int version
    CHECK(xBoard.onBoard(incorrectPosition) == false); // Using the position version 
    /* It is important that the final call tests the position version the position
       the position version implicitly calls the original Gameboard class's definition
       of the position version which in turn calls the int version. If something
       goes wrong during this process, it is possible that calling the int version
       through the parent's function could potentially call the Gameboard version 
       of the int version instead of the overrided int version of PositiveXBoard
    */
}

TEST_CASE("Game Board: Add piece - proper return values")
{
    // Create an xBoard so there is a possibility of pieces not being on the board
    PositiveXBoard xBoard{};

    // Create a piece on the board and a piece that cannot be on the board
    Piece* correctPiece = new Piece{};
    Piece* incorrectPiece = new Piece{std::make_pair(-5, 1)};
    REQUIRE(correctPiece != nullptr);
    REQUIRE(incorrectPiece != nullptr);

    // Make sure the piece on the board is properly added
    CHECK(xBoard.addPiece(correctPiece));
    CHECK(correctPiece->getOnBoard());

    // Make sure error conditions are properly handled
    CHECK(xBoard.addPiece(incorrectPiece) == false);
    CHECK(xBoard.addPiece(nullptr) == false);
    
}

TEST_CASE("Game Board: Occupied on board, unoccupied on board, and add piece updating internal structures")
{
    // Create an xBoard so there is a possibility of pieces not being on the board
    PositiveXBoard xBoard{};

    // Check for proper unoccupied on board statuses in default cases
    CHECK(xBoard.occupiedOnBoard(1, 1) == false);
    CHECK(xBoard.occupiedOnBoard(-1, 1) == false); // Case where the point is not on the board
    CHECK(xBoard.occupiedOnBoard(std::make_pair(2, 7)) == false);

    // Check for proper unoccupied on board statuses in default cases
    CHECK(xBoard.unoccupiedOnBoard(1, 1));
    CHECK(xBoard.unoccupiedOnBoard(-1, 1) == false); // Case where the point is not on the board
    CHECK(xBoard.unoccupiedOnBoard(std::make_pair(2, 7)));
    
    // Add a piece to the board
    Piece* piece = new Piece{};
    CHECK(xBoard.addPiece(piece));

    // Make sure the piece is reported as on the board
    CHECK(xBoard.occupiedOnBoard(0, 0));
    CHECK(xBoard.occupiedOnBoard(std::make_pair(0, 0)));
    CHECK(xBoard.unoccupiedOnBoard(0, 0) == false);
    CHECK(xBoard.unoccupiedOnBoard(std::make_pair(0, 0)) == false);
}

TEST_CASE("Game Board: Get piece")
{
    // Create a board
    GameBoard board{};
    
    // Add a piece to the board
    Piece* piece = new Piece{};
    CHECK(board.addPiece(piece));

    // Make sure the piece is properly returned
    CHECK(board.getPiece(0, 0) == piece);
    CHECK(board.getPiece(std::make_pair(0, 0)) == piece);

    // Make sure other positions still have no pieces
    CHECK(board.getPiece(1, 0) == nullptr);
    CHECK(board.getPiece(std::make_pair(1, 0)) == nullptr);
}

TEST_CASE("Game Board: Add Pieces")
{
    // Create a board
    PositiveXBoard xBoard{};

    // Add 2 pieces to the board
    Piece* piece1 = new Piece{};
    Piece* piece2 = new Piece{std::make_pair(1, 0)};
    std::vector<Piece*> pieces {piece1, piece2};
    CHECK(xBoard.addPieces(pieces));

    // Make sure both pieces are properly returned
    CHECK(xBoard.getPiece(0, 0) == piece1);
    CHECK(xBoard.getPiece(std::make_pair(0, 0)) == piece1);
    CHECK(xBoard.getPiece(1, 0) == piece2);
    CHECK(xBoard.getPiece(std::make_pair(1, 0)) == piece2);

    // Make sure other positions still have no pieces
    CHECK(xBoard.getPiece(2, 0) == nullptr);
    CHECK(xBoard.getPiece(std::make_pair(2, 0)) == nullptr);

    // Make sure adding 2 pieces to the same position fails and adds only the first one
    Piece* samePiece1 = new Piece{std::make_pair(3, 0)};
    Piece* samePiece2 = new Piece{std::make_pair(3, 0)};
    CHECK(xBoard.addPieces( {samePiece1, samePiece2} ) == false);
    CHECK(xBoard.getPiece(3, 0) == samePiece1);

    // Make sure adding a piece that's not on the board fails and that adding stops after
    // a single add fails
    Piece* legalPiece1 = new Piece{std::make_pair(4, 0)};
    Piece* illegalPiece = new Piece{std::make_pair(-1, 0)};
    Piece* legalPiece2 = new Piece{std::make_pair(5, 0)};
    CHECK(xBoard.addPieces({ legalPiece1, illegalPiece, legalPiece2 }) == false);
    CHECK(xBoard.getPiece(4, 0) == legalPiece1);
    CHECK(xBoard.getPiece(5, 0) == nullptr);

    delete samePiece2;
    delete illegalPiece;
    delete legalPiece2;
}

TEST_CASE("Game Board: Remove piece") {
    // Create an xBoard so there is a possibility of pieces not being on the board
    PositiveXBoard xBoard{};

    // Check that edge cases where nothing is removed correctly return false
    CHECK(xBoard.removePiece(5, 4) == false);
    CHECK(xBoard.removePiece(std::make_pair(-5, 1)) == false);

    // Add pieces to the board and make sure they are properly added
    Piece* piece1 = new Piece{};
    Piece* piece2 = new Piece{std::make_pair(1, 1)};
    CHECK(xBoard.addPiece(piece1));
    CHECK(xBoard.addPiece(piece2));
    CHECK(xBoard.occupiedOnBoard(0, 0));
    CHECK(xBoard.occupiedOnBoard(1, 1));

    // Remove the 2 added pieces using the int and pair versions
    CHECK(xBoard.removePiece(0, 0));
    CHECK(xBoard.occupiedOnBoard(0, 0) == false);
    CHECK(xBoard.occupiedOnBoard(1, 1));
    CHECK(xBoard.removePiece(std::make_pair(1, 1)));
    CHECK(xBoard.occupiedOnBoard(1, 1) == false);
    CHECK(piece1->getOnBoard() == false);
    CHECK(piece2->getOnBoard() == false);
}

TEST_CASE("Game Board: Move pieces - default usage") 
{
    // Create an xBoard so there is a possibility of pieces not being on the board
    PositiveXBoard xBoard{};

    // Add a new piece to the board at (0, 0)
    Piece* piece = new Piece{};
    xBoard.addPiece(piece);
    CHECK(piece->getPosition() == std::make_pair(0, 0));
    CHECK(xBoard.occupiedOnBoard(0, 0));

    // Move the piece 4 times and ensure proper outputs for each variation of the function
    for(int i = 1; i <= 4; i++) {
        switch(i) {
            case 1:
                CHECK(xBoard.movePiece(0, 0, 1, 1));
                break;
            case 2:
                CHECK(xBoard.movePiece(std::make_pair(1, 1), 2, 2));
                break;
            case 3:
                CHECK(xBoard.movePiece(2,2, std::make_pair(3, 3)));
                break;
            case 4:
                CHECK(xBoard.movePiece(std::make_pair(3, 3), std::make_pair(4, 4)));
                break;
        }
        CHECK(piece->getPosition() == std::make_pair(i, i));
        CHECK(xBoard.occupiedOnBoard(i, i));
        CHECK(xBoard.occupiedOnBoard(i - 1, i - 1) == false);
    }
}

TEST_CASE("Game Board: Move pieces - error conditions")
{
    // Create an xBoard so there is a possibility of pieces not being on the board
    PositiveXBoard xBoard{};

    // Check error conditions where the prev position is not on the board
    CHECK(xBoard.movePiece(0, 0, 1, 1) == false);
    CHECK(xBoard.movePiece(std::make_pair(2, 2), 1, 1) == false);
    CHECK(xBoard.movePiece(3, 3, std::make_pair(2, 2)) == false);
    CHECK(xBoard.movePiece(std::make_pair(2, 2), std::make_pair(2, 2)) == false);

    // Add two more pieces to the board
    Piece* piece1 = new Piece{};
    Piece* piece2 = new Piece{std::make_pair(1, 1)};
    xBoard.addPiece(piece1);
    xBoard.addPiece(piece2);

    // Check error conditions where the new position is not on the board
    CHECK(xBoard.movePiece(0, 0, -1, 1) == false);
    CHECK(xBoard.movePiece(std::make_pair(0, 0), -2, 2) == false);
    CHECK(xBoard.movePiece(0, 0, std::make_pair(-3, 3)) == false);
    CHECK(xBoard.movePiece(std::make_pair(0, 0), std::make_pair(-4, 4)) == false);

    // Check error conditions where the new position is occupied
    CHECK(xBoard.movePiece(0, 0, 1, 1) == false);
    CHECK(xBoard.movePiece(std::make_pair(0, 0), 1, 1) == false);
    CHECK(xBoard.movePiece(0, 0, std::make_pair(1, 1)) == false);
    CHECK(xBoard.movePiece(std::make_pair(0, 0), std::make_pair(1, 1)) == false);
}

using Player = Piece::Player;

TEST_CASE("Game Board: Get pieces controlled by player")
{
    // Create a gameBoard with 2 players
    GameBoard gameBoard{ { Player::white, Player::black } };

    // Add 3 white pieces and 2 black piecces to the board
    Piece* whitePiece1 = new Piece{Player::white};
    Piece* whitePiece2 = new Piece{Player::white, std::make_pair(1, 0)};
    Piece* whitePiece3 = new Piece{Player::white, std::make_pair(2, 0)};
    Piece* blackPiece1 = new Piece{Player::black, std::make_pair(0, 1)};
    Piece* blackPiece2 = new Piece{Player::black, std::make_pair(0, 2)};
    CHECK(gameBoard.addPieces({ whitePiece1, whitePiece2, whitePiece3, blackPiece1, blackPiece2 }));

    // Get the white and black pieces and also get the pieces of the current player (should be white)
    std::vector<Move::position> whitePieces = gameBoard.getPiecesOfPlayer(Player::white);
    std::vector<Move::position> blackPieces = gameBoard.getPiecesOfPlayer(Player::black);

    // Make sure pieces were properly returned
    for(int i = 0; i <=2; i++) { // (0, 0), (1, 0), and (2, 0) are in whitePieces and not blackPieces
        CHECK(std::find(whitePieces.begin(), whitePieces.end(), std::make_pair(i, 0)) != whitePieces.end());
        CHECK(std::find(blackPieces.begin(), blackPieces.end(), std::make_pair(i, 0)) == blackPieces.end());
    }
    for(int j = 1; j <= 2; j++) { // (0, 1), (0, 2) are in blackPieces and not whitePieces
        CHECK(std::find(whitePieces.begin(), whitePieces.end(), std::make_pair(0, j)) == whitePieces.end());
        CHECK(std::find(blackPieces.begin(), blackPieces.end(), std::make_pair(0, j)) != blackPieces.end());
    }

    gameBoard.removePiece(0, 0);
    std::vector<Move::position> newWhitePieces = gameBoard.getPiecesOfPlayer(Player::white);
    CHECK(std::find(newWhitePieces.begin(), newWhitePieces.end(), std::make_pair(0, 0)) == newWhitePieces.end());
}

TEST_CASE("Game Board: Get default player captures")
{
    // Create a new GameBoard object
    GameBoard board{{Player::white, Player::silver, Player::gold}};

    // Make sure the player's captures are properly initialized by the constructor
    REQUIRE(board.getPlayerCaptures(Player::white) != nullptr);
    REQUIRE(board.getPlayerCaptures(Player::black) == nullptr); // Black is not mentioned in the constructor
    REQUIRE(board.getPlayerCaptures(Player::silver) != nullptr);
    REQUIRE(board.getPlayerCaptures(Player::gold) != nullptr);

    // Check that the default capture vectors are all empty
    CHECK(board.getPlayerCaptures(Player::white)->size() == 0);
    CHECK(board.getPlayerCaptures(Player::silver)->size() == 0);
    CHECK(board.getPlayerCaptures(Player::gold)->size() == 0);
}

TEST_CASE("Game Board: Capture pieces and get updated player captures")
{
    // Create an xBoard so there is a possibility of pieces not being on the board
    PositiveXBoard xBoard{{Player::white, Player::black, Player::silver, Player::gold}};

    // Check problem cases
    CHECK(xBoard.capturePiece(1, 1) == false);
    CHECK(xBoard.capturePiece(std::make_pair(-1, 5)) == false);

    // Add 2 pieces to the board
    Piece* piece1 = new Piece{};
    Piece* piece2 = new Piece{std::make_pair(1, 1)};
    CHECK(xBoard.addPiece(piece1));
    CHECK(xBoard.addPiece(piece2));
    piece1->addPlayer(Player::white); // piece1 belongs to white
    CHECK(xBoard.occupiedOnBoard(0, 0));
    CHECK(xBoard.occupiedOnBoard(1, 1));

    // Capture 2 pieces and make sure everything updates properly
    CHECK(xBoard.capturePiece(0, 0));
    CHECK(xBoard.capturePiece(std::make_pair(1, 1)));
    CHECK(xBoard.occupiedOnBoard(0, 0) == false);
    CHECK(xBoard.occupiedOnBoard(1, 1) == false);
    CHECK(xBoard.capturePiece(0, 0) == false);
    CHECK(piece1->getOnBoard() == false);
    CHECK(piece2->getOnBoard() == false);
    // Make sure the player's captures are properly initialized
    REQUIRE(xBoard.getPlayerCaptures(Player::white) != nullptr);
    // piece1 belongs to white so it should not get added to white's captured pieces
    CHECK(xBoard.getPlayerCaptures(Player::white)->size() == 1);
    // Neither piece belongs to black so they both should get added to black's captured pieces
    CHECK(xBoard.getPlayerCaptures(Player::black)->size() == 2);
}

TEST_CASE("Game Board: Get default player score")
{
    // Create a new GameBoard object
    GameBoard board{{Player::white, Player::black, Player::silver, Player::gold}};

    // Make sure the score is 0
    CHECK(board.getPlayerScore(Player::white) == 0);
    CHECK(board.getPlayerScore(Player::black) == 0);
    CHECK(board.getPlayerScore(Player::silver) == 0);
    CHECK(board.getPlayerScore(Player::gold) == 0);
}

TEST_CASE("Game Board: Get updated player score")
{
    // Create a new GameBoard object
    GameBoard board{ { Player::white, Player::black, Player::silver, Player::gold } };
    
    // Create a white piece and a black piece and capture them both
    Piece* whitePiece = new Priority1Piece{std::make_pair(0, 0)};
    Piece* blackPiece = new Priority2Piece{std::make_pair(3, 3)};
    CHECK(board.addPiece(whitePiece));
    CHECK(board.addPiece(blackPiece));
    whitePiece->addPlayer(Player::white);
    blackPiece->addPlayer(Player::black);
    CHECK(board.capturePiece(0, 0));
    CHECK(board.capturePiece(3, 3));

    // Make sure scores updated properly
    CHECK(board.getPlayerScore(Player::white) == 2); // White doesn't gain 5 from whitePiece
    CHECK(board.getPlayerScore(Player::black) == 1); // Black doesn't gain 7 from blackPiece
    CHECK(board.getPlayerScore(Player::silver) == 3); // Silver gains points from both captures
    CHECK(board.getPlayerScore(Player::gold) == 3); // Gold gains points from both captures
}
