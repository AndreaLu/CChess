#include "../CChess.h"
#include "../ChessWindow.h"
#define GAMEGUI 1 // 1 : GUI, 0 : CONSOLE

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
   // Console play, init some variables
   CChess::ChessBoard board;
   std::string input;
   bool invalidInput = true;
   int coordinates[] = {0,0,0,0};

   while( true )
   {
      std::cout << board.getString() << std::endl << "Your turn! input like 'xFrom,yFrom,xTo,yTo'"
                << std::endl;

      while(invalidInput)
      {
         // Acquire input from console
         std::cin >> input;
         std::vector<std::string> values = split(input,',');

         // Verify the input is valid **************************************************************
         // ****************************************************************************************

         // Verify the user typed 4 values separated by 3 commas
         if(values.size() != 4)
         {
            std::cout << "Please type 4 values." << std::endl;
            continue;
         }

         // Verify the values were numbers
         bool validCoordinates = true;
         for(int i = 0; i < 4; i++)
            if ( !(std::istringstream(values[i]) >> coordinates[i]) )
            {
               std::cout << values[i] << " is not a valid coordinate." << std::endl;
               validCoordinates = false;
               break;
            }
         if( !validCoordinates )
            continue;

         // Verify the numbers make up a valid move
         std::list<CChess::Move>::const_iterator mit = board.moves.begin();
         while(mit != board.moves.end())
         {
            CChess::Move move = *mit;
            if( move.xFrom == coordinates[0] &&
                move.yFrom == coordinates[1] &&
                move.xTo   == coordinates[2] &&
                move.yTo   == coordinates[3])
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
      board.makeMove(board.computeBestMove(CChess::Black));
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

