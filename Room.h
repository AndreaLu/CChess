
#ifndef ROOM_H_
#define ROOM_H_

#include <SFML/Graphics.hpp>

class Room
{
public:
   virtual void loop(sf::RenderWindow&) = 0;
};

#endif /* ROOM_H_ */
