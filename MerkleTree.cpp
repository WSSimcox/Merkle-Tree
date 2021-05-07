// Created by William Simcox for CSS343

#include "MerkleTree.h"

// Default constructor
MerkleTree::MerkleTree()
= default;

// Overloaded RBTree insert function for the merkle tree
void MerkleTree::insert(const byte digest[], const unsigned int key)
{
  // Inserts using RBTree insert
  RBTree::insert(digest, key);

  // Inserting the mid nodes
  if (key > 0)
  {
    // Inserts hash nodes
    RBTree::insert(digest, key - 50);
    hashify(this->root);
  }
}

// This function recalculates all changed hashes
void MerkleTree::hashify(Node *current_node)
{
  // Checking if tree is empty
  if (current_node == nullptr)
  {
    return;
  }

  SHA256 hash;

  // If node has left child and is a mid node
  if (current_node->left != nullptr && current_node->left->key % 100 == 50)
  {
    hashify(current_node->left);
  }

  // If node has right child and is a mid node
  if (current_node->right != nullptr && current_node->left->key % 100 == 50)
  {
    hashify(current_node->right);
  }

  // Updates left hash node and checks if not null
  if (current_node->left != nullptr)
  {
    hash.Update(current_node->left->digested_data, CryptoPP::SHA256::DIGESTSIZE);
  }

  // Updates right hash node and checks if not null
  if (current_node->right != nullptr)
  {
    hash.Update(current_node->right->digested_data, CryptoPP::SHA256::DIGESTSIZE);
  }

  // Finalizes the hash
  hash.Final(current_node->digested_data);
}

// This function takes in a data node and the key you are querying to
// see if it exists in the tree.
bool MerkleTree::verify(const byte *digest, const unsigned int key)
{
  Node *traversing_node = verifySearcher(root, key);

  // Making sure not nullptr
  if (traversing_node == nullptr)
  {
    return false;
  }

  // Memory compare
  if (memcmp(traversing_node->digested_data, digest, CryptoPP::SHA256::DIGESTSIZE) != 0)
  {
    return false;
  }

  // while not at the top
  while (traversing_node != root)
  {
    // Declaration of hash and hash array
    SHA256 hash;
    byte recalculated_digest[CryptoPP::SHA256::DIGESTSIZE];

    // Comparing traversing nodes
    if (traversing_node == traversing_node->parent->left)
    {
      hash.Update(traversing_node->digested_data, CryptoPP::SHA256::DIGESTSIZE);
      hash.Update(traversing_node->parent->right->digested_data, CryptoPP::SHA256::DIGESTSIZE);
    } else if (traversing_node == traversing_node->parent->right)
    {
      hash.Update(traversing_node->parent->left->digested_data, CryptoPP::SHA256::DIGESTSIZE);
      hash.Update(traversing_node->digested_data, CryptoPP::SHA256::DIGESTSIZE);
    }
    hash.Final(recalculated_digest);

    // Memory compare
    if (memcmp(traversing_node->parent->digested_data, recalculated_digest, CryptoPP::SHA256::DIGESTSIZE) != 0)
    {
      return false;
    }
    // Node goes up tree
    traversing_node = traversing_node->parent;
  }
  return true;
}

// This function depth searches through the merkle tree and returns
// a matched node.
Node *MerkleTree::verifySearcher(Node *current_node, const unsigned int key)
{
  // While loop that goes down the tree and searches
  while (current_node != nullptr)
  {
    if (key < current_node->key)
    {
      current_node = current_node->left;
    } else if (key > current_node->key)
    {
      current_node = current_node->right;
    } else
    {
      break;
    }
  }
  return current_node;
}

