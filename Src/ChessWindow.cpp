#include "../ChessWindow.h"
#ifndef NULL
#define NULL 0
#endif

#include "../PlayRoom.h"
ChessWindow::ChessWindow()
{
   clientW      = 800;
   clientH      = 800;

   roomCount    = 1;
   gameRooms    = new Room*[roomCount]();
   gameRooms[0] =  new PlayRoom(clientW, clientH, this);
   currentRoom  = 0;
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
      if(currentRoom >= roomCount)
         break;
      this->gameRooms[currentRoom]->loop(window);
      Controls::loop();
      window.display();
   }
}

void ChessWindow::moveToNextRoom()
{
   this->currentRoom++;
}
