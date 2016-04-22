#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  int getMinimum()const;
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);

  bool hasRoom(); //returns true if a value can fit in
  bool lookLeft(); //recursively looks to the left nodes and moves items down if room is found.
  bool lookRight(); //recursively looks to the right nodes and moves donw
  bool lookForRoom(); //calls left then right

  int findInsertionIndex(int value); //finds where the new key and child should be inserted
  int findInsertionKeyIndex(int value); // finds where the new value is going
  //int findSplitIndex(int index); //finds the leftmost values of the internal node to be split

  void repairRelationships(); //damn it mom, a family ski trip does not mean dad doesn't want a divorce any more

  void insertKeyAndChild(int value, BTreeNode* child); //inserts the key and the child into the valid arrays

  void removeMin(); //removes the min value and key
  void removeMax(); //removes the max value and key from node



  InternalNode* performInternalSplit(int value, BTreeNode* node); // if values can't be moved, starts a leaf split
}; // InternalNode class

#endif
