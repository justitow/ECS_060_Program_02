#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
  this->repairRelationships();

  int index = this->findInsertionKeyIndex(value);
  BTreeNode* node = this->children[index]->insert(value);

  if (node != NULL)
  {
    if (!this->hasRoom())
    {
      if(!this->lookForRoom(value, node))
      {
        return performInternalSplit(value, node);
      }
    }
    if (!(this->rightSibling != NULL && (((InternalNode*)this->getRightSibling())->keys[0] == value)))
      this->insertKeyAndChild(node->getMinimum(), node);
  }

  this->repairRelationships();
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node
  this->children[0] = oldRoot;
  this->keys[0] = oldRoot->getMinimum();
  this->children[1] = node2;
  this->keys[1] = node2->getMinimum();
  count = 2;

} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  // students may write this
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

bool InternalNode::hasRoom()
{
  if (this->count == this->internalSize)
    return false;
  return true;
}

bool InternalNode::lookForRoom(int value, BTreeNode* node)
{
  /*
  if (this->lookLeft() || this->lookRight(value, node, 0))
    return true;
  return false;

  */
  
  if (this->leftSibling != NULL && ((InternalNode*)this->getLeftSibling())->hasRoom())
  {
    InternalNode* left = ((InternalNode*)this->getLeftSibling());
    left->insertKeyAndChild(this->keys[0], this->children[0]);
    this->removeMin();
    return true;
  }
  else if(this->rightSibling != NULL && ((InternalNode*)this->getRightSibling())->hasRoom())
  {
    InternalNode* right = ((InternalNode*)this->getRightSibling());
    int max = this->keys[count-1];
    if (value > max)
    {
      right->insertKeyAndChild(value, node);
    }
    else
    {
      right->insertKeyAndChild(this->keys[this->count - 1], this->children[this->count - 1]);
      this->removeMax();
    }
    return true;
  }
  else
  return false;
  
}

bool InternalNode::lookLeft()
{
  InternalNode* left = (InternalNode*)this->getLeftSibling();

  if (left != NULL)
  {
    if (left->hasRoom() || ((!left->hasRoom()) && left->lookLeft()))
    {
      left->insertKeyAndChild(this->keys[0], this->children[0]); // this will need to also insert the corresponding child function
      //oh, this is where I use the one that is inserting from a sibling
      this->removeMin();
      return true;
    }
  }

  return false;
}

bool InternalNode::lookRight(int value, BTreeNode* node, int iter)
{
  InternalNode* right = (InternalNode*)this->getRightSibling();

  if (right != NULL)
  {
    if (right->hasRoom() || ((!right->hasRoom() && right->lookRight(value, node, iter + 1))))
    {
      int max = this->keys[count-1];

      if (value > max && iter == 0)
      {
        right->insertKeyAndChild(value, node);
      }
      else
      {
        right->insertKeyAndChild(this->keys[this->count - 1], this->children[this->count - 1]);
        this->removeMax();
      }
      return true;
    }
  }


  return false;
}

int InternalNode::findInsertionKeyIndex(int value)
{
  int i = 0;

  if (value < this->keys[0])
  {
    return 0;
  }
  while (value > this->keys[i] && i < this->count)
  {
    i++;
  }
  return i-1;
}


int InternalNode::findInsertionIndex(int value)
{
  int i = 0;
  while(value > this->keys[i] && i < this->count)
    i++;
  return i;
}


void InternalNode::insertKeyAndChild(int value, BTreeNode *child)
{
  int index = this->findInsertionIndex(value);

  for (int i = getCount(); i >= index; i--)
  {
    this->keys[i] = this->keys[i-1];
    this->children[i] = this->children[i-1];
  }

  this->keys[index] = value;
  this->children[index] = child;

  count++;
}

InternalNode* InternalNode::performInternalSplit(int value, BTreeNode* node)
{
  this->repairRelationships();

  int index = this->findInsertionIndex(value);

  InternalNode* newNode = new InternalNode(internalSize, leafSize, this->parent, NULL, NULL);
  if (count%2 == 1)
  {
    if (index <= count/2)
    {
      for (int i = count/2; i < this->count; i++)
      {
        newNode->insertKeyAndChild(this->children[i]->getMinimum(), this->children[i]);
      }

      this->count = this->count/2;
      this->insertKeyAndChild(node->getMinimum(), node);
    }
    else
    {
      for (int i = count/2 + 1; i < count; i++)
      {
        newNode->insertKeyAndChild(this->children[i]->getMinimum(), this->children[i]);
      }
      newNode->insertKeyAndChild(node->getMinimum(), node);
      this->count = this->count/2 + 1;
    }
  }
  else
  {
    if (index <= this->count/2-1)
    {
      for (int i = this->count/2-1; i < this->count; i++)
      {
        newNode->insertKeyAndChild(this->children[i]->getMinimum(), this->children[i]);
      }

      this->count = count/2 - 1;
      this->insertKeyAndChild(node->getMinimum(), node);
    }
    else
    {
      for (int i = this->count/2; i < count; i++)
        newNode->insertKeyAndChild(this->children[i]->getMinimum(), this->children[i]);

      newNode->insertKeyAndChild(node->getMinimum(), node);
      this->count = count/2;
    }
  }

  this->repairRelationships();
  newNode->repairRelationships();
  return newNode;
}

void InternalNode::removeMax()
{
  if (this->count > 1)
  {
    this->count -= 1;
  }
}

void InternalNode::removeMin()
{
  for (int i = 0; i < this->getCount()-1; i++)
  {
    this->keys[i] = this->keys[i+1];
    this->children[i] = this->children[i+1];
  }
  this->count -= 1;
}

void InternalNode::repairRelationships()
{

  if (this->count > 1)
  {
    for (int i = 0; i < this->count-1; i++)
    {
      this->keys[i] = this->children[i]->getMinimum();
      this->children[i]->setRightSibling(this->children[i+1]);
      this->children[i+1]->setLeftSibling(this->children[i]);
      this->children[i+1]->setRightSibling(NULL);
    }
  }
  this->children[0]->setLeftSibling(NULL);
  this->children[count-1]->setRightSibling(NULL);
  this->keys[this->count - 1] = this->children[this->count - 1]->getMinimum();

}
/*
int InternalNode::findSplitIndex(int index)
{
  int splitdex;
  if (index < internalSize/2)
    splitdex = internalSize/2 - 1;
  else
    splitdex = internalSize/2;

  if (internalSize%2 == 1)
    splitdex++;

  return splitdex;
}
 */