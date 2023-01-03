cmake_minimum_required(VERSION 3.2)

project(anarchy-chess_build)
set(TEST_GAMES_SOURCE_FILES helloworld.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build/src)
add_executable(anarchy-chess_build ${TEST_GAMES_SOURCE_FILES} ${SRC_FILES} ${HEADER_FILES})

# target_link_libraries(anarchy-chess_build PRIVATE piece)