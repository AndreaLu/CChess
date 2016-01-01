![](https://github.com/AndreaLu/CChess/blob/master/cchess.png?raw=true)
# CChess
Cross-platform chess game written in C++.

Try and defeat the AI I wrote or go pro and write your own pretty easily.

### CChess as a Library

This project is made up of two main namespaces independent of each other, namely CChess and GUI. The first provides a set of classes which allow to play chess, the latter is based on SFML and handles a GUI (window creation, graphical rendering and user input managing). You can certainly employ CChess for your own purposes, in which case credits are greatly appreciated (a link to this repository is enough).

#### Usage
The usage is extremely simple.

To play chess you only need to instantiate `CChess::ChessBoard` and properly use its methods. Following there is a brief description of the most important ones.
- `void resetBoard()` prepares the board for a new match.
- `void makeMove(Move move)` plays the specified move. `Move` is a struct containing two pairs of coordinates that describe the move: `xFrom,yFrom,xTo,yTo` (0,0 is the top left cell whereas 7,7 is the bottom right).
- `Move computeBestMove(Player player)` returns the best move for the specified player according to the currently implemented AI. `Player` is an enum containing, surprise surprise, `Black` and `White`.
- `std::string getString()` returns a string representing a printable version of the board.

Check out [Src/Main.cpp](https://github.com/AndreaLu/CChess/blob/master/Src/Main.cpp#L19) to see an example of a command-line based usage
