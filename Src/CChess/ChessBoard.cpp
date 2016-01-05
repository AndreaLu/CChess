#include "../CChess.h"
#include "../GUI.h"
using namespace CChess;

int abs(int x)
{
   if(x < 0)
      return -x;
   return x;
}
double abs(double x)
{
   if( x < 0 )
      return -x;
   return x;
}
double pow(double a, int n)
{
   if( n == 0 )
      return 1.0;
   double ret = a;
   while(n-- > 0)
      ret *= a;
   return ret;
}
// each chell has a value ranging from 0 to 1
double clamp(double x, double max)
{
   return x > max ? max : x;
}
double getCellValue(Player p, int x, int y)
{
   double yValue = 0, xValue; // from 0 to 1
   if( p == White )
      yValue = (7.0-(double)y)/7.0;
   else
      yValue = (double)y/7.0;
   double diff = ((double)x-3.5) / 3.5;
   if( diff < 0 ) diff = -diff;
   xValue = 1.0 - diff;
   return (xValue + yValue)/2.0;
}
namespace CChess
{
   ChessBoard::ChessBoard()
   {
      resetMatch();
      // intellect,n pairs:
      // (8,2) is a good one
      intellect = 8;
      n = 2;
   }
   ChessBoard::~ChessBoard()
   {
      clearHistory();
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

      // Clear history data
      clearHistory();

      // Chessboard variables
      state           = Playing;    // Game state (playing, over, stalemate)
      turn            = White;      // Player turn
      wLeftRookMoved  = false;      // These are used to verify the castle move conditions
      bLeftRookMoved  = false;
      wRightRookMoved = false;
      bRightRookMoved = false;
      bKingMoved      = false;
      wKingMoved      = false;
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
   void ChessBoard::computeAvailableMoves(Player p, std::list<Move>* pMoves, bool checkKing)
   {
      // This function makes pMoves (or this->moves in the case pMoves is not specified)
      // contain the available moves to player p
      std::list<Move>& moves = *pMoves;
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

               // Normal move
               destY = y + (p == White ? -1 : 1);
               if( destY < 0 || destY > 7 )
                  continue;
               if( pieces[x][destY].type == Piece::None )
               {
                  if( destY != 0 && destY != 7 )
                     myMoves.push_back(Move(x,y,x,destY));
                  else
                  {
                     // Final move
                     myMoves.push_back(Move(x,y,x,destY,Piece::Queen));
                     myMoves.push_back(Move(x,y,x,destY,Piece::Knight));
                  }
                  // Boost start
                  if( (p == White && y == 6) || (p == Black && y == 1) )
                  {
                     destY = y + (p == White ? -2 : 2);
                     if( pieces[x][destY].type == Piece::None )
                        myMoves.push_back(Move(x,y,x,destY));
                  }
               }
               destY = y + (p == White ? -1 : 1);

               // Normal capture
               if( x > 0 && pieces[x-1][destY].type != Piece::None
                         && pieces[x-1][destY].owner != p )
                  myMoves.push_back(Move(x,y,x-1,destY));
               if( x < 7 && pieces[x+1][destY].type != Piece::None
                         && pieces[x+1][destY].owner != p )
                  myMoves.push_back(Move(x,y,x+1,destY));

               // Special move: en-passant capture
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

               // Special move castle
               bool moved          = (p == White ? wKingMoved      : bKingMoved);
               bool leftRookMoved  = (p == White ? wLeftRookMoved  : bLeftRookMoved);
               bool rightRookMoved = (p == White ? wRightRookMoved : bRightRookMoved);
               if( moved )
                  continue;
               // Right castle
               if( pieces[x+1][y].type == Piece::None &&
                   pieces[x+2][y].type == Piece::None &&
                   pieces[x+3][y].type == Piece::Rook &&
                   !rightRookMoved &&
                   pieces[x+3][y].owner == p )
                  myMoves.push_back(Move(x,y,x+2,y));
               // Left castle
               if( pieces[x-1][y].type == Piece::None &&
                   pieces[x-2][y].type == Piece::None &&
                   pieces[x-3][y].type == Piece::None &&
                   pieces[x-4][y].type == Piece::Rook &&
                   !leftRookMoved &&
                   pieces[x-4][y].owner == p )
                  myMoves.push_back(Move(x,y,x-2,y));
            }
         }
      }

      // If no king check control has to be made, copy myMoves to moves and return *****************
      // *******************************************************************************************
      if( !checkKing )
      {
         moves.clear();
         for(std::list<Move>::const_iterator it = myMoves.begin(); it != myMoves.end(); ++it)
            moves.push_back(*it);
         myMoves.clear();
         return;
      }

      // Find the moves in myMoves that put the king in check **************************************
      // *******************************************************************************************
      // Find the position of the king of player p
      int origKingX = 0, origKingY = 0;
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

      Player opponent = ((p == White) ? Black : White);
      std::list<Move> deletion;
      std::list<Move>::iterator eit;
      std::list<Move>::iterator it;
      std::list<Move> mvs;

      for( it = myMoves.begin(); it != myMoves.end(); ++it )
      {
         // For each move in myMoves
         Move move = *it;

         // If move concerns the king, update its coordinates
         int kingX = origKingX, kingY = origKingY;
         if( pieces[move.xFrom][move.yFrom].type == CChess::Piece::King &&
             pieces[move.xFrom][move.yFrom].owner == p )
         {
            kingX = move.xTo;
            kingY = move.yTo;
         }

         makeMove(move);

         // If the king is in check, mark move for deletion
         computeAvailableMoves(opponent, &mvs, false);
         for( eit = mvs.begin(); eit != mvs.end(); ++eit )
            if( (*eit).xTo == kingX && (*eit).yTo == kingY )
            {
               deletion.push_back(move);
               break;
            }

         unmakeLastMove();
      }

      // Delete these moves from myMoves ***********************************************************
      // *******************************************************************************************
      for(eit = deletion.begin(); eit != deletion.end(); ++eit)
         myMoves.remove(*eit);
      deletion.clear();

      // Now that I removed the moves that make the king in check from myMoves
      // I can check castle move condition
      int kingY = (p == White ? 7 : 0);
      if( pieces[4][kingY] == Piece(Piece::King,p) )
      {
         Move leftCastle(4, kingY, 2, kingY);
         Move rightCastle(4, kingY, 6, kingY);
         Move leftCastleCondition(4, kingY, 3, kingY);
         Move rightCastleCondition(4, kingY, 5, kingY);
         if( (std::find(myMoves.begin(), myMoves.end(), leftCastle) != myMoves.end()) &&
            !(std::find(myMoves.begin(), myMoves.end(), leftCastleCondition) != myMoves.end()))
            myMoves.remove(leftCastle);
         if( (std::find(myMoves.begin(), myMoves.end(), rightCastle) != myMoves.end()) &&
            !(std::find(myMoves.begin(), myMoves.end(), rightCastleCondition) != myMoves.end()))
            myMoves.remove(rightCastle);
      }

      // Copy myMoves to moves *********************************************************************
      // *******************************************************************************************
      moves.clear();
      for(it = myMoves.begin(); it != myMoves.end(); ++it)
         moves.push_back(*it);
      myMoves.clear();
   }
   void ChessBoard::setIntellect(int intellect)
   {
      this->intellect = intellect;
   }
   Move ChessBoard::computeBestMove(Player p)
   {
      MoveTree tree;
      tree.root = new TreeNode();
      computeMoveTree(p, intellect, tree.root);
      printMoveTree(tree);
      if( tree.root->childrenCount == 0 )
         return(Move());
      std::list<TreeNode*> leaves;
      std::list<TreeNode*>::const_iterator it;

      double bestScore = 0, score;
      Move   bestMove;
      tree.getLeaves(&leaves, tree.root->children[0]);
      for( it = leaves.begin(); it != leaves.end(); ++it )
         bestScore += (*it)->relativeScore;
      bestScore /= (double)leaves.size();
      bestMove = tree.root->children[0]->move;

      for( int i = 1; i < tree.root->childrenCount; i++ )
      {
         tree.getLeaves(&leaves, tree.root->children[i]);
         score = 0;
         for( it = leaves.begin(); it != leaves.end(); ++it )
            score += (*it)->relativeScore;
         score /= (double)leaves.size();
         if( score > bestScore )
         {
            bestScore = score;
            bestMove = tree.root->children[i]->move;
         }
      }

      leaves.clear();
      tree.clear();
      return bestMove;
   }
   void ChessBoard::computeMoveTree(Player p, unsigned int level, TreeNode* parent, Player originalPlayer)
   {
      Player opponent = (p == White ? Black : White);
      Player actualOriginalPlayer = (level == intellect ?  p : originalPlayer);
      // originalPlayer is the player who called computeBestMove

      // Fill moves with the available moves for p
      std::list<Move> moves;
      computeAvailableMoves(p, &moves, true);
      int N = moves.size();
      if( level == intellect )
         N = moves.size();
      else N = moves.size() > n ? n : moves.size();
      //N = (moves.size() >= factor*n ? factor*n : moves.size());

      // find the N best moves for player p
      parent->children = new TreeNode*[N];
      parent->childrenCount = N;
      for(int i = 0; i < N; i++)
      {
         // Find the best move in moves
         std::list<Move>::const_iterator it = moves.begin();
         makeMove(*it);
         double bestScore = computeScore(p);
         Move bestMove = *it;
         unmakeLastMove();
         it++;
         while( it != moves.end() )
         {
            makeMove(*it);
            double moveScore = computeScore(p);
            // Change movescore accordingly to checkmate
            if(level == intellect)
            {
               std::list<Move> myMoves;
               std::list<Move> oppMoves;
               // Find the opponent king
               int kingX = 0, kingY = 0;
               for( int x = 0; x < 8; x++ )
               for( int y = 0; y < 8; y++ )
                  if( pieces[x][y] == Piece(Piece::King, opponent) )
                  {
                     kingX = x;
                     kingY = y;
                  }
               // If the opponent king is in check (aka: there is a move I can make to capture it)
               computeAvailableMoves(p, &myMoves, false);
               for(std::list<Move>::iterator mmit = myMoves.begin(); mmit != myMoves.end(); ++mmit)
               {
                  if( (*mmit).xTo == kingX && (*mmit).yTo == kingY )
                  {
                     // And if the opponent has no moves to play
                     computeAvailableMoves(opponent, &oppMoves, true);
                     if( oppMoves.size() == 0 )
                     {
                        // Then *mmit is a winning move
                        moveScore = 999999999999999;
                        break;
                     }
                  }
               }
            }
            if( moveScore >= bestScore )
            {
               bestScore = moveScore;
               bestMove = *it;
            }
            unmakeLastMove();
            it++;
         }

         // Add the move to the tree and remove it from moves
         parent->children[i] = new TreeNode();
         parent->children[i]->parent = parent;
         parent->children[i]->move = bestMove;
         // recalculate bestScore
         double score;
         if( p != actualOriginalPlayer )
         {
            makeMove(bestMove);
            score = computeScore( actualOriginalPlayer );
            unmakeLastMove();
         }
         else
            score = bestScore;
         parent->children[i]->actualScore = score;
         double temp = score - parent->actualScore;
         parent->children[i]->relativeScore = parent->relativeScore + temp;
         moves.remove(bestMove);

         if( level == 0 )
            continue;

         // Continue building the tree
         makeMove(bestMove);
         computeMoveTree(opponent, level-1, parent->children[i], actualOriginalPlayer);
         unmakeLastMove();
      }
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

      Piece mover = pieces[move.xFrom][move.yFrom];
      pieces[move.xTo][move.yTo] = mover;
      pieces[move.xFrom][move.yFrom].type = Piece::None;

      // Special move: en-passant capture **********************************************************
      // *******************************************************************************************
      // if the previous move in the history is a boost move enable en-passant capture
      if( history.size() > 1 )
      {
         GameSnapshot* ps = *(--(--history.end()));
         // Find the piece which moved in the previous move
         Piece previousMover = ps->pieces[ps->move.xFrom][ps->move.yFrom];
         if( abs(ps->move.yTo - ps->move.yFrom) == 2  && previousMover.type == Piece::Pawn )
         {
            Player p = previousMover.owner;
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

      // Special move: castle **********************************************************************
      // *******************************************************************************************
      if( mover.type == Piece::King )
      {
         ev = new GameSnapshot::Event;
         ev->type = GameSnapshot::Event::motion;
         // Right castle
         if( move.xTo == move.xFrom + 2 )
         {
            pieces[move.xTo-1][move.yTo] = pieces[7][move.yTo];
            pieces[7][move.yTo].type = Piece::None;
            ev->srcX = 7;
            ev->srcY = move.yTo;
            ev->dstX = move.xTo-1;
            ev->dstY = move.yTo;
         }
         // Left castle
         if( move.xTo == move.xFrom - 2 )
         {
            pieces[move.xTo+1][move.yTo] = pieces[0][move.yTo];
            pieces[0][move.yTo].type = Piece::None;
            ev->srcX = 0;
            ev->srcY = move.yTo;
            ev->dstX = move.xTo+1;
            ev->dstY = move.yTo;
         }
         gs->events.push_back(ev);
      }

      // Add the motion event
      ev = new GameSnapshot::Event;
      ev->type = GameSnapshot::Event::motion;
      ev->srcX = move.xFrom;
      ev->srcY = move.yFrom;
      ev->dstX = move.xTo;
      ev->dstY = move.yTo;
      gs->events.push_back(ev);

      // Special move: pawn final move *************************************************************
      // *******************************************************************************************
      if( move.type != Piece::None )
      {
         pieces[move.xTo][move.yTo] = Piece(move.type, p);
         ev = new GameSnapshot::Event;
         ev->type = GameSnapshot::Event::capture;
         ev->srcX = move.xFrom;
         ev->srcY = move.yFrom;
         gs->events.push_back(ev);
         ev = new GameSnapshot::Event;
         ev->type = GameSnapshot::Event::creation;
         ev->piece = Piece(move.type, p);
         ev->srcX = move.xTo;
         ev->srcY = move.yTo;
         gs->events.push_back(ev);
      }

      if( !checkGameState )
         return;

      // Check the state of the game (playing, stalemate, over) ************************************
      // *******************************************************************************************
      if( mover.type == Piece::King )
      {
         if( mover.owner == White )
            wKingMoved = true;
         else
            bKingMoved = true;
      }
      if( mover.type == Piece::Rook )
      {
         if( mover.owner == White )
         {
            if( move.xFrom == 0 )
               wLeftRookMoved  = true;
            else
               wRightRookMoved = true;
         }
         else
         {
            if( move.xFrom == 0 )
               bLeftRookMoved  = true;
            else
               bRightRookMoved = true;
         }
      }

      turn = opponent;

      // If enemy has no move to make and is in check, it is over!
      std::list<Move> moves;
      std::list<Move>::const_iterator it;
      computeAvailableMoves(opponent, &moves, true);
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
                  computeAvailableMoves(p, &moves, false);
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
   void ChessBoard::unmakeLastMove()
   {
      // Retrieve the last snapshot and remove it from the history
      GameSnapshot* gs = *(--history.end());
      history.pop_back();
      // Unmake the move
      loadSnapshot(gs);
      // Cleanup
      delete gs;
   }
   // Computes the score of the current arrangement with respect to player p
   double ChessBoard::computeScore(Player p)
   {
      double score = 0;
      for( int x = 0; x < 8; x++ )
      for( int y = 0; y < 8; y++ )
      {
         Piece& piece = pieces[x][y];
         if( piece.type == Piece::None )
            continue;
         double factor = (piece.owner == p ? 1.0 : -1.0);
         score += factor * piece.getValue();
      }
      return score;
   }
   Piece ChessBoard::getPiece(int x, int y)
   {
      return pieces[x][y];
   }
   void ChessBoard::saveHistory(const char* filename)
   {
      char buffer[100];
      int cnt = 0;
      std::ofstream out(filename, std::ios_base::out);
      for( std::list<GameSnapshot*>::iterator it = history.begin(); it != history.end(); ++it )
      {
         sprintf( buffer,"%i %i %i %i",
               (*it)->move.xFrom,
               (*it)->move.yFrom,
               (*it)->move.xTo,
               (*it)->move.yTo );
         if( cnt++ == 0 )
            out << buffer;
         else
            out << '\n' << buffer;
      }
      out.close();
   }
   void ChessBoard::loadHistory(const char* filename)
   {
      std::ifstream infile(filename);
      std::string line;
      Move move;
      std::list<Move> moves;

      // each line of the history should be in the form of xfrom yfrom xto yto
      // e.g.: 3 6 3 4
      resetMatch();
      // Read next move from file
      while( infile >> move.xFrom >> move.yFrom >> move.xTo >> move.yTo )
      {
         // Put the available moves in moves
         computeAvailableMoves(turn, &moves, true);
         // If move is containde within moves
         if( (std::find(moves.begin(), moves.end(), move) != moves.end()) )
         {
            // Play the specified move
            makeMove(move, true);
            // If the match is over or in stalemate exit
            if( state != Playing )
               break;
         }
      }
      infile.close();
   }
   // Creates a complete snapshot of the current game
   GameSnapshot* ChessBoard::createSnapshot()
   {
      GameSnapshot* gs = new GameSnapshot();
      gs->state = state;
      gs->turn = turn;
      gs->winner = winner;
      gs->wKingMoved = wKingMoved;
      gs->bKingMoved = bKingMoved;
      gs->wLeftRookMoved = wLeftRookMoved;
      gs->wRightRookMoved = wRightRookMoved;
      gs->bLeftRookMoved = bLeftRookMoved;
      gs->bRightRookMoved = bRightRookMoved;
      for( int x = 0; x < 8; x++ )
      for( int y = 0; y < 8; y++ )
         gs->pieces[x][y] = pieces[x][y];
      return gs;
   }
   // Restore the current game to the specified snapshot
   void ChessBoard::loadSnapshot(GameSnapshot* gs)
   {
      state  = gs->state;
      turn   = gs->turn;
      winner = gs->winner;
      wKingMoved = gs->wKingMoved;
      bKingMoved = gs->bKingMoved;
      wLeftRookMoved = gs->wLeftRookMoved;
      wRightRookMoved = gs->wRightRookMoved;
      bLeftRookMoved = gs->bLeftRookMoved;
      bRightRookMoved = gs->bRightRookMoved;
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
