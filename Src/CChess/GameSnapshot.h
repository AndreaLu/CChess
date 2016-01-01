#ifndef GAMESNAPSHOT_H_
#define GAMESNAPSHOT_H_

namespace CChess
{
   struct GameSnapshot
   {
      struct Event
      {
         enum Type {
            motion,
            capture,
            creation
         } type;
         int srcX, srcY, dstX, dstY;
         Piece piece;
      };
      Piece pieces[8][8];         // Snapshot of the pieces in the chess board
      Move move;                  // Move that links this snapshot to the following one (history)
      std::list<Event*> events;   // List of the events that characterize the move
      GameState state;
   };
}


#endif /* GAMESNAPSHOT_H_ */
