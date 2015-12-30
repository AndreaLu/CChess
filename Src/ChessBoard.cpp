#include "../CChess.h"
using namespace CChess;
#define CCHESS_DEBUG 1
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
      resetMatch();
   }
   ChessBoard::~ChessBoard()
   {
      clearHistory();
   }
   void ChessBoard::resetMatch()
   {
      turn = White;
#if CCHESS_DEBUG == 0
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
#else
      for(int i = 0; i < 8; i++)
      {
         pieces[i][6].type = Piece::Pawn;
         pieces[i][6].owner = White;
      }
      pieces[4][4] = Piece(Piece::Pawn, Black);
      pieces[4][5] = Piece(Piece::Pawn, White);

      pieces[0][0] = Piece(Piece::King, White);
      pieces[7][0] = Piece(Piece::King, Black);
      pieces[5][0] = Piece(Piece::Pawn, White);
      pieces[5][1] = Piece(Piece::Pawn, White);
      pieces[5][2] = Piece(Piece::Pawn, White);
      pieces[6][2] = Piece(Piece::Pawn, White);
      pieces[7][2] = Piece(Piece::Pawn, White);
#endif

      // Clear history data
      clearHistory();

      // Game state
      state = Playing;
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

            // Pawn Moves ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            if( type == Piece::Pawn )
            {
               int destY;
               // Boost start ----------------------------------------------------------------------
               if( (p == White && y == 6) || (p == Black && y == 1) )
               {
                  destY = y + (p == White ? -2 : 2);
                  if( pieces[x][destY].type == Piece::None )
                     myMoves.push_back(Move(x,y,x,destY));
               }

               // Normal move ----------------------------------------------------------------------
               destY = y + (p == White ? -1 : 1);
               if( destY < 0 || destY > 7 )
                  continue;
               if( pieces[x][destY].type == Piece::None )
                  myMoves.push_back(Move(x,y,x,destY));

               // Normal capture -------------------------------------------------------------------
               if( x > 0 && pieces[x-1][destY].type != Piece::None
                         && pieces[x-1][destY].owner != p )
                  myMoves.push_back(Move(x,y,x-1,destY));
               if( x < 7 && pieces[x+1][destY].type != Piece::None
                         && pieces[x+1][destY].owner != p )
                  myMoves.push_back(Move(x,y,x+1,destY));

               // Special move: en-passant capture -------------------------------------------------
               if( history.size() > 0 )
               {
                  GameSnapshot* ps = *(--history.end());
                  Piece piece = ps->pieces[ps->move.xFrom][ps->move.yFrom];
                  if( piece.owner == pieces[x][y].owner || piece.type != Piece::Pawn )
                     continue;
                  // If the previous move was a boost
                  if( abs(ps->move.yTo - ps->move.yFrom) == 2 &&
                  // and the pawn is in the right place
                      abs(ps->move.xFrom - x) == 1 && abs(ps->move.yFrom - y) == 2 )
                  // Then add the en-passant capture move
                     myMoves.push_back(Move(x,y,ps->move.xFrom,(piece.owner == White ? 5 : 2)));
               }
               continue;
            }

            // Bishop Moves ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

            // Rook Moves ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

            // Knight Moves ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

            // King Moves ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
      std::list<Move> mvs;

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

         // Make the move without checking the game state
         makeMove(move);

         // Check if the king is in check
         computeAvailableMoves(enemy, false, &mvs);
         for(eit = mvs.begin(); eit != mvs.end(); ++eit)
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
      // Fill moves with the available moves for p
      computeAvailableMoves(p);

      // Give a score to each move and return the best one!
      std::list<Move>::const_iterator it = moves.begin();
      makeMove(*it);
      int bestScore = computeScore(p);
      Move bestMove = *it;
      unmakeMove();
      it++;
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

   void ChessBoard::makeMove(Move move, bool checkGameState)
   {  // checkGameState is false by default

      // Detect the player that makes the move
      Player p = pieces[move.xFrom][move.yFrom].owner;
      Player opponent = (p == White ? Black : White);


      // Save history ******************************************************************************
      // *******************************************************************************************
      GameSnapshot* gs = createSnapshot();
      gs->move = move;
      history.push_back(gs);

      // Make the move *****************************************************************************
      // *******************************************************************************************
      GameSnapshot::Event* ev;
      // If a piece was captured add the corresponding event
      if( pieces[move.xTo][move.yTo].type != Piece::None )
      {
         ev = new GameSnapshot::Event;
         ev->type = GameSnapshot::Event::capture;
         ev->srcX = move.xTo;
         ev->srcY = move.yTo;
         gs->events.push_back(ev);
      }
      // Save the piece to the destination
      pieces[move.xTo][move.yTo] = pieces[move.xFrom][move.yFrom];
      // Remove the piece from the source
      pieces[move.xFrom][move.yFrom].type = Piece::None;

      // Special move: en-passant capture **********************************************************
      // *******************************************************************************************
      // if the previous move in the history is a boost move enable en-passant capture
      if(history.size() > 1)
      {
         GameSnapshot* ps = *(--(--history.end()));
         Piece piece = ps->pieces[ps->move.xFrom][ps->move.yFrom];
         if( abs(ps->move.yTo - ps->move.yFrom) == 2  && piece.type == Piece::Pawn )
         {
            Player p = piece.owner;
            if( move.xTo == ps->move.xFrom &&
                move.yTo == ps->move.yFrom + (p == White ? -1 : 1) )
            {
               // Eat the pawn
               pieces[ps->move.xTo][ps->move.yTo].type = Piece::None;
               ev = new GameSnapshot::Event;
               ev->type = GameSnapshot::Event::capture;
               ev->srcX = ps->move.xTo;
               ev->srcY = ps->move.yTo;
               gs->events.push_back(ev);
            }
         }
      }

      // Add the motion event
      ev = new GameSnapshot::Event;
      ev->type = GameSnapshot::Event::motion;
      ev->srcX = move.xFrom;
      ev->srcY = move.yFrom;
      ev->dstX = move.xTo;
      ev->dstY = move.yTo;
      gs->events.push_back(ev);



      // Check the state of the game (playing, stalemate, over) ************************************
      // *******************************************************************************************
      if( !checkGameState )
               return;

      turn = opponent;

      // If enemy has no move to make and is in check, it is over!
      std::list<Move> moves;
      std::list<Move>::const_iterator it;
      computeAvailableMoves(opponent, true, &moves);
      if( moves.size() == 0 )
      {
         // Find enemy king position
         bool checkMate = false;
         Piece enemyKing(Piece::King, opponent);
         for(int x = 0; x < 8; x++)
         {
            for(int y = 0; y < 8; y++)
            {
               if( pieces[x][y] == enemyKing )
               {
                  // Check if enemyKing is in check
                  computeAvailableMoves(p, false, &moves);
                  for(it = moves.begin(); it != moves.end(); ++it)
                     if( (*it).xTo == x && (*it).yTo == y )
                     {
                        checkMate = true;
                        break;
                     }
                  if( checkMate )
                     break;
               }
            }
            if( checkMate )
               break;
         }
         if( checkMate )
         {
            state = Over;
            winner = p;
         }
         else
         {
            // no moves but no check
            state = Stalemate;
         }
      }
   }
   void ChessBoard::unmakeMove()
   {
      // Retrieve the last snapshot and remove it from the history
      GameSnapshot* gs = *(--history.end());
      history.pop_back();
      // Unmake the move
      loadSnapshot(gs);
      // Cleanup
      delete gs;
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
         finalScore = -finalScore;

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
      for(std::list<GameSnapshot*>::const_iterator it = history.begin(); it != history.end(); ++it)
      {

         sprintf( buffer,"%i,%i,%i,%i\n",
               (*it)->move.xFrom,
               (*it)->move.yFrom,
               (*it)->move.xTo,
               (*it)->move.yTo );
         out << buffer;
      }
      out.close();
   }
   GameSnapshot* ChessBoard::createSnapshot()
   {
      GameSnapshot* gs = new GameSnapshot();
      for( int x = 0; x < 8; x++ )
         for( int y = 0; y < 8; y++ )
            gs->pieces[x][y] = pieces[x][y];
      return gs;
   }
   void ChessBoard::loadSnapshot(GameSnapshot* gs)
   {
      for( int x = 0; x < 8; x++ )
      for( int y = 0; y < 8; y++ )
         pieces[x][y] = gs->pieces[x][y];
   }

   void ChessBoard::clearHistory()
   {
      GameSnapshot* gs;
      while(history.size() > 0)
      {
         gs = *(--history.end());
         history.pop_back();
         delete gs;
      }
   }
}
