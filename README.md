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

Here is a fully functional simple sample demonstrating a command-line based usage:
```
CChess::ChessBoard board;
int coordinates[4];
while( true )
{
   std::cout << board.getString() << std::endl << "Your turn! Input like 'xFrom,yFrom,xTo,yTo'"
             << std::endl;

   bool invalidInput = true;

   // Acquire input from console ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   while( invalidInput )
   {
      std::string input;
      std::cin >> input;
      std::vector<std::string> values = split(input,',');

      // Verify the user entered 4 values
      if(values.size() != 4)
      {
         std::cout << "Please type 4 values." << std::endl;
         continue;
      }

      // Verify the values are numbers
      bool validCoordinates = true;
      for(int i = 0; i < 4; i++)
         if ( !(std::istringstream(values[i]) >> coordinates[i]) )
         {
            std::cout << values[i] << " is not a valid coordinate." << std::endl;
            validCoordinates = false;
            break;
         }
      if( !validCoordinates )
         continue;

      // Verify the numbers make up a valid move
      std::list<CChess::Move> moves;
      board.computeAvailableMoves(CChess::White, &moves);
      std::list<CChess::Move>::const_iterator mit;
      for( mit = board.moves.begin(); mit != board.moves.end(); ++mit )
      {
         CChess::Move move = *mit;
         if( move.xFrom == coordinates[0] &&
             move.yFrom == coordinates[1] &&
             move.xTo   == coordinates[2] &&
             move.yTo   == coordinates[3])
         {
            invalidInput = false;
            break;
         }
      }
      if(invalidInput)
         std::cout << "The specified move is invalid." << std::endl;
   }

   // The specified move is valid ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   CChess::Move selectedMove =
         CChess::Move(coordinates[0],coordinates[1],coordinates[2],coordinates[3]);
   board.makeMove(selectedMove);
   if(board.state == CChess::Over)
   {
      std::cout << "You won, well done!" << std::endl;
      break;
   }
   if(board.state == CChess::Stalemate)
   {
      std::cout << "Stalemate!" << std::endl;
      break;
   }
   std::cout << board.getString() << std::endl << "My turn, lemme think..." << std::endl;
   board.makeMove(board.computeBestMove(CChess::Black));
   if(board.state == CChess::Over)
   {
      std::cout << board.getString() << std::endl << "You lost..." << std::endl;
      break;
   }
   if(board.state == CChess::Stalemate)
   {
      std::cout << "Stalemate!" << std::endl;
      break;
   }
}
```
