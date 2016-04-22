#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  int getMax() const;
  void print(Queue <BTreeNode*> &queue);

  bool hasRoom(); //returns true if a value can fit in
  bool lookLeft(int value); //recursively looks to the left nodes and moves items down if room is found.
  bool lookRight(int value); //recursively looks to the right nodes and moves donw
  bool lookForRoom(int value); //calls left then right

  void insertValue(int value); // rearranges the array and places the integer in the right location

  void removeMin(); //removes the min value and rearranges the array
  void removeMax(); //removes the max value from node

  int findInsertionIndex(int value); // find the index of where the
  int findSplitIndex(int index); //find the location of the leftmost value of the right leaf from the split.

  LeafNode* performLeafSplit(int value); // if values can't be moved, starts a leaf split

}; //LeafNode class

#endif
