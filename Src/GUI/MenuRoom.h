#ifndef MENUROOM_H_
#define MENUROOM_H_


class MenuRoom : public Room
{
public:
   MenuRoom(int clientW, int clientH, ChessWindow* cw);
   void loop(sf:: RenderWindow&);
private:
   int clientW, clientH;

   sf::RenderTexture backgroundTexture;
   sf::Sprite backgroundSprite;

   ChessWindow* chessWindow;
};


#endif /* MENUROOM_H_ */
