// Created by William Simcox for CSS343

#pragma once

#include "cryptlib.h"
#include "sha.h"
#include <string>

using namespace std;
using namespace CryptoPP;

// This struct is for the node that contains a hash and color
// which is stored into the Red-Black Tree.
struct Node
{
  unsigned int key;
  // 0 = black, 1 = red
  int color;
  Node *parent;
  Node *left;
  Node *right;
  byte digested_data[CryptoPP::SHA256::DIGESTSIZE];
  // Constructors
  explicit Node(unsigned int key_in);
  Node();
};

// This class represents a Red-Black tree data structure
class RBTree
{
 public:
  // constructor and destructor
  RBTree();
  ~RBTree();
  // This function is for inserting a node into the Red-Black Tree
  virtual void insert(const byte digest[], unsigned int key);

 protected:
  // root of the tree
  Node *root = nullptr;
  // This function rebalances the Red-Black tree
  void rebalanceTree(Node *inserted_node);
  // This function rotates a node left and is used by rebalanceTree
  void leftRotate(Node *node_to_rotate);
  // This function rotates a node right and is used by rebalanceTree
  void rightRotate(Node *node_to_rotate);
  // This function goes through the tree and deletes each node
  void deleteTree(Node *node);
};
