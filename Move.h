#ifndef MOVE_H_
#define MOVE_H_

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
      }
      Move()
      {
         xFrom = yFrom = xTo = yTo = -1;
      }
      int xFrom, yFrom;
      int xTo, yTo;
   };
}



#endif /* MOVE_H_ */
