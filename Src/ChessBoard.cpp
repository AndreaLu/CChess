#include "../CChess.h"
using namespace CChess;

int abs(int x)
{
   if(x < 0)
      return -x;
   return x;
}
namespace CChess
{
   ChessBoard::ChessBoard()
   {
   #ifndef CCHESS_DEBUG
      resetMatch();
   #else
      for(int x = 0; x < 8; x++)
         for(int y = 0; y < 8; y++)
            pieces[x][y].type = Piece::None;

      pieces[3][4].type = Piece::Knight;
      pieces[3][4].owner = Black;
      pieces[1][4].type = Piece::Pawn;
      pieces[1][4].owner = White;

      pieces[0][0].type = Piece::King;
      pieces[0][0].owner = White;
      pieces[7][0].type = Piece::King;
      pieces[7][0].owner = Black;
   #endif
   }
   ChessBoard::ChessBoard(ChessBoard* cb)
   {
      for(int x = 0; x < 8; x++)
         for(int y = 0; y < 8; y++)
         {
            pieces[x][y].owner = cb->pieces[x][y].owner;
            pieces[x][y].type  = cb->pieces[x][y].type;
         }
   }
   ChessBoard::~ChessBoard()
   {
      moves.clear();
   }
   void ChessBoard::resetMatch()
   {
      // Rearrange the pieces
      for(int i = 0; i < 8; i++)
      {
         // Create first row of pieces
         if( i == 0 || i == 7 )
         {
            pieces[0][i].type = Piece::Rook;
            pieces[1][i].type = Piece::Knight;
            pieces[2][i].type = Piece::Bishop;
            pieces[3][i].type = Piece::Queen;
            pieces[4][i].type = Piece::King;
            pieces[5][i].type = Piece::Bishop;
            pieces[6][i].type = Piece::Knight;
            pieces[7][i].type = Piece::Rook;
         }
         pieces[i][0].owner = Black;
         pieces[i][7].owner = White;

         // Create pawns
         pieces[i][1].type = Piece::Pawn;
         pieces[i][6].type = Piece::Pawn;
         pieces[i][1].owner = Black;
         pieces[i][6].owner = White;

         // Empty the field
         if( i > 1 && i < 6 )
            for( int j = 0; j < 8; j++ )
               pieces[j][i].type = Piece::None;

      }
   }
   std::string ChessBoard::getString()
   {
      std::string ret = "";
      for(int y = 0; y < 8; y++)
      {
         for(int x = 0; x < 8; x++)
         {
            switch(pieces[x][y].type)
            {
               case(Piece::None):
                  ret += "-  ";
                  continue;
               case(Piece::Bishop):
                  ret += "B";
                  ret += (pieces[x][y].owner == White ? "B " : "  ");
                  continue;
               case(Piece::King):
                  ret += "+";
                  ret += (pieces[x][y].owner == White ? "+ " : "  ");
                  continue;
               case(Piece::Knight):
                  ret += "K";
                  ret += (pieces[x][y].owner == White ? "K " : "  ");
                  continue;
               case(Piece::Pawn):
                  ret += "P";
                  ret += (pieces[x][y].owner == White ? "P " : "  ");
                  continue;
               case(Piece::Queen):
                  ret += "Q";
                  ret += (pieces[x][y].owner == White ? "Q " : "  ");
                  continue;
               case(Piece::Rook):
                  ret += "R";
                  ret += (pieces[x][y].owner == White ? "R " : "  ");
                  continue;
            }
         }
         ret += "\n";
      }
      return ret;
   }

   void ChessBoard::computeAvailableMoves(Player p)
   {
      this->moves.clear();

      // Find all pieces owned by p ################################################################
      // ###########################################################################################
      for(int x = 0; x < 8; x ++)
      {
         for(int y = 0; y < 8; y++)
         {
            if( pieces[x][y].owner != p )
               continue;
            Piece::Type type = pieces[x][y].type;
            if( type == Piece::None )
               continue;

            // Pawn Moves **************************************************************************
            // *************************************************************************************
            if( type == Piece::Pawn)
            {
               int destY;
               // Boost start
               if( (p == White && y == 6) || (p == Black && y == 1) )
               {
                  destY = y + (p == White ? -2 : 2);
                  if( pieces[x][destY].type == Piece::None )
                     this->moves.push_back(Move(x,y,x,destY));
               }
               // Normal move
               destY = y + (p == White ? -1 : 1);
               if( pieces[x][destY].type == Piece::None )
                  this->moves.push_back(Move(x,y,x,destY));
               // Eat
               if( x > 0 && pieces[x-1][destY].type != Piece::None
                     && pieces[x-1][destY].owner != p)
                  this->moves.push_back(Move(x,y,x-1,destY));
               if( x < 7 && pieces[x+1][destY].type != Piece::None
                     && pieces[x+1][destY].owner != p)
                  this->moves.push_back(Move(x,y,x+1,destY));
               continue;
            }

            // Bishop Moves ************************************************************************
            // *************************************************************************************
            if( type == Piece::Bishop || type == Piece::Queen )
            {
               int destY, destX, steps;
               int xStep[] = {1,1,-1,-1};
               int yStep[] = {1,-1,-1,1};
               for(int i = 0; i < 4; i++)
               {
                  steps = 0;
                  while(true)
                  {
                     steps++;
                     destX = x + steps * xStep[i];
                     destY = y + steps * yStep[i];
                     if( destX < 0 || destX > 7 || destY < 0 || destY > 7 )
                        break;
                     if( pieces[destX][destY].type == Piece::None )
                     {
                        moves.push_back(Move(x,y,destX,destY));
                        continue;
                     }
                     // destX,destY contains a piece
                     if( pieces[destX][destY].owner != p )
                        moves.push_back(Move(x,y,destX,destY));
                     break;

                  }
               }
               continue;
            }

            // Rook Moves **************************************************************************
            // *************************************************************************************
            if( type == Piece::Rook || type == Piece::Queen )
            {
               int xStep[] = {1,0,-1,0};
               int yStep[] = {0,1,0,-1};
               int destX, destY, steps;

               for(int i = 0; i < 4; i++)
               {
                  steps = 0;
                  while(true)
                  {
                     steps++;
                     destX = x + steps * xStep[i];
                     destY = y + steps * yStep[i];
                     if( destX < 0 || destX > 7 || destY < 0 || destY > 7 )
                        break;
                     if( pieces[destX][destY].type == Piece::None )
                     {
                        moves.push_back(Move(x,y,destX,destY));
                        continue;
                     }
                     // destX,destY contains a piece
                     if( pieces[destX][destY].owner != p )
                        moves.push_back(Move(x,y,destX,destY));
                     break;
                  }
               }
            }

            // Knight Moves ************************************************************************
            // *************************************************************************************
            if( type == Piece::Knight )
            {
               int offsetX[] = {2 , 1,-1,-2, -2,-1,1,2};
               int offsetY[] = {-1,-2,-2,-1, 1 ,2 ,2,1};
               int destX, destY;

               for(int i = 0; i < 8; i++)
               {
                  destX = x + offsetX[i];
                  destY = y + offsetY[i];
                  if( destX < 0 || destX > 7 || destY < 0 || destY > 7)
                     continue;
                  if( pieces[destX][destY].type == Piece::None ||
                     pieces[destX][destY].owner != p )
                     moves.push_back(Move(x,y,destX,destY));
               }
            }

            // King Moves **************************************************************************
            // *************************************************************************************
            if( type == Piece::King )
            {
               int offsetX[] = {1,1,0,-1,-1,-1,0,1};
               int offsetY[] = {0,1,1,1,0,-1,-1,-1};
               int destX, destY;

               for(int i = 0; i < 8; i++)
               {
                  destX = x + offsetX[i];
                  destY = y + offsetY[i];
                  if( destX < 0 || destX > 7 || destY < 0 || destY > 7)
                     continue;
                  if( pieces[destX][destY].type == Piece::None ||
                     pieces[destX][destY].owner != p )
                     moves.push_back(Move(x,y,destX,destY));
               }
            }
         }
      }

      // Remove moves that expose the king to a check ##############################################
      // ###########################################################################################
      // Find the king
      int kingX, kingY;
      for(int x = 0; x < 8; x++)
         for(int y = 0; y < 8; y++)
            if(pieces[x][y].type == Piece::King && pieces[x][y].owner == p)
            {
               kingX = x;
               kingY = y;
            }
      std::list<Move>::iterator it = moves.begin();
      while(it != moves.end())
      {
         Move move = (*it);

         int stepX[] = {1,1,0,-1,-1,-1,0 , 1};
         int stepY[] = {0,1,1,1 ,0 ,-1,-1,-1};
         int destX, destY, steps;
         bool diagonal = false;
         int direction = -1;

         if(move.xFrom == kingX && move.yFrom > kingY)
         {
            direction = 2;
            steps = move.yFrom - kingY;
         }
         else if(move.xFrom == kingX && move.yFrom < kingY)
         {
            direction = 6;
            steps = kingY - move.yFrom;
         }
         else if(move.yFrom == kingY && move.xFrom < kingX)
         {
            direction = 4;
            steps = kingX - move.xFrom;
         }
         else if(move.yFrom == kingY && move.xFrom > kingX)
         {
            direction = 0;
            steps = move.xFrom - kingX;
         }
         else if(abs(move.yFrom - kingY) == abs(move.xFrom - kingX))
         {
            diagonal = true;
            steps = abs(move.yFrom - kingY);
            if(move.xFrom < kingX && move.yFrom < kingY)
               direction = 5;
            else if(move.xFrom < kingX && move.yFrom > kingY)
               direction = 3;
            else if(move.xFrom > kingX && move.yFrom < kingY)
               direction = 7;
            else if(move.xFrom > kingX && move.yFrom > kingY)
               direction = 1;
         }

         if(direction == -1)
         {
            it++;
            continue;
         }
         while(true)
         {
            steps++;
            destX = kingX + stepX[direction] * steps;
            destY = kingY + stepY[direction] * steps;
            if( destX < 0 || destY < 0 || destX > 7 || destY > 7 )
               break;
            Piece piece = pieces[destX][destY];
            if( piece.type == Piece::None )
               continue;
            if( piece.owner == p )
               break;

            if( piece.type == Piece::Queen ||
               ( diagonal  && piece.type == Piece::Bishop ) ||
               ( !diagonal && piece.type == Piece::Rook   )    )
            {
               moves.erase(it);
               break;
            }
         }
         it++;
      }
   }
   Move ChessBoard::computeBestMove(Player p)
   {
      // Fill moves with the available moves for p
      computeAvailableMoves(p);
      // Give a score to each move and return the best one!
      int bestScore = 0, moveScore = 0;
      Move bestMove(-1,-1,-1,-1);
      std::list<Move>::const_iterator it = moves.begin();
      bestScore = simulateMove(*it)->computeScore(p);
      // TODO: this sucks
      while(it != moves.end())
      {
         ChessBoard* moveBoard = simulateMove(*it);
         moveScore = moveBoard->computeScore(p);
         if( moveScore >= bestScore )
         {
            bestScore = moveScore;
            bestMove = *it;
         }
         delete moveBoard;
         it++;
      }
      return bestMove;
   }
   ChessBoard* ChessBoard::simulateMove(Move move)
   {
      ChessBoard* newChessBoard = new ChessBoard(this);
      newChessBoard->makeMove(move);
      return newChessBoard;
   }
   void ChessBoard::makeMove(Move move)
   {
      pieces[move.xTo][move.yTo].type = pieces[move.xFrom][move.yFrom].type;
      pieces[move.xTo][move.yTo].owner = pieces[move.xFrom][move.yFrom].owner;
      pieces[move.xFrom][move.yFrom].type = Piece::None;
   }
   int ChessBoard::computeScore(Player p)
   {
      int movesScore = 0, piecesScore = 0;
      for(int x = 0; x < 8; x++)
         for(int y = 0; y < 8; y++)
            if(pieces[x][y].owner == p)
               piecesScore += pieces[x][y].getValue();
            else
               piecesScore -= pieces[x][y].getValue();

      computeAvailableMoves(p);
      movesScore += moves.size();
      computeAvailableMoves(p == White ? Black : White);
      movesScore -= moves.size();

      int finalScore = movesScore * piecesScore;
      if( movesScore < 0 && piecesScore < 0 )
         finalScore = - finalScore;

      return finalScore;
   }
}
