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
   void moveToNextRoom();
   CChess::Player player;
private:
   int clientW;
   int clientH;
   Room** gameRooms;
   int currentRoom;
   int roomCount;
};

#endif /* CHESSGAME_H_ */
