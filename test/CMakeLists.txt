cmake_minimum_required(VERSION 3.2)
project(anarchy-chess_tests)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_LOGIC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/logic)
set(TEST_GAMES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/games)

set(TESTING_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/TestBoard.h)
set(TEST_LOGIC_SOURCE_FILES ${TEST_LOGIC_DIR}/MoveTest.cpp ${TEST_LOGIC_DIR}/PieceTest.cpp ${TEST_LOGIC_DIR}/GameBoardTest.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build/test)
add_executable(anarchy-chess_tests ${CMAKE_CURRENT_SOURCE_DIR}/doctest.h test.cpp  ${SRC_FILES} ${HEADER_FILES} ${TEST_LOGIC_SOURCE_FILES} ${TESTING_HEADERS})
