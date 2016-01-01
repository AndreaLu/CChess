
#ifndef ROOM_H_
#define ROOM_H_

class Room
{
public:
   virtual void loop(sf::RenderWindow&) = 0;
   virtual ~Room() {};
};

#endif /* ROOM_H_ */
