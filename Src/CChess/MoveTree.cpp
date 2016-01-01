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
   void MoveTree::clear()
   {
      if( root->childrenCount == 0 )
         return;
      std::list<TreeNode*> leaves;
      std::list<TreeNode*>::iterator lit;
      for( int i = 0; i < root->childrenCount; i++ )
      {
         while( root->children[i]->childrenCount > 0 )
         {
            getLeaves(&leaves, root->children[i]);
            for( lit = leaves.begin(); lit != leaves.end(); ++lit )
            {
               (*lit)->parent->childrenCount = 0;
               (*lit)->parent->children = NULL;
               delete *lit;
            }
         }
         delete root->children[i];
      }

      root->childrenCount = 0;
      root->children = NULL;
   }
}
