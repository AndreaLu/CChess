#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

#include "Move.h"
#include "Player.h"
#include "Piece.h"
#include "GameSnapshot.h"
#include "GameState.h"
namespace CChess
{
   struct MoveTree
   {
      MoveTree()
      {
         this->move = Move();
         childrenCount = 0;
         children = NULL;
         score = 0;
      }
      MoveTree(Move move)
      {
         this->move = move;
         childrenCount = 0;
         children = NULL;
         score = 0;
      }
      Move move;
      MoveTree** children;
      int childrenCount;
      double score;
   };
   class ChessBoard
   {
   public:
      GameState state;
      Player winner;
      Player turn;
      // Constructor
      ChessBoard();
      // Distructor
      ~ChessBoard();
      // Resets the match
      void resetMatch();
      // Determine the best move available
      // to the specified player. If the player
      // has no move (i.e. the match is over), the
      // invalid move -1,-1,-1,-1 will be returned.
      Move computeBestMove(Player);
      // Determine how good the specified player
      // is in the current arrangement
      double computeScore(Player);
      // Converts the chessboard in the current arrangement
      // to a printable string
      std::string getString();

      std::list<Move> moves;
      // Perform the specified move
      void makeMove(Move, bool checkGameState = false);
      // Fill moves with the available moves
      // for the specified playe
      void computeAvailableMoves(Player, bool CheckKing = true, std::list<Move>* moves = NULL);
      // Get the piece in a specific cell of the board
      Piece getPiece(int x, int y);
      std::list<GameSnapshot*> history;

      void setIntellect(int);
   private:
      Move _computeBestMove(Player,int);
      void computeMoveTree(Player,Player, int level, MoveTree* parent = NULL, MoveTree* root = NULL);
      // Go back of 1 move in the history
      void unmakeMove();
      // Available moves list

      // Chessboard
      Piece pieces[8][8];
      // Move history
      GameSnapshot* createSnapshot();
      void loadSnapshot(GameSnapshot*);
      void clearHistory();

      // For debugging purposes print history to
      // an external file "history.txt"
      void printHistory();
      bool wKingMoved;
      bool bKingMoved;

      unsigned int intellect;
      unsigned int n;
   };

}

#endif /*CHESSBOARD_H_*/
