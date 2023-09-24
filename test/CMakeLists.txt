cmake_minimum_required(VERSION 3.2)
project(anarchy-chess_tests)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_LOGIC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/logic)
set(TEST_GAMES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/games)
set(TEST_CHESS_DIR ${TEST_GAMES_DIR}/chess)
set(TEST_CHESS_PIECES_DIR ${TEST_CHESS_DIR}/pieces)

set(TESTING_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/TestBoard.h ${CMAKE_CURRENT_SOURCE_DIR}/TestPieces.h ${CMAKE_CURRENT_SOURCE_DIR}/TestChessHelpers.h)
set(TEST_LOGIC_SOURCE_FILES ${TEST_LOGIC_DIR}/MoveTest.cpp ${TEST_LOGIC_DIR}/PieceTest.cpp 
    ${TEST_LOGIC_DIR}/GameBoardTest.cpp ${TEST_LOGIC_DIR}/GameStateTest.cpp ${TEST_CHESS_DIR}/ChessBoardTest.cpp 
    ${TEST_CHESS_DIR}/ChessGameStateTest.cpp ${TEST_CHESS_DIR}/ChessPieceTest.cpp 
    ${TEST_CHESS_PIECES_DIR}/BishopTest.cpp ${TEST_CHESS_PIECES_DIR}/KingTest.cpp 
    ${TEST_CHESS_PIECES_DIR}/KnightTest.cpp ${TEST_CHESS_PIECES_DIR}/PawnTest.cpp 
    ${TEST_CHESS_PIECES_DIR}/QueenTest.cpp ${TEST_CHESS_PIECES_DIR}/RookTest.cpp
    ${TEST_CHESS_PIECES_DIR}/KnookTest.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build/test)
add_executable(anarchy-chess_tests ${CMAKE_CURRENT_SOURCE_DIR}/doctest.h test.cpp  ${SRC_FILES} ${HEADER_FILES} ${TEST_LOGIC_SOURCE_FILES} ${TESTING_HEADERS})
