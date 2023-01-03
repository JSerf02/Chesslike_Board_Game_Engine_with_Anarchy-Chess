# CMake generated Testfile for 
# Source directory: C:/Users/jserf/Documents/Anarchy Chess
# Build directory: C:/Users/jserf/Documents/Anarchy Chess/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(anarchy-chess_test-results "C:/Users/jserf/Documents/Anarchy Chess/build/test/anarchy-chess_tests.exe")
set_tests_properties(anarchy-chess_test-results PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/jserf/Documents/Anarchy Chess/CMakeLists.txt;25;add_test;C:/Users/jserf/Documents/Anarchy Chess/CMakeLists.txt;0;")
subdirs("test")
subdirs("src")
