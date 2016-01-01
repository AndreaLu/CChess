#ifndef PIECE_H_
#define PIECE_H_
#include "Player.h"
namespace CChess
{
   struct Piece
   {
      enum Type {None, Pawn, Knight, Bishop, Rook, Queen, King } type;
      Player owner;
      bool operator==(const Piece& piece)
      {
         return( owner == piece.owner && type == piece.type );
      }
      Piece()
      {
         type = None;
         owner = Black;
      }
      Piece(Type type, Player owner)
      {
         this->type = type;
         this->owner = owner;
      }
      double getValue()
      {
         switch(type)
         {
         case(Pawn):
               return 1.0;
         case(Knight):
               return 3.0;
         case(Bishop):
               return 5.0;
         case(Rook):
               return 8.0;
         case(Queen):
               return 9.0;
         default:
            return 0.0;
         }
      }
   };
}

#endif /* PIECE_H_ */
