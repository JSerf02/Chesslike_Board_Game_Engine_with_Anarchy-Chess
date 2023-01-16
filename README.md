# Anarchy-Chess
A simple chess-like board game engine that is made to support the anarchy chess rules and provide a clean interface for the addition of new complex pieces and game modes.

Note: This project is still in the early stages of development and is far from complete. 

## Implemented Features
### Logic
All of the internal logic for the general chess-like board game engine is completely implemented and thoroughly tested. Here is a brief overview of the modules:

#### Move
A list of positions and a callback function that will be called whenever the player moves to one of these positions

#### Piece
A single game piece. Designed as a parent class with two inheretable methods that make it easy to define any complex behavior for how the piece should move and atack.

#### GameBoard
A board containing pieces. Controls the pieces without considering game rules.

#### GameState
Manages the board and all of the pieces. Provides an interface for making moves and determining information necessary for making logical moves (that may be displayed in the visual interface). Considers all game logic and calls on appropriate functions from GameBoard to direct game control.

## Coming soon
### Pieces
Default implementations for all of the Anarchy Chess pieces with all of the Anarchy Chess moves.
Some special moves include:
- En Passant (with chaining)
- En Arriere (also with chaining)
- Il Vaticano
- Knight Boosting 
- Radioactive Beta Decay
- Siberian Swipe
- Many, many more :)

### Simple Command-Line Interface
This whole engine would be useless if you couldn't use it to play Chess! The current plans are to add 2 different command-line interfaces for 2 different gameplay variations.

The first type of game/interface will be for a standard, 2 player game of Anarchy Chess.

The second type of game/interface will be for single player Anarchy Chess puzzles

## Future plans
- A visual UI for Anarchy Chess puzzles
- Release?

## How to use
To run, build the project with CMake and run ./build/src/anarchy-chess_build.exe
- Currently, there is no game to run, as the CLI module is not yet implemented

To run tests, build the project with CMake and run ./build/test/anarchy-chess_tests.exe
