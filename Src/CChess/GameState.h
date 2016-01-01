#ifndef GAMESTATE_H_
#define GAMESTATE_H_


namespace CChess
{
   enum GameState
   {
      Playing,       // Playing
      Stalemate,     // Stalemate
      Over           // There was a winner
   };
}


#endif /* GAMESTATE_H_ */
