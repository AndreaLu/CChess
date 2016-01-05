#include "../GUI.h"
#ifndef NULL
#define NULL 0
#endif


ChessWindow::ChessWindow()
{
   clientW      = 800;
   clientH      = 800;

   currentRoom = new MenuRoom(clientW, clientH, this);

   userPlayer = CChess::White;
}
ChessWindow::~ChessWindow()
{

}

void ChessWindow::start()
{
   sf::RenderWindow window(sf::VideoMode(clientW, clientH), "CChess");
   window.setFramerateLimit(60);

   while (window.isOpen())
   {
      sf::Event event;

      // Acquire controls ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      while (window.pollEvent(event))
      {
         switch(event.type)
         {
            case(sf::Event::Closed):
               window.close();
            break;
            case(sf::Event::MouseMoved):
               Controls::mouseX = event.mouseMove.x;
               Controls::mouseY = event.mouseMove.y;
            break;
            case(sf::Event::MouseButtonPressed):
               if(event.mouseButton.button == sf::Mouse::Left)
                  Controls::leftMousePressed = true;
               else if(event.mouseButton.button == sf::Mouse::Right)
                  Controls::rightMousePressed = true;
            break;
            case(sf::Event::Resized):
                  Controls::windowResized = true;
            break;
            default:
            break;
         }
         if (event.type == sf::Event::Closed)
             window.close();
      }

      // Game Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      window.clear();
      if( currentRoom != NULL)
         currentRoom->loop(window);
      Controls::loop();
      window.display();
   }
}

void ChessWindow::moveToRoom(Room* room)
{
   if( currentRoom != NULL )
      delete currentRoom;
   currentRoom = room;
}
