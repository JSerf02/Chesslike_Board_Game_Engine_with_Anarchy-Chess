# Chess-like Board Game Engine
A simple yet powerful C++ Chess-like board game engine that can build games of many types. Scroll down if you're interested in the Anarchy Chess engine attached to this project!

#### Credits
Everything in this engine was thoroughly tested using the DocTest C++ testing suite and all code outside of the testing suite was written by Jacob Serfaty.

## Overview
This engine supports games that are "Chess-like", meaning it supports games that:
- Are turn-based
- Are played on a board with discrete tiles
- Use pieces that inhabit the board with a maximum of one piece per tile
- Only offer pieces a finite number of spaces to move to on a given turn

By meeting all of these criteria, you can take advantage of the powerful tools of this engine to produce your game!
Note that this means that this engine supports games that:
- Have infinite boards
- Have boards that are non-rectangular or disconnected
- Have more than 2 players
- Allow multiple players to control the same piece
- Enforce complex rules for piece movement



## Structure
This engine consists of 5 main modules: Piece, Move, Action, GameBoard, and GameState.

Below is a quick description of each module and a couple of their defining features. To see the full capabilities of these modules, check out each of their respective header files!

### Piece
A piece that is on the board, which stores its current position and which players control it. 

Each specific piece implementation should also override the virtual callback functions `generateMoves()` and `generateAttackingMoves()` that define which spaces the piece can move to and which spaces the piece can attack at a given moment in the game.

### Move
A list of board positions that a piece can move to, along with two lists of Actions. The first list of actions, the Premoves, is performed immediately before the piece moves and the second list, the Postmoves, is performed immediately after the pieces moves.

These objects are returned by the `generateMoves()` and `generateAttackingMoves()` callbacks in the `Piece` class. Note that attacking moves only care about positions and disregard the two lists of Actions.

### Action
An abstract class that defines an interface for manipulating the board before and after moving a piece.

Actions consist of 3 virtual callback functions that users should override:
1. `callAction()`
2. `reverseAction()`
3. `applySymptomaticEffects()`

The `callAction()` callback function performs all of the board manipulations. However, all manipulations done by this function must be "asymptomatic", meaning they are reversible and if something goes wrong, the board state is not permanently altered. This is necessary as it allows the engine to play out a sequence of actions to evaluate a future board state to determine its validity. For example, the engine can "simulate" a chess move and check if it will put the player into check before allowing the player to make that move and permanently alter the board.

The `reverseAction()` callback function undoes the effects of `callAction()` and must also be asymptomatic as a result.

The `applySymptomaticEffects()` callback function applies any additional effects that will permanently alter the board and cannot be undone. This callback function is not called when simulating moves, so it should only be used sparingly, likely to control internal states that only affect future moves.

In most circumstances, actions are managed by `std::shared_ptr` smart pointers. To help make actions and assign them to these smart pointers, the engine defines a `make_action` macro that accepts a pointer of any type that inherits the `Action` class and creates a `std::shared_ptr` containing that action.
- Ex: `std::shared_ptr myAction = make_action(new TestAction());`

### Board
The data structure that stores and manages the pieces. 

The board represents its positions as 2D coordinate pairs that are validated through a virtual `onBoard()` function. Overriding this function will allow you to define your own types of boards however you see fit!

Managing the pieces entails controlling whether each piece is on the board, which positions each piece inhabits, and managing the memory of the pieces. 

The board also contains an associated Captured Pieces structure that keeps track of which pieces were removed from the board and which players these piece removals benefit.

### GameState
A data structure that manages the game by providing a high-level interface for controlling the board, interacting with each piece's moves, and applying a given move's actions.

# Anarchy Chess
To showcase the capabilities of this engine, I have implemented a Chess backend engine that supports many of the Anarchy Chess subreddit's custom moves and custom rules. Many of these moves rely on complex logic that would be challenging to implement in a simpler engine.

The most complex feature of Anarchy Chess that is not present in regular Chess is move priority. Move priority allows moves to group themselves based on their importance and to block other moves that are less important. This allows for certain moves to be forced whenever they are available and also for certain moves to be partially forced, meaning they are required when only simpler moves are available, but they can still be overridden by more important moves.

Below is a list of special moves currently implemented (more coming soon!). Note that some of these are present in regular Chess, but they are still complex and worth mentioning since simpler engines often do not include them.
- Pawn Boosting: A Pawn can move 2 spaces forward on its first turn.
- En Passant (forced, priority 10): A Pawn can capture another Pawn that moved 2 spaces forward if it would have been able to capture it if it only moved one space forward.
- Super Passant (forced, priority 10): Following an En Passant, a Pawn can continue traveling diagonally and capturing enemy pawns that are behind the traveled positions. A Pawn that Super Passants all the way to the end of the board may also Promote and/or Knight Boost (See Pawn Promotion and Knight Boosting).
- Castling: If a King and Rook belonging to the same player have not yet moved, they may move past each other and settle right next to one another at the midpoint between their original positions.
- Pawn Promotion: If a Pawn makes it to the end of the board, it may be promoted into any piece.
- Knook Promotion: A Pawn may promote to a Knook, a mythical piece that moves like a Rook and a Knight.
- Knight Boosting: If a Pawn is promoted to a Knight, it can take an extra move.
- Il Vaticano (partially forced, priority 3): If two Bishops are aligned horizontally or vertically 3 spaces away from each other with 2 Pawns occupying the spaces between them, the Bishops may swap places and capture the Pawns in the process.
- Knooklear Fusion: A Knight may move onto the space of a Rook of the same color. If the Knight does so, it will fuse with the Rook, forming a Knook.

Thanks to the power of the engine, implementing these special moves is relatively easy. Check out the `.cpp` files for each of the Chess piece types to see how these special moves are implemented and how easy it is to add more!

# Release
Actual library releases for the Chesslike Board Game Engine and the Anarchy Chess Engine are coming soon once more special moves are added, once I figure out how to separate the two projects and compile them into libraries, and once I figure out how to get a software license. 

To run the many unit tests, build the project with CMake and run ./build/test/anarchy-chess_tests.exe

Note that the project is currently configured to create binaries and not libraries, so building the project will also produce an executable that runs a simple "Hello World!" program.
