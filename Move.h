#ifndef MOVE_H_
#define MOVE_H_

#include "Piece.h"
namespace CChess
{
   struct Move
   {
      Move(int xF, int yF, int xT, int yT)
      {
         xFrom = xF;
         yFrom = yF;
         xTo = xT;
         yTo = yT;
         type = Piece::None;
      }
      Move(int xF, int yF, int xT, int yT, Piece::Type t)
      {
         Move(xF,yF,xT,yT);
         type = t;
      }
      Move()
      {
         Move(-1,-1,-1,-1);
      }

      int xFrom, yFrom; // Move source
      int xTo, yTo;     // Move destination
      Piece::Type type; // In the case of pawn transformation: the desired piece
   };
}



#endif /* MOVE_H_ */
