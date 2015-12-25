#ifndef PIECE_H_
#define PIECE_H_

namespace CChess
{
   struct Piece
   {
      enum Type {None, Pawn, Knight, Bishop, Rook, Queen, King } type;
      Player owner;
      int getValue()
      {
         switch(type)
         {
         case(Pawn):
               return 1;
         case(Knight):
               return 3;
         case(Bishop):
               return 5;
         case(Rook):
               return 8;
         case(Queen):
               return 9;
         default:
            return 0;
         }
      }
   };
}

#endif /* PIECE_H_ */
