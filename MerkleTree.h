// Created by William Simcox

#pragma once
#include "RBTree.h"

// This RBTree child class represents a Merkle Tree
class MerkleTree : RBTree
{
 public:
  // Constructor
  MerkleTree();
  // Overloaded RBTree insert function for the merkle tree
  void insert(const byte digest[], unsigned int key);
  // This function takes in a data node and the key you are querying to
  // see if it exists in the tree.
  bool verify(const byte digest[], unsigned int key);

 private:
  // This function recalculates all changed hashes
  void hashify(Node *current_node);
  // This function depth searches through the merkle tree and returns
  // a matched node.
  Node *verifySearcher(Node *current_node, unsigned int key);
};
