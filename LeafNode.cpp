#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()

int LeafNode::getMax() const
{
  if (count > 0)
    return values[count-1];
  else
    return 0;
}


LeafNode* LeafNode::insert(int value)
{
  // students must write this

  if (!this->hasRoom())
  {
    if(!this->lookForRoom(value))
    {
      return performLeafSplit(value);
    }
  }

  insertValue(value);

  return NULL;
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

bool LeafNode::hasRoom()
{
  if (this->count == this->leafSize)
    return false;
  return true;
}

bool LeafNode::lookForRoom(int value)
{
  if (this->lookLeft(value) || this->lookRight(value))
    return true;
  else
    return false;

}

bool LeafNode::lookLeft(int value)
{
  LeafNode* left = (LeafNode*)this->getLeftSibling();

  if (left != NULL)
  {
    if (left->hasRoom() || ((!left->hasRoom()) && left->lookLeft(value)))
    {
      left->insertValue(this->getMinimum());
      this->removeMin();
      return true;
    }
  }

  return false;
}

bool LeafNode::lookRight(int value)
{
  LeafNode* right = (LeafNode*)this->getRightSibling();

  if (right != NULL)
  {
    if (right->hasRoom() || ((!right->hasRoom() && right->lookRight(value))))
    {
      right->insertValue(this->getMax());
      this->removeMax();
      return true;
    }
  }

  return false;
}

void LeafNode::removeMax()
{
  if (count > 1)
  {
    count -= 1;
  }
}

int LeafNode::findSplitIndex(int index)
{
  int splitdex;
  if (index < leafSize/2)
    splitdex = leafSize/2 - 1;
  else
    splitdex = leafSize/2;

  if (leafSize%2 == 1)
    splitdex++;

  return splitdex;
}

LeafNode* LeafNode::performLeafSplit(int value)
{
  int index = this->findInsertionIndex(value);

  LeafNode* newNode = new LeafNode(leafSize, this->parent, NULL, NULL);
  if (count%2 == 1)
  {
    if (index <= count/2)
    {
      for (int i = count/2; i < this->count; i++)
      {
        newNode->insertValue(this->values[i]);
      }

      this->count = this->count/2 ;
      this->insertValue(value);
    }
    else
    {
      for (int i = count/2 + 1; i < count; i++)
      {
        newNode->insert(this->values[i]);
      }
      newNode->insertValue(value);
      this->count = this->count/2 + 1;
    }
  }
  else
  {
    if (index <= this->count/2-1)
    {
      for (int i = this->count/2-1; i < this->count; i++)
      {
        newNode->insertValue(this->values[i]);
      }

      this->count = count/2 - 1;
      this->insertValue(value);
    }
    else
    {
      for (int i = this->count/2; i < count; i++)
        newNode->insertValue(this->values[i]);

      newNode->insertValue(value);
      this->count = count/2;
    }
  }


  return newNode;
}

int LeafNode::findInsertionIndex(int value)
{
  int i = 0;
  while (value > this->values[i] && i < this->count)
    i++;
  return i;
}

void LeafNode::removeMin()
{
  for (int i = 0; i < this->getCount()-1; i++)
  {
    this->values[i] = this->values[i+1];
  }
  count -= 1;
}

void LeafNode::insertValue(int value)
{
  int index = this->findInsertionIndex(value);

  for (int i = getCount(); i >= index; i--)
  {
    this->values[i] = this->values[i-1];
  }

  this->values[index] = value;
  count++;
}
