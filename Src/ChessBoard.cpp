#include "../CChess.h"
using namespace CChess;
#define CCHESS_DEBUG 0
// TODO: implement the 'castle' move
// TODO: implement pawn's final move
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
   #if CCHESS_DEBUG == 0
      resetMatch();
   #else
      for(int x = 0; x < 8; x++)
         for(int y = 0; y < 8; y++)
            pieces[x][y].type = Piece::None;

      pieces[3][4] = Piece(Piece::Knight, Black);
      pieces[1][4] = Piece(Piece::Queen, White);
      pieces[1][6] = Piece(Piece::Queen, White);
      pieces[7][0] = Piece(Piece::King, Black);
      pieces[0][0] = Piece(Piece::King, White);

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
   void ChessBoard::computeAvailableMoves(Player p, bool checkKing, std::list<Move>* pMoves )
   {
      std::list<Move>& moves = (pMoves == NULL ? this->moves : *pMoves);
      std::list<Move> myMoves;

      // Put into myMoves all the moves normally available to every piece owned by p ***************
      // *******************************************************************************************
      for( int x = 0; x < 8; x++ )
      {
         for( int y = 0; y < 8; y++ )
         {
            Piece::Type type = pieces[x][y].type;
            if( pieces[x][y].owner != p || type == Piece::None )
               continue;

            // Pawn Moves --------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------
            if( type == Piece::Pawn )
            {
               int destY;
               // Boost start
               if( (p == White && y == 6) || (p == Black && y == 1) )
               {
                  destY = y + (p == White ? -2 : 2);
                  if( pieces[x][destY].type == Piece::None )
                     myMoves.push_back(Move(x,y,x,destY));
               }
               // Normal move
               destY = y + (p == White ? -1 : 1);
               if( destY < 0 || destY > 7 )
                  continue;
               if( pieces[x][destY].type == Piece::None )
                  myMoves.push_back(Move(x,y,x,destY));
               // Eat
               if( x > 0 && pieces[x-1][destY].type != Piece::None
                         && pieces[x-1][destY].owner != p )
                  myMoves.push_back(Move(x,y,x-1,destY));
               if( x < 7 && pieces[x+1][destY].type != Piece::None
                         && pieces[x+1][destY].owner != p )
                  myMoves.push_back(Move(x,y,x+1,destY));
               continue;
            }

            // Bishop Moves ------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------
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
                        myMoves.push_back(Move(x,y,destX,destY));
                        continue;
                     }
                     // destX,destY contains a piece
                     if( pieces[destX][destY].owner != p )
                        myMoves.push_back(Move(x,y,destX,destY));
                     break;
                  }
               }
               if( type == Piece::Bishop )
                  continue;
            }

            // Rook Moves --------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------
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
                        myMoves.push_back(Move(x,y,destX,destY));
                        continue;
                     }
                     // destX,destY contains a piece
                     if( pieces[destX][destY].owner != p )
                        myMoves.push_back(Move(x,y,destX,destY));
                     break;
                  }
               }
               continue;
            }

            // Knight Moves ------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------
            if( type == Piece::Knight )
            {
               int offsetX[] = {2 , 1,-1,-2, -2,-1,1,2};
               int offsetY[] = {-1,-2,-2,-1, 1 ,2 ,2,1};
               int destX, destY;

               for(int i = 0; i < 8; i++)
               {
                  destX = x + offsetX[i];
                  destY = y + offsetY[i];
                  if( destX < 0 || destX > 7 || destY < 0 || destY > 7 )
                     continue;
                  if( pieces[destX][destY].type == Piece::None ||
                      pieces[destX][destY].owner != p )
                     myMoves.push_back(Move(x,y,destX,destY));
               }
               continue;
            }

            // King Moves --------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------
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
                     myMoves.push_back(Move(x,y,destX,destY));
               }
            }
         }
      }
      // If no king check has to be made, copy myMoves to moves and return *************************
      // *******************************************************************************************
      if( !checkKing )
      {
         moves.clear();
         std::list<Move>::const_iterator it = myMoves.begin();
         while(it != myMoves.end())
         {
            moves.push_back(*it);
            it++;
         }
         myMoves.clear();
         return;
      }
      // Find the position of the king of player p *************************************************
      // *******************************************************************************************
      int origKingX, origKingY;
      bool kingFound = false;
      for(int x = 0; x < 8; x++)
         for(int y = 0; y < 8; y++)
            if(pieces[x][y].type == Piece::King && pieces[x][y].owner == p)
            {
               origKingX = x;
               origKingY = y;
               kingFound = true;
            }
      if(!kingFound) // This should never happen
         return;

      // Find the moves in myMoves that put the king in check **************************************
      // *******************************************************************************************
      Player enemy = ((p == White) ? Black : White);
      std::list<Move> deletion;
      std::list<Move>::iterator eit;
      std::list<Move>::iterator it;

      for( it = myMoves.begin(); it != myMoves.end(); ++it )
      {
         // For each move in myMoves
         Move move = *it;

         // If the move concerns the king, update its coordinates
         int kingX = origKingX, kingY = origKingY;
         if(pieces[move.xFrom][move.yFrom].type == CChess::Piece::King &&
            pieces[move.xFrom][move.yFrom].owner == p)
         {
            kingX = move.xTo;
            kingY = move.yTo;
         }

         // Make the move
         makeMove(move);

         // Check if the king is in check
         computeAvailableMoves(enemy, false);
         for(eit = moves.begin(); eit != moves.end(); ++eit)
            if((*eit).xTo == kingX && (*eit).yTo == kingY)
            {
               // If it is, mark this move for deletion
               deletion.push_back(move);
               break;
            }
         // Unmake the move
         unmakeMove();
      }

      // Delete these moves from myMoves ***********************************************************
      // *******************************************************************************************
      Move del, cur;
      for(eit = deletion.begin(); eit != deletion.end(); ++eit)
      {
         // del is the move to be deleted
         del = *eit;
         // Search del inside myMoves
         for(it = myMoves.begin(); it != myMoves.end(); ++it)
         {
            cur = *it;
            if(cur.xFrom == del.xFrom &&
               cur.yFrom == del.yFrom &&
               cur.xTo   == del.xTo   &&
               cur.yTo   == del.yTo )
            {
               // Erase del from myMove and exit the for
               myMoves.erase(it);
               break;
            }
         }
      }
      deletion.clear();

      // Copy myMoves to moves *********************************************************************
      // *******************************************************************************************
      moves.clear();
      for(it = myMoves.begin(); it != myMoves.end(); ++it)
         moves.push_back(*it);
      myMoves.clear();
   }
   Move ChessBoard::computeBestMove(Player p)
   {
      // TODO: fix segmentation fault
      // Fill moves with the available moves for p
      computeAvailableMoves(p);

      // Give a score to each move and return the best one!
      std::list<Move>::const_iterator it = moves.begin();
      makeMove(*it);
      int bestScore = computeScore(p);
      Move bestMove = *it;
      unmakeMove();
      it++;
      // TODO: this sucks
      while( it != moves.end() )
      {
         makeMove(*it);
         int moveScore = computeScore(p);
         if( moveScore >= bestScore )
         {
            bestScore = moveScore;
            bestMove = *it;
         }
         unmakeMove();
         it++;
      }
      return bestMove;
   }

   void ChessBoard::makeMove(Move move)
   {
      // Save history
      history.push_back(move);
      eatenPieces.push_back(pieces[move.xTo][move.yTo]);
      // Save the piece to the destination
      pieces[move.xTo][move.yTo] = pieces[move.xFrom][move.yFrom];
      // Remove the piece from the source
      pieces[move.xFrom][move.yFrom].type = Piece::None;
   }
   void ChessBoard::unmakeMove()
   {
      // TODO this won't work with a pawn changed to something else
      // Retrieve the last move and remove it from the history
      Move lastMove = *(--history.end());
      history.pop_back();
      Piece lastEaten = *(--eatenPieces.end());
      eatenPieces.pop_back();
      // Unmake the move
      pieces[lastMove.xFrom][lastMove.yFrom] = pieces[lastMove.xTo][lastMove.yTo];
      pieces[lastMove.xTo  ][lastMove.yTo]   = lastEaten;
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

      std::list<Move> moves;
      computeAvailableMoves(p, false, &moves);
      movesScore += moves.size();
      computeAvailableMoves(p == White ? Black : White, false, &moves);
      movesScore -= moves.size();

      int finalScore = movesScore * piecesScore;
      if( movesScore < 0 && piecesScore < 0 )
         finalScore = - finalScore;

      return finalScore;
   }
   Piece ChessBoard::getPiece(int x, int y)
   {
      return pieces[x][y];
   }
   void ChessBoard::printHistory()
   {
      char buffer[100];
      std::ofstream out("history.txt");
      for(std::list<Move>::const_iterator it = history.begin(); it != history.end(); ++it)
      {
         sprintf( buffer,"%i,%i,%i,%i\n",
               (*it).xFrom,
               (*it).yFrom,
               (*it).xTo,
               (*it).yTo );
         out << buffer;
      }
      out.close();
   }
}
