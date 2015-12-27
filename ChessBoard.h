#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

#include "Move.h"
#include "Player.h"
#include "Piece.h"

namespace CChess
{
   class ChessBoard
   {
   public:
      // Constructor
      ChessBoard();
      // Copy constructor
      ChessBoard(ChessBoard*);
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
      int computeScore(Player);
      // Converts the chessboard in the current arrangement
      // to a printable string
      std::string getString();
      // Perform the specified move
      void makeMove(Move);
      // Go back of 1 move in the history
      void unmakeMove();
      // Available moves list
      std::list<Move> moves;
      // Fill moves with the available moves
      // for the specified playe
      void computeAvailableMoves(Player, bool CheckKing = true, std::list<Move>* moves = NULL);
      // Get the piece in a specific cell of the board
      Piece getPiece(int x, int y);

   private:
      // Chessboard
      Piece pieces[8][8];
      // Move history
      std::list<Move> history;
      // For debugging purposes print history to
      // an external file "history.txt"
      void printHistory();
      // Variables needed to unmake a move
      std::list<Piece> eatenPieces;

   };
}

#endif /*CHESSBOARD_H_*/
