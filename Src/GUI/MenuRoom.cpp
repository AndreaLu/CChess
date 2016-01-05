#include "../GUI.h"

MenuRoom::MenuRoom(int clientW, int clientH, ChessWindow* cw)
{

   this->chessWindow = cw;
   this->clientW = clientW;
   this->clientH = clientH;

   int cellW = clientW / 8;
   int cellH = clientH / 8;

   // Draw the title
   sf::Font font;
   font.loadFromFile("font.ttf");
   sf::Text text;
   text.setFont(font);
   text.setString("CChess");
   text.setCharacterSize(40);
   text.setColor(sf::Color::Black);
   text.setStyle(sf::Text::Bold);

   // Load pieces sprite
   sf::Texture piecesTexture;
   piecesTexture.loadFromFile("pieces.png");
   piecesTexture.setSmooth(true);
   sf::Sprite wKnight, bRook;
   wKnight.setTexture(piecesTexture);
   wKnight.setTextureRect(sf::IntRect(300,0,100,100));
   bRook.setTexture(piecesTexture);
   bRook.setTextureRect(sf::IntRect(400,100,100,100));



   // Create a background with a checkboard
   backgroundTexture.create(clientW,clientH);
   backgroundTexture.clear();
   sf::RectangleShape rectangle(sf::Vector2f(cellW,cellH));
   for(int x = 0; x < 8; x++)
   for(int y = 0; y < 8; y++)
   {
      rectangle.setFillColor( ((x+y) & 1) == 0 ? sf::Color(242,218,78) : sf::Color(156,61,14) );
      rectangle.setPosition(x*cellW, y*cellH);
      backgroundTexture.draw(rectangle);
   }
   // Some shadows!!
   int y = 150;
   wKnight.setRotation(-23.0f);
   bRook.setRotation(30.0f);
   wKnight.setColor(sf::Color(0,0,0,180));
   bRook.setColor(sf::Color(0,0,0,180));
   wKnight.setPosition(clientW / 2 - text.getLocalBounds().width / 2 - 100 - 10, y - 30 + 10);
   bRook.setPosition(clientW / 2 + text.getLocalBounds().width / 2 + 10 - 10, y + 18);
   backgroundTexture.draw(wKnight);
   backgroundTexture.draw(bRook);
   // Create a light box containing the title
   sf::RectangleShape box;

   box.setSize(sf::Vector2f(314,80));
   box.setFillColor(sf::Color(0,0,0,180));
   box.setPosition(sf::Vector2f(
         clientW / 2 - 157 - 10,
         y + 10
   ));
   backgroundTexture.draw(box);
   box.setPosition(sf::Vector2f(
         clientW / 2 - 157,
         y
   ));
   box.setFillColor(sf::Color(240,240,240));

   backgroundTexture.draw(box);

   text.setPosition(clientW / 2 - text.getLocalBounds().width / 2 , y + text.getLocalBounds().height / 2 );
   backgroundTexture.draw(text);

   // Draw 2 pieces
   wKnight.setRotation(-23.0f);
   bRook.setRotation(30.0f);
   wKnight.setPosition(clientW / 2 - text.getLocalBounds().width / 2 - 100, y - 30);
   bRook.setPosition(clientW / 2 + text.getLocalBounds().width / 2 + 10, y + 8);
   wKnight.setColor(sf::Color(255,255,255));
   bRook.setColor(sf::Color(255,255,255));
   backgroundTexture.draw(wKnight);
   backgroundTexture.draw(bRook);

   // Draw the text
   text.setString("Press mouse L to play as White, R as Black");
   text.setColor(sf::Color(12,12,12));
   text.setCharacterSize(30);
   text.setPosition(clientW / 2 - text.getGlobalBounds().width / 2, y + 300);
   backgroundTexture.draw(text);

   backgroundSprite = sf::Sprite(backgroundTexture.getTexture());
   backgroundSprite.setScale(sf::Vector2f(1.0f,-1.0f));
   backgroundSprite.setPosition(0,clientH);
}

void MenuRoom::loop(sf::RenderWindow& window)
{
   window.draw(backgroundSprite);

   if( Controls::leftMousePressed )
   {
      chessWindow->userPlayer = CChess::White;
      chessWindow->moveToRoom(new PlayRoom(clientW, clientH, chessWindow));
   }
   if( Controls::rightMousePressed )
   {
      chessWindow->userPlayer = CChess::Black;
      chessWindow->moveToRoom(new PlayRoom(clientW, clientH, chessWindow));
   }
}
