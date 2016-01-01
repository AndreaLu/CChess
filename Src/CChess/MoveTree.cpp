#include "../CChess.h"

namespace CChess
{
   void MoveTree::getLeaves(std::list<TreeNode*>* leaves, TreeNode* node, bool firstCall)
   {
      if( node == NULL )
         node = root;
      if( firstCall )
         leaves->clear();

      for( int i = 0; i < node->childrenCount; i++ )
      {
         if( node->children[i]->childrenCount > 0 )
         {
            getLeaves(leaves, node->children[i], false);
         }
         else
         {
            leaves->push_back(node->children[i]);
         }
      }
   }
}
