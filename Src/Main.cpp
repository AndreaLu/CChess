#include "../CChess.h"
#include <iostream>
#include <vector>
#include <sstream>
using namespace CChess;

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

int main()
{
   ChessBoard board;

   // let's play
   std::string input;
   std::vector<std::string> values;
   while(true)
   {
      std::cout << board.getString() << std::endl
                << "Your turn! Input like 'xfrom,yfrom,xto,yto'" << std::endl;
      std::cin >> input;
      values = split(input,',');
      int xFrom, yFrom, xTo, yTo;
      if ( ! (std::istringstream(values[0]) >> xFrom) ) xFrom = 0;
      if ( ! (std::istringstream(values[1]) >> yFrom) ) yFrom = 0;
      if ( ! (std::istringstream(values[2]) >> xTo) )   xTo = 0;
      if ( ! (std::istringstream(values[3]) >> yTo) )   yTo = 0;
      // TODO: perform checks on input
      board.makeMove(Move(xFrom,yFrom,xTo,yTo));
      // TODO: detect player's victory
      std::cout << board.getString() << "It is my turn, let me think..." << std::endl;
      board.makeMove(board.computeBestMove(Black));
      // TODO: detect computer's victory
   }
   return 0;
}

