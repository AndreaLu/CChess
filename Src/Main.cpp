#include "../CChess.h"
#include "../ChessWindow.h"
#define GAMEGUI 0 // 1 : GUI, 0 : CONSOLE

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

int main()
{
#if GAMEGUI == 1
   ChessWindow window;
   window.start();
#else
   CChess::ChessBoard board;
   // Console play
   std::string input;
   while( true )
   {
      std::cout << board.getString() << std::endl << "Your turn! input like 'xFrom,yFrom,xTo,yTo'"
                << std::endl;


      bool invalidInput = true;
      int coordinates[] = {0,0,0,0};
      while(invalidInput)
      {
         std::cin  >> input;
         std::vector<std::string> values = split(input,',');
         // Verify the input is valid **************************************************************
         // ****************************************************************************************
         if(values.size() != 4)
         {
            std::cout << "Please type 4 values." << std::endl;
            continue;
         }
         for(int i = 0; i < 4; i++)
         {
            if ( ! (std::istringstream(values[i]) >> coordinates[i]) )
            {
               std::cout << values[i] << " is not a valid coordinate." << std::endl;
               break;
            }
            if(coordinates[i] < 0)
            {
               std::cout << "A coordinate cannot be < 0." << std::endl;
               break;
            }
            if(coordinates[i] > 7)
            {
               std::cout << "A coordinate cannot be > 7." << std::endl;
               break;
            }
            if(i == 3)
               invalidInput = false;
         }
         if(invalidInput)
            continue;
         invalidInput = true;
         board.computeAvailableMoves(CChess::White);
         std::list<CChess::Move>::const_iterator mit = board.moves.begin();
         while(mit != board.moves.end())
         {
            if((*mit).xFrom == coordinates[0] &&
               (*mit).yFrom == coordinates[1] &&
               (*mit).xTo   == coordinates[2] &&
               (*mit).yTo   == coordinates[3])
            {
               invalidInput = false;
               break;
            }
            mit++;
         }
         if(invalidInput)
            std::cout << "The specified move is invalid." << std::endl;
      }
      // The specified move is valid ***************************************************************
      // *******************************************************************************************
      board.makeMove(CChess::Move(coordinates[0],coordinates[1],coordinates[2],coordinates[3]));
      // TODO: detect player's victory
      std::cout << board.getString() << std::endl << "My turn, lemme think..." << std::endl;
      CChess::Move bestMove = board.computeBestMove(CChess::Black);
      board.makeMove(bestMove);
      // TODO: detect computer's victory
   }
#endif
   return 0;
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

