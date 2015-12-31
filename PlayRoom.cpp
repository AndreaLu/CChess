#include "PlayRoom.h"

float iY(float y)
{
   return(7.0f - y);
}
PlayRoom::PlayRoom(int clientW, int clientH, ChessWindow* win)
{
   selectedPieceX = 0;
   selectedPieceY = 0;

   player = CChess::White;
   pieceSelected = false;

   if( player == CChess::White )
   {
      isMoveSelected = false;
   }
   else
   {
      isMoveSelected = true;
      selectedMove = board.computeBestMove(CChess::White);
   }
   gameWindow = win;

   this->clientW = clientW;
   this->clientH = clientH;
   cellW = clientW / 8;
   cellH = clientH / 8;

   // Fill a texture with the chessboard background ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   chessBoardTexture.create(clientW, clientH);
   // TODO: handle creation error
   chessBoardTexture.clear();
   sf::RectangleShape rectangle(sf::Vector2f(cellW,cellH));
   for(int x = 0; x < 8; x++)
      for(int y = 0; y < 8; y++)
      {
         rectangle.setFillColor( ((x+y) & 1) == 0 ? sf::Color(242,218,78) : sf::Color(156,61,14) );
         rectangle.setPosition(x*cellW, y*cellH);
         chessBoardTexture.draw(rectangle);
      }
   chessBoardTexture.display();
   background = sf::Sprite(chessBoardTexture.getTexture());

   // Load pieces texture and sprites ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   piecesTexture.loadFromFile("pieces.png");
   piecesTexture.setSmooth(true);
   // TODO: handle loading error
   int i = 0;
   for(int y = 0; y < 2; y++)
      for(int x = 0; x < 6; x++)
      {
         pieceSprites[i].setTexture(piecesTexture);
         pieceSprites[i++].setTextureRect(sf::IntRect(x*100,y*100,100,100));
      }
   enum PieceType
   {
      WKing = 0, WQueen, WBishop, WKnight, WRook, WPawn,
      BKing,     BQueen, BBishop, BKnight, BRook, BPawn
   };

   // Instantiate all the pieces ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   for( int x = 0; x < 8; x++ )
      for( int y = 0; y < 8; y++ )
      {
         CChess::Piece piece = board.getPiece(x,y);
         CChess::Piece::Type tp = piece.type;
         if(tp == CChess::Piece::None)
            continue;
         PieceType type;
         type = piece.owner == CChess::White ?
                (tp == CChess::Piece::Bishop ? WBishop : (tp == CChess::Piece::King ? WKing : (
                 tp == CChess::Piece::Knight ? WKnight : (tp == CChess::Piece::Pawn ? WPawn : (
                 tp == CChess::Piece::Queen ? WQueen : WRook))))) :
                (tp == CChess::Piece::Bishop ? BBishop : (tp == CChess::Piece::King ? BKing : (
                 tp == CChess::Piece::Knight ? BKnight : (tp == CChess::Piece::Pawn ? BPawn : (
                 tp == CChess::Piece::Queen ? BQueen : BRook)))));
         GPiece* newPiece = new GPiece(x*100, y*100, &pieceSprites[type]);
         pieces.push_back(newPiece);
      }
}

void PlayRoom::loop(sf::RenderWindow& window)
{
   if( Controls::windowResized )
   {
      // TODO: adjust clientW,clientH,cellW,cellH
   }

   window.draw(background);

   // If an event is occurring, show it, otherwise accept user input
   // Show animations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   if( animations.size() > 0 )
   {
      Animation* anim = (Animation*)(*animations.begin());
      if( anim->getLifetime() > 0 )
         anim->loop();
      else
      {
         animations.pop_front();
         // TODO: handle victory (player or pc) or stalemate
         if( animations.size() == 0 && board.turn != this->player )
         {
            // Time for the pc to make its move
            isMoveSelected = true;
            selectedMove =
                  board.computeBestMove( player == CChess::White ? CChess::Black : CChess::White );
         }
      }
   }
   // Make a move ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   else if( isMoveSelected )
   {
      // User made his move checking the state of the game after it
      board.makeMove(selectedMove, true);
      // add the animations
      std::list<CChess::GameSnapshot::Event*>& events = (*(--board.history.end()))->events;
      GPiece* piece;
      for(std::list<CChess::GameSnapshot::Event*>::iterator it = events.begin();
          it != events.end(); ++it)
      {
         CChess::GameSnapshot::Event* ev = *it;
         if( ev->type == CChess::GameSnapshot::Event::capture ||
             ev->type == CChess::GameSnapshot::Event::motion )
         {
            // Find the piece that has to be captured
            piece = NULL;
            for( std::list<GPiece*>::iterator pit = pieces.begin(); pit != pieces.end();
                 ++pit )
            {
               if( (*pit)->x == ev->srcX * 100 && (*pit)->y == ev->srcY * 100 )
               {
                  piece = (*pit);
                  break;
               }
            }
            if( piece == NULL)
               continue;
            if( ev->type == CChess::GameSnapshot::Event::capture )
               animations.push_back( (Animation*)(new CaptureAnimation(piece, &pieces)) );
            else
               animations.push_front( (Animation*)(
                     new TranslationAnimation(piece,ev->dstX * cellW,ev->dstY * cellH)));
         }
      }
      isMoveSelected = false;
   }
   else
   {
      sf::RectangleShape cell(sf::Vector2f(cellW,cellH));

      // Highlight available move cells ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if( pieceSelected )
      {
         cell.setFillColor(sf::Color::Yellow);
         for( std::list<CChess::Move>::iterator it = availableMoves.begin();
                                                it != availableMoves.end();
                                              ++it )
         {
            CChess::Move move = *it;
            if( move.xFrom == selectedPieceX && move.yFrom == selectedPieceY )
            {
               cell.setPosition(
                     move.xTo * cellW,
                     (player == CChess::White ? move.yTo : iY(move.yTo)) * cellH
               );
               window.draw(cell);
            }
         }
      }

      // Highlight the cell under the cursor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      int selX = 0, selY = 0;
      while( selX * cellW < Controls::mouseX ) selX++; selX--;
      while( selY * cellH < Controls::mouseY ) selY++; selY--;
      cell.setPosition(selX * cellW, selY * cellH);
      cell.setFillColor(sf::Color::Green);
      window.draw(cell);

      // User input handling ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if( Controls::leftMousePressed )
      {
         if( player != CChess::White ) selY = iY( selY );
         // Piece selection
         if( !pieceSelected )
         {
            bool selectionValid = false;
            board.computeAvailableMoves(player, true, &availableMoves);
            for( std::list<CChess::Move>::iterator it = availableMoves.begin();
                                                   it != availableMoves.end();
                                                   ++it )
            {
               CChess::Move move = *it;
               if( move.xFrom == selX && move.yFrom == selY )
               {
                  selectionValid = true;
                  break;
               }
            }
            if( selectionValid )
            {
               pieceSelected = true;
               selectedPieceX = selX;
               selectedPieceY = selY;
            }
         }
         // Move selection or piece deselection
         else
         {
            for( std::list<CChess::Move>::iterator it = availableMoves.begin();
                                                   it != availableMoves.end();
                                                   ++it )
            {
               CChess::Move move = *it;
               if( move.xFrom == selectedPieceX && move.yFrom == selectedPieceY &&
                   move.xTo == selX && move.yTo == selY )
               {
                  isMoveSelected = true;
                  selectedMove   = move;
                  break;
               }
            }
            pieceSelected = false;
         }
      }
   }

   // Draw the pieces ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   for( std::list<GPiece*>::const_iterator it = pieces.begin(); it != pieces.end(); ++it )
   {
      GPiece* piece = *it;
      piece->sprite->setPosition(sf::Vector2f(piece->x,
            player == CChess::White ? piece->y : iY((float)piece->y / (float)cellH) * (float)cellH));
      piece->sprite->setColor(sf::Color(255,255,255,piece->alpha*255));
      window.draw(*piece->sprite);
   }
}
