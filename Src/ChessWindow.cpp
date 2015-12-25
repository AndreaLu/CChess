#include "../ChessWindow.h"

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
   int duration;
};
void ChessWindow::start()
{
   int mouseX, mouseY;
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
   std::list<Piece> pieces;
   pieces.push_back(Piece(0,  0  ,&pieceSprites[BRook]));
   pieces.push_back(Piece(100,0  ,&pieceSprites[BKnight]));
   pieces.push_back(Piece(200,0  ,&pieceSprites[BBishop]));
   pieces.push_back(Piece(300,0  ,&pieceSprites[BQueen]));
   pieces.push_back(Piece(400,0  ,&pieceSprites[BKing]));
   pieces.push_back(Piece(500,0  ,&pieceSprites[BBishop]));
   pieces.push_back(Piece(600,0  ,&pieceSprites[BKnight]));
   pieces.push_back(Piece(700,0  ,&pieceSprites[BRook]));
   pieces.push_back(Piece(0,  700,&pieceSprites[WRook]));
   pieces.push_back(Piece(100,700,&pieceSprites[WKnight]));
   pieces.push_back(Piece(200,700,&pieceSprites[WBishop]));
   pieces.push_back(Piece(300,700,&pieceSprites[WQueen]));
   pieces.push_back(Piece(400,700,&pieceSprites[WKing]));
   pieces.push_back(Piece(500,700,&pieceSprites[WBishop]));
   pieces.push_back(Piece(600,700,&pieceSprites[WKnight]));
   pieces.push_back(Piece(700,700,&pieceSprites[WRook]));
   // Create pawns
   for(int i = 0; i < 8; i++)
   {
      pieces.push_back(Piece(i*100,100,&pieceSprites[BPawn]));
      pieces.push_back(Piece(i*100,600,&pieceSprites[WPawn]));
   }

   bool updatesAvailable = true;
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
          // Draw the pieces
          std::list<Piece>::const_iterator it = pieces.begin();
          while(it != pieces.end())
          {
             (*it).sprite->setPosition(sf::Vector2f((*it).x,(*it).y));
             window.draw(*(*it).sprite);
             it++;
          }

          window.display();

          updatesAvailable = false;
       }
   }
}
