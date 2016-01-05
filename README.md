![](https://github.com/AndreaLu/CChess/blob/master/cchess.png?raw=true)
# CChess
Cross-platform chess game written in C++.

Try and defeat the AI I wrote or go pro and write your own pretty easily.

### CChess as a Library

This project is made up of two main namespaces independent of each other, namely CChess and GUI. The first provides a set of classes which allow to play chess, the latter is based on [SFML](http://www.sfml-dev.org/) and manages a Graphical User Interface (window creation, graphical rendering and user input handling). You can certainly employ CChess for your own purposes, in which case credits are greatly appreciated (a link to this repository is enough). To do that, embed [CChess.h](https://github.com/AndreaLu/CChess/blob/master/Src/CChess.h) and the whole folder [CChess](https://github.com/AndreaLu/CChess/tree/master/Src/CChess) in your project.

#### Usage
The usage is extremely simple.

To play chess you only need to instantiate `CChess::ChessBoard` and properly use its methods. Following there is a brief description of the most important ones.
- `void resetBoard()` prepares the board for a new match.
- `void computeAvailableMoves(Player p, std::list<Move>* moves)` fills `moves` with all the moves available to player `p`.
`Player` is an enum containing `Black` and `White`. `Move` is a structure which holds two pair of coordinates that describe the move: `xFrom,yFrom,xTo,yTo` (0,0, is the top left cell whereas 7,7 is the bottom right).
- `Move computeBestMove(Player player)` returns the best move for the specified player according to the currently implemented AI.
- `void makeMove(Move move)` plays the specified move.

Check out [Src/Main.cpp](https://github.com/AndreaLu/CChess/blob/master/Src/Main.cpp#L20) to see an example of a fully functional command-line based usage where the user plays against the CPU (althought creating human vs. human or cpu vs. cpu matches is straightforward).
