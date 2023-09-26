cmake_minimum_required(VERSION 3.2)
project(anarchy-chess)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall  -O3 -march=native -std=c++20")
set(CMAKE_BUILD_TYPE Debug) # Adds debug symbols and optimizations

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(INCLUDE_LOGIC_DIR ${INCLUDE_DIR}/logic)
set(INCLUDE_ACTIONS_DIR ${INCLUDE_LOGIC_DIR}/actions)
set(INCLUDE_GAMES_DIR ${INCLUDE_DIR}/games)
set(INCLUDE_CHESS_DIR ${INCLUDE_GAMES_DIR}/chess)
set(INCLUDE_CHESS_PIECES_DIR ${INCLUDE_CHESS_DIR}/pieces)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCE_LOGIC_DIR ${SOURCE_DIR}/logic)
set(SOURCE_ACTIONS_DIR ${SOURCE_LOGIC_DIR}/actions)
set(SOURCE_GAMES_DIR ${SOURCE_DIR}/games)
set(SOURCE_CHESS_DIR ${SOURCE_GAMES_DIR}/chess)
set(SOURCE_CHESS_PIECES_DIR ${SOURCE_CHESS_DIR}/pieces)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_GAMES_DIR})
include_directories(${INCLUDE_CHESS_DIR})
include_directories(${INCLUDE_CHESS_PIECES_DIR})
include_directories(${INCLUDE_LOGIC_DIR})
include_directories(${INCLUDE_ACTIONS_DIR})
include_directories(${LIB_DIR})

set(HEADER_FILES ${INCLUDE_LOGIC_DIR}/Piece.h ${INCLUDE_LOGIC_DIR}/GameBoard.h 
    ${INCLUDE_LOGIC_DIR}/Move.h ${INCLUDE_LOGIC_DIR}/GameState.h ${INCLUDE_LOGIC_DIR}/Action.h 
    ${INCLUDE_ACTIONS_DIR}/AddPieceAction.h ${INCLUDE_ACTIONS_DIR}/CapturePieceAction.h 
    ${INCLUDE_ACTIONS_DIR}/MovePieceAction.h ${INCLUDE_ACTIONS_DIR}/RemovePieceAction.h
    ${INCLUDE_ACTIONS_DIR}/TryCapturePieceAction.h ${LIB_DIR}/HashPair.h 
    ${INCLUDE_CHESS_DIR}/ChessBoard.h ${INCLUDE_CHESS_DIR}/ChessGameState.h 
    ${INCLUDE_CHESS_DIR}/ChessPiece.h ${INCLUDE_CHESS_PIECES_DIR}/Bishop.h ${INCLUDE_CHESS_PIECES_DIR}/King.h
    ${INCLUDE_CHESS_PIECES_DIR}/Knight.h ${INCLUDE_CHESS_PIECES_DIR}/Pawn.h ${INCLUDE_CHESS_PIECES_DIR}/Queen.h
    ${INCLUDE_CHESS_PIECES_DIR}/Rook.h ${INCLUDE_CHESS_PIECES_DIR}/Knook.h)
set(SRC_FILES ${SOURCE_LOGIC_DIR}/Move.cpp ${SOURCE_LOGIC_DIR}/Piece.cpp ${SOURCE_LOGIC_DIR}/GameBoard.cpp 
    ${SOURCE_LOGIC_DIR}/GameState.cpp ${SOURCE_ACTIONS_DIR}/AddPieceAction.cpp 
    ${SOURCE_ACTIONS_DIR}/CapturePieceAction.cpp ${SOURCE_ACTIONS_DIR}/MovePieceAction.cpp 
    ${SOURCE_ACTIONS_DIR}/RemovePieceAction.cpp ${SOURCE_ACTIONS_DIR}/TryCapturePieceAction.cpp
    ${SOURCE_CHESS_DIR}/ChessBoard.cpp ${SOURCE_CHESS_DIR}/ChessGameState.cpp 
    ${SOURCE_CHESS_DIR}/ChessPiece.cpp ${SOURCE_CHESS_PIECES_DIR}/Bishop.cpp 
    ${SOURCE_CHESS_PIECES_DIR}/King.cpp ${SOURCE_CHESS_PIECES_DIR}/Knight.cpp 
    ${SOURCE_CHESS_PIECES_DIR}/Pawn.cpp ${SOURCE_CHESS_PIECES_DIR}/Queen.cpp 
    ${SOURCE_CHESS_PIECES_DIR}/Rook.cpp ${SOURCE_CHESS_PIECES_DIR}/Knook.cpp)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(test)
add_subdirectory(src)