#ifndef MOVETREE_H_
#define MOVETREE_H_

namespace CChess
{
   struct TreeNode
   {
      TreeNode()
      {
         move = Move();
         children = NULL;
         childrenCount = 0;
         actualScore = 0;
         relativeScore = 0;
         parent = NULL;
      }
      Move move;
      TreeNode** children;
      TreeNode* parent;
      int childrenCount;
      double actualScore;
      double relativeScore;
   };

   struct MoveTree
   {
      void getLeaves(std::list<TreeNode*>* leaves, TreeNode* node = NULL, bool firstCall = true);
      void clear();
      MoveTree()
      {
         root = NULL;
      }
      TreeNode* root;
   };
}


#endif /* MOVETREE_H_ */
