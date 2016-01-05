#include "GUI.h"
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

   // Console play ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   CChess::ChessBoard board;
   while( true )
   {
      CChess::Player user, cpu;
      int coords[4];

      // Ask the user for the party ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      std::string input;
      do
      {
         std::cout << "How do you wanna play? (w/b):";
         std::cin >> input;
      }
      while( input.length() > 1 || (input[0] != 'w' && input[0] != 'b') );
      user = (input[0] == 'w' ? CChess::White : CChess::Black);
      cpu  = (user == CChess::White ? CChess::Black : CChess::White);
      if( user == CChess::Black )
         board.makeMove(board.computeBestMove(cpu));
      while( true )
      {
         std::cout << board.getString() << std::endl << "Your turn!" << std::endl;

         // Get a valid move from the console ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         CChess::Move selectedMove;
         while( true )
         {
            std::cout << "Please input like 'xFrom,yFrom,xTo,yTo'" << std::endl;

            std::cin >> input;
            std::vector<std::string> values = split(input,',');

            // Verify the values are four
            if(values.size() != 4)
               continue;

            // Verify the values are numbers
            bool validCoordinates = true;
            for(int i = 0; i < 4; i++)
               if ( !(std::istringstream(values[i]) >> coords[i]) )
                  validCoordinates = false;
            if( !validCoordinates )
               continue;

            // Verify the numbers make up a valid move
            selectedMove = CChess::Move(coords[0], coords[1], coords[2], coords[3]);
            std::list<CChess::Move> moves;
            board.computeAvailableMoves(user, &moves);
            if( (std::find(moves.begin(), moves.end(), selectedMove) != moves.end()) )
               break;
         }

         // Play the selected move ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         board.makeMove(selectedMove);
         if( board.state == CChess::Over || board.state == CChess::Stalemate )
         {
            std::cout << (board.state == CChess::Over ? "You won!" : "Stalemate!") << std::endl;
            break;
         }

         // AI turn ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         std::cout << board.getString() << std::endl << "My turn, lemme think..." << std::endl;
         board.makeMove(board.computeBestMove(cpu));
         if( board.state == CChess::Over || board.state == CChess::Stalemate )
         {
            std::cout << board.getString() << std::endl
                      << (board.state == CChess::Over ? "You lost!" : "Stalemate!") << std::endl;
            break;
         }
      }
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

