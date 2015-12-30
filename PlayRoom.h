#ifndef PLAYROOM_H_
#define PLAYROOM_H_

#include <SFML/Graphics.hpp>
#include "CChess.h"
#include "Piece.h"
#include "Room.h"
#include "Controls.h"
#include "ChessWindow.h"

// This class represents the pieces to be drawed
struct GPiece
{
   sf::Sprite* sprite;   // Sprite of the piece
   int x,y;              // Position inside the window
   float alpha;          // from 0 to 1
   GPiece(int x, int y, sf::Sprite* sprite)
   {
      this->sprite = sprite;
      this->x = x;
      this->y = y;
      this->alpha = 1.0f;
   }
   ~GPiece()
   {
   }
};
class Animation
{
public:
   virtual void loop() = 0;
   virtual int  getLifetime() = 0;
};

class CaptureAnimation : public Animation
{
public:
   CaptureAnimation(GPiece* piece, std::list<GPiece*>* pieces)
   {
      this->piece = piece;
      duration = 20;
      lifetime = duration;
      this->pieces = pieces;
   }
   void loop()
   {
      if( lifetime > 1 )
      {
         piece->alpha = (float)(lifetime-2) / (float)duration;
         lifetime--;
      }
      if( lifetime == 1 )
      {
         // Delete piece from pieces
         pieces->remove(piece);
         lifetime = 0;
      }
   }
   int getLifetime()
   {
      return lifetime;
   }
private:
   int duration;
   int lifetime;
   GPiece* piece;
   std::list<GPiece*>* pieces;
};

class TranslationAnimation : public Animation
{
public:
   TranslationAnimation(GPiece* piece, int xDest, int yDest)
   {
      duration = 20;
      lifetime = duration;
      this->piece = piece;
      this->xDst = xDest;
      this->yDst = yDest;
      this->xSrc = piece->x;
      this->ySrc = piece->y;
   }
   void loop()
   {
      if( lifetime > 0 )
      {
         lifetime--;
         float lifetime_f = (float)lifetime;
         float duration_f = (float)duration;
         float xSrc_f = xSrc;
         float ySrc_f = ySrc;
         float xDst_f = xDst;
         float yDst_f = yDst;
         piece->x = xSrc_f * (lifetime_f/duration_f) + xDst_f * (duration_f-lifetime_f)/duration_f;
         piece->y = ySrc_f * (lifetime_f/duration_f) + yDst_f * (duration_f-lifetime_f)/duration_f;
      }
   }
   int getLifetime() { return lifetime; }
private:
   int duration; // 2 seconds lifetime
   int lifetime;
   int xSrc, ySrc, xDst, yDst;
   GPiece* piece;
};


class PlayRoom : public Room
{
public:
   PlayRoom(int clientW, int clientH, ChessWindow* cw);
   void loop(sf::RenderWindow&);
private:
   ChessWindow* gameWindow;

   int clientW, clientH;
   sf::Sprite background;
   sf::Texture piecesTexture;
   CChess::ChessBoard board;
   sf::RenderTexture chessBoardTexture;
   std::list<GPiece*> pieces;
   sf::Sprite pieceSprites[12];
   CChess::Player player;// = CChess::White;

   // Stuff needed by loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   std::list<void*> animations;
   int cellW, cellH;
   bool pieceSelected;//= false;
   int selectedPieceX, selectedPieceY;
   std::list<CChess::Move> availableMoves;
   CChess::Move selectedMove;
   bool isMoveSelected;
};
#endif /* PLAYROOM_H_ */
