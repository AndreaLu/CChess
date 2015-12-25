#ifndef CHESSGAME_H_
#define CHESSGAME_H_

#include <SFML/Graphics.hpp>
#include "CChess.h"
#include "Piece.h"

// This class handles graphics and controls of the game
class ChessWindow
{
public:
   // Constructor
   ChessWindow();
   // Destructor
   ~ChessWindow();
   // Let the game begin!
   void start();
private:
   CChess::ChessBoard board;
   int clientW;
   int clientH;
};

#endif /* CHESSGAME_H_ */
