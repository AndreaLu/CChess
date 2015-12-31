#ifndef MENUROOM_H_
#define MENUROOM_H_

#include <SFML/Graphics.hpp>
#include "Room.h"
#include "Controls.h"


class MenuRoom : public Room
{
public:
   MenuRoom(int clientW, int clientH);
   void loop(sf:: RenderWindow&);
private:
   int clientW, clientH;
};


#endif /* MENUROOM_H_ */
