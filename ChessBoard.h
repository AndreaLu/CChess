#ifndef CHESSBOARD_H
#define CHESSBOARD_H

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
   private:
      // Creates a new ChessBoard with the specified
      // move performed without modifying this
      // particular instance of ChessBoard.
      ChessBoard* simulateMove(Move);
      // Fill the list with the available moves for the specified player
      void computeAvailableMoves(Player);
      std::list<Move> moves;
      // Chessboard
      Piece pieces[8][8];


   };
}

#endif
