cmake_minimum_required(VERSION 3.2)
project(anarchy-chess)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall  -O3 -march=native -std=c++20")
set(CMAKE_BUILD_TYPE Debug) # Adds debug symbols and optimizations

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(INCLUDE_LOGIC_DIR ${INCLUDE_DIR}/logic)
set(INCLUDE_GAMES_DIR ${INCLUDE_DIR}/games)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCE_LOGIC_DIR ${SOURCE_DIR}/logic)
set(SOURCE_GAMES_DIR ${SOURCE_DIR}/games)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_GAMES_DIR})
include_directories(${INCLUDE_LOGIC_DIR})

set(HEADER_FILES ${INCLUDE_LOGIC_DIR}/Piece.h ${INCLUDE_LOGIC_DIR}/GameBoard.h 
    ${INCLUDE_LOGIC_DIR}/Move.h)
set(SRC_FILES ${SOURCE_LOGIC_DIR}/Move.cpp ${SOURCE_LOGIC_DIR}/Piece.cpp ${SOURCE_LOGIC_DIR}/GameBoard.cpp)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(test)
add_subdirectory(src)