#include "../ChessWindow.h"
#ifndef NULL
#define NULL 0
#endif
ChessWindow::ChessWindow()
{
   clientW = 800;
   clientH = 800;
}

ChessWindow::~ChessWindow()
{

}
// This class represents the pieces to be drawed
struct Piece
{
   sf::Sprite* sprite;   // Sprite of the piece
   int x,y;              // Position inside the window
   Piece(int x, int y, sf::Sprite* sprite)
   {
      this->sprite = sprite;
      this->x = x;
      this->y = y;
   }
   ~Piece()
   {
   }
};
// This class represents an animation
struct Animation
{
   Piece* piece;     // Pointer to the piece to be animated
   int xFrom, yFrom; // Starting position of the animation
   int xEnd, yEnd;   // Ending position of the animation
   float duration;
   float fullDuration;
   Piece* toKill;
   Animation(Piece* piece, int xFrom, int yFrom, int xEnd, int yEnd, float duration, Piece* toKill)
   {
      this->piece = piece;
      this->xFrom = xFrom;
      this->yFrom = yFrom;
      this->xEnd  = xEnd;
      this->yEnd  = yEnd;
      this->duration = duration;
      this->fullDuration = duration;
      this->toKill = toKill;
   }
};
void ChessWindow::start()
{
   CChess::ChessBoard board;               // Board to play chess
   // Control variables
   int mouseX, mouseY;                     // Cursor position
   bool leftMousePressed;                  // Left button pressed
   bool rightMousePressed;                 // Right button pressed
   int selectedX, selectedY;               // Selected cell position
   bool selected = false;                  // Is there a selected piece
   std::list<CChess::Move> availableMoves; // Moves available to the selected piece
   std::list<CChess::Move>::const_iterator it;
   std::list<Animation*> animations;       // Animations to show
   CChess::Move selectedMove(-1,-1,-1,-1); // Selected move
   bool playerTurn = true;                 // Is this player's turn?

   sf::RenderWindow window(sf::VideoMode(clientW, clientH), "CChess");
   // TODO: handle window creation error
   window.setFramerateLimit(60);

   // Fill a texture with the chessboard background ************************************************
   // **********************************************************************************************
   sf::RenderTexture chessBoard;
   chessBoard.create(clientW,clientH);
   // TODO: handle creation error
   chessBoard.clear();
   sf::RectangleShape rectangle(sf::Vector2f(100.0f,100.0f));
   for(int x = 0; x < 8; x++)
      for(int y = 0; y < 8; y++)
      {
         rectangle.setFillColor( ((x+y) & 1) == 0 ? sf::Color(242,218,78) : sf::Color(156,61,14));
         rectangle.setPosition(sf::Vector2f((float)x*100.0f,(float)y*100.0f));
         chessBoard.draw(rectangle);
      }
   chessBoard.display();
   sf::Sprite background(chessBoard.getTexture());

   // Load pieces texture and sprites **************************************************************
   // **********************************************************************************************
   sf::Texture piecesTexture;
   piecesTexture.loadFromFile("pieces.png");
   piecesTexture.setSmooth(true);
   // TODO: handle loading error
   sf::Sprite pieceSprites[12];
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


   // Instantiate all the pieces *******************************************************************
   // **********************************************************************************************
   std::list<Piece*> pieces;
   pieces.push_back(new Piece(0,  0  ,&pieceSprites[BRook]));
   pieces.push_back(new Piece(100,0  ,&pieceSprites[BKnight]));
   pieces.push_back(new Piece(200,0  ,&pieceSprites[BBishop]));
   pieces.push_back(new Piece(300,0  ,&pieceSprites[BQueen]));
   pieces.push_back(new Piece(400,0  ,&pieceSprites[BKing]));
   pieces.push_back(new Piece(500,0  ,&pieceSprites[BBishop]));
   pieces.push_back(new Piece(600,0  ,&pieceSprites[BKnight]));
   pieces.push_back(new Piece(700,0  ,&pieceSprites[BRook]));
   pieces.push_back(new Piece(0,  700,&pieceSprites[WRook]));
   pieces.push_back(new Piece(100,700,&pieceSprites[WKnight]));
   pieces.push_back(new Piece(200,700,&pieceSprites[WBishop]));
   pieces.push_back(new Piece(300,700,&pieceSprites[WQueen]));
   pieces.push_back(new Piece(400,700,&pieceSprites[WKing]));
   pieces.push_back(new Piece(500,700,&pieceSprites[WBishop]));
   pieces.push_back(new Piece(600,700,&pieceSprites[WKnight]));
   pieces.push_back(new Piece(700,700,&pieceSprites[WRook]));
   // Create pawns
   for(int i = 0; i < 8; i++)
   {
      pieces.push_back(new Piece(i*100,100,&pieceSprites[BPawn]));
      pieces.push_back(new Piece(i*100,600,&pieceSprites[WPawn]));
   }

   bool updatesAvailable = true;
   board.computeAvailableMoves(CChess::White);
   while (window.isOpen())
   {
       sf::Event event;
       // Acquire controls *************************************************************************
       // ******************************************************************************************
       while (window.pollEvent(event))
       {
          switch(event.type)
          {
             case(sf::Event::Closed):
                window.close();
             break;
             case(sf::Event::MouseMoved):
                mouseX = event.mouseMove.x;
                mouseY = event.mouseMove.y;
                updatesAvailable = true;
             break;
             case(sf::Event::MouseButtonPressed):
                if(event.mouseButton.button == sf::Mouse::Left)
                   leftMousePressed = true;
                else if(event.mouseButton.button == sf::Mouse::Right)
                   rightMousePressed = true;
                updatesAvailable = true;
             break;
             case(sf::Event::Resized):
                updatesAvailable = true;
             break;
             default:
             break;
          }
           if (event.type == sf::Event::Closed)
               window.close();
       }

       // Update window ****************************************************************************
       // ******************************************************************************************
       if(updatesAvailable)
       {
          window.clear();

          // Draw the background
          window.draw(background);

          if(animations.size() == 0)
          {
             // Gameplay ***************************************************************************
             // ************************************************************************************
             // It's player's turn
             // Highlight the square under the cursor
             sf::RectangleShape shape;
             int selX = 0, selY = 0;
             while((++selX)*100 < mouseX); selX--;
             while((++selY)*100 < mouseY); selY--;
             shape.setPosition(selX*100.0f,selY*100.0f);
             shape.setSize(sf::Vector2f(100.0f,100.0f));
             shape.setFillColor(sf::Color::Green);
             window.draw(shape);
             if(rightMousePressed)
             {
                selected = false;
                rightMousePressed = false;
             }
             if(leftMousePressed)
             {
                if(!selected)
                {
                   availableMoves.clear();
                   it = board.moves.begin();
                   while(it != board.moves.end())
                   {
                      if((*it).xFrom == selX && (*it).yFrom == selY)
                         availableMoves.push_back(*it);
                      it++;
                   }
                   if(availableMoves.size() > 0)
                   {
                      selected = true;
                      selectedX = selX;
                      selectedY = selY;
                   }
                }
                else
                {
                   // Mouse left button pressed with a piece selected:
                   // if the pointer is over an available move, just move,
                   // otherwise deselect the piece
                   bool moveClicked = false;
                   it = availableMoves.begin();
                   int xFrom, yFrom, xTo, yTo;
                   while(it != availableMoves.end())
                   {
                      if((*it).xTo == selX && (*it).yTo == selY)
                      {
                         moveClicked = true;
                         xFrom = (*it).xFrom * 100;
                         yFrom = (*it).yFrom * 100;
                         xTo   = (*it).xTo   * 100;
                         yTo   = (*it).yTo   * 100;
                         selectedMove = (*it);
                         break;
                      }
                      it++;
                   }
                   if(moveClicked)
                   {
                      Piece* piece, *toKill = NULL;
                      // Find piece that has to be moved
                      std::list<Piece*>::const_iterator pit = pieces.begin();
                      while(pit != pieces.end())
                      {
                         if( (*pit)->x == xFrom && (*pit)->y == yFrom )
                            piece = *pit;
                         if( (*pit)->x == xTo && (*pit)->y == yTo )
                            toKill = *pit;
                         pit++;
                      }
                      // start a new animation
                      // Find eventual piece to kill

                      animations.push_back(
                            new Animation(piece,piece->x,piece->y,selX*100,selY*100,10,toKill));
                      selected = false;
                   }
                   else
                   {
                      // deselect
                      selected = false;
                   }
                }
                leftMousePressed = false;
             }

             // Highlight possible moves & selected piece
             if(selected)
             {
                shape.setPosition(selectedX*100.0f,selectedY*100.0f);
                shape.setFillColor(sf::Color::Yellow);
                window.draw(shape);
                it = availableMoves.begin();
                while(it != availableMoves.end())
                {
                   shape.setFillColor(sf::Color::Magenta);
                   shape.setPosition((*it).xTo*100.0f,(*it).yTo*100.0f);
                   window.draw(shape);
                   it++;
                }
             }
             updatesAvailable = false;
          }
          if( animations.size() > 0 )
          {
              std::list<Animation*>::iterator ait = animations.begin();
              while(ait != animations.end())
              {
                 Animation* anim = *ait;
                 float f = anim->duration/anim->fullDuration;
                 anim->piece->x = (float)anim->xFrom * f + (float)anim->xEnd * (1.0f-f);
                 anim->piece->y = (float)anim->yFrom * f + (float)anim->yEnd * (1.0f-f);
                 anim->duration -= 1.0f;
                 if(anim->duration <= 0.0f)
                 {
                    anim->piece->x = anim->xEnd;
                    anim->piece->y = anim->yEnd;
                    if(anim->toKill != NULL)
                    {
                       pieces.remove(anim->toKill);
                       delete anim->toKill;
                    }
                    // Remove animation from the list
                    animations.clear();
                    // Make the move!
                    board.makeMove(selectedMove);
                    if(playerTurn)
                    {
                       // This is time for the PC to make its move
                       playerTurn = false;
                       selectedMove = board.computeBestMove(CChess::Black);
                       // Add new animation for the move
                       Piece* piece, *toKill = NULL;
                       std::list<Piece*>::const_iterator pit = pieces.begin();
                       while(pit != pieces.end())
                       {
                          Piece* pc = *pit;
                          if(pc->x == selectedMove.xFrom*100 && pc->y == selectedMove.yFrom*100)
                             piece = pc;
                          if(pc->x == selectedMove.xTo*100 && pc->y == selectedMove.yTo*100)
                             toKill = pc;
                          pit++;
                       }
                       animations.push_back(new Animation(piece,piece->x,piece->y,
                             selectedMove.xTo*100, selectedMove.yTo*100, 10, toKill));
                       break;
                    }
                    else
                    {
                       playerTurn = true;
                       board.computeAvailableMoves(CChess::White);
                       break;
                    }
                 }
                 ait++;
              }
          }
          // Draw the pieces
          std::list<Piece*>::const_iterator pit = pieces.begin();
          while(pit != pieces.end())
          {
             (*pit)->sprite->setPosition(sf::Vector2f((*pit)->x,(*pit)->y));
             window.draw(*(*pit)->sprite);
             pit++;
          }

          window.display();
       } // if(updatesAvailable)
   }
}
