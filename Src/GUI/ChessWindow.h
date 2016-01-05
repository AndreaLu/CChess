#ifndef CHESSGAME_H_
#define CHESSGAME_H_

// This class handles graphics and controls of the game
class ChessWindow
{
public:
   // Constructor
   ChessWindow();
   // Destructor
   ~ChessWindow();
   void start();
   void moveToRoom(Room*);
   CChess::Player userPlayer;
private:
   int clientW;
   int clientH;
   Room* currentRoom;
};

#endif /* CHESSGAME_H_ */
