// This function was added to aid the debug of IA algorithms
#include "../GUI.h"
struct GraphicalNode
{
   GraphicalNode(CChess::Move move, GraphicalNode* parent, double score)
   {
      this->move = move;
      this->parent = parent;
      x = 0;
      y = 0;
      this->score = score;
   }
   CChess::Move move;
   GraphicalNode* parent;
   int x,y;
   double score;
};
std::vector<std::list<GraphicalNode*>*> nodes;
void navigateTree(CChess::TreeNode* node, int level = 0, GraphicalNode* parent = NULL);
#define repeat(x) for(int ____i_dm12=0;____i_dm12<x;++____i_dm12)
void printMoveTree(const CChess::MoveTree& tree)
{
   sf::Vertex line[] =
   {
       sf::Vertex(sf::Vector2f(10, 10)),
       sf::Vertex(sf::Vector2f(150, 150))
   };

   sf::RenderTexture renderTexture;
   renderTexture.create(4096*2*2, 80*7);
   renderTexture.clear();
   sf::Font font;
   font.loadFromFile("font.ttf");
   sf::Text text;
   text.setCharacterSize(12);
   text.setColor(sf::Color::White);
   text.setFont(font);
   // for each node in the tree create a correpsonding graphical node
   navigateTree(tree.root);
   int x,y;
   int w = 130;
   int h = 80;
   std::vector<std::list<GraphicalNode*>*>::const_iterator nit = nodes.begin();
   std::list<GraphicalNode*>::const_iterator mit;
   y = 20;
   for( nit = nodes.begin();   nit != nodes.end();   ++nit )
   {
      x = 20;
      // draw at y
      for( mit = (*nit)->begin(); mit != (*nit)->end(); ++mit )
      {
         GraphicalNode* node = *mit;
         // draw at x,y
         node->x = x;
         node->y = y;
         if( node->parent != NULL )
         {
            // draw line from x,y to node->parent->x,y
            line[0] = sf::Vertex(sf::Vector2f(node->parent->x, node->parent->y));
            line[1] = sf::Vertex(sf::Vector2f(x, y));
            renderTexture.draw(line, 2, sf::Lines);
         }
         // draw node->move to x,y
         std::string str;
         char buff[30];
         if( node->move == CChess::Move() )
            sprintf(buff,"Invalid move");
         else
            sprintf(buff,"%i,%i,%i,%i\nScore %f",
               node->move.xFrom,
               node->move.yFrom,
               node->move.xTo,
               node->move.yTo,
               node->score);
         str = buff;
         text.setString(str);
         text.setPosition(x,y);
         renderTexture.draw(text);
         // recalculate x
         x += w;
      }
      // recalculate y
      y += h;
   }
   renderTexture.display();
   renderTexture.getTexture().copyToImage().saveToFile("toto.png");
}

void navigateTree(CChess::TreeNode* node, int level, GraphicalNode* parent)
{
   // Clear out the resources
   if( parent == NULL )
   {
      std::vector<std::list<GraphicalNode*>*>::const_iterator it = nodes.begin();
      while(it != nodes.end())
      {
         std::list<GraphicalNode*>::const_iterator git = (*it)->begin();
         while(git != (*it)->end())
         {
            delete *git;
            git++;
         }
         (*it)->clear();
         it++;
      }
   }
   // Add the node node
   while( nodes.size() < (unsigned int)(level + 1) )
      nodes.push_back(new std::list<GraphicalNode*>());
   std::vector<std::list<GraphicalNode*>*>::iterator it = nodes.begin();
   repeat(level) it++;
   GraphicalNode* newNode = new GraphicalNode(node->move,parent,node->actualScore);
   (*it)->push_back(newNode);
   // Add the children of node
   int N = node->childrenCount;
   for( int i = 0; i < N; i++ )
      navigateTree(node->children[i], level+1, newNode);
}

