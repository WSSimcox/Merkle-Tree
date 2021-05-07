// Created by William Simcox for CSS343

#include <iostream>
#include "RBTree.h"

// Default constructor
RBTree::RBTree()
= default;

// Destructor
RBTree::~RBTree()
{
  deleteTree(root);
}

// Constructor
Node::Node(unsigned int key_in) : digested_data{0}
{
  key = key_in;
  parent = nullptr;
  left = nullptr;
  right = nullptr;
  color = 1;
}

// Default constructor
Node::Node() : digested_data{0}
{
  key = 0;
  parent = nullptr;
  left = nullptr;
  right = nullptr;
  color = 1;
}

// This function is for inserting a hash into the Red-Black Tree
void RBTree::insert(const byte digest[], unsigned int key_in)
{
  // Creating new node
  Node *inserted_node = new Node(key_in);
  // Memory copy
  memcpy_s(inserted_node->digested_data, CryptoPP::SHA256::DIGESTSIZE, digest, CryptoPP::SHA256::DIGESTSIZE);
  Node *current_node = nullptr;
  Node *next_node = this->root;

  // Checking if tree root is nullptr
  if (root == nullptr)
  {
    // Setting to new node and making it black
    root = inserted_node;
    root->color = 0;
    return;
  }

  // Finding where to place new node
  while (next_node != nullptr)
  {
    current_node = next_node;
    if (inserted_node->key < next_node->key)
    {
      next_node = next_node->left;
    } else
    {
      next_node = next_node->right;
    }
  }

  // Setting parent
  inserted_node->parent = current_node;

  // Comparing inserted nodes key and finding direction
  if (inserted_node->key < current_node->key)
  {
    current_node->left = inserted_node;
  } else
  {
    current_node->right = inserted_node;
  }

  // If grandparent is null
  if (inserted_node->parent->parent == nullptr)
  {
    return;
  }
  // Rebalancing the tree
  rebalanceTree(inserted_node);
}

// This function rebalances the Red-Black tree
void RBTree::rebalanceTree(Node *inserted_node)
{
  // Creating node uncle
  Node *uncle_node;
  // While parent color is red
  while (inserted_node->parent->color == 1)
  {
    if (inserted_node->parent == inserted_node->parent->parent->right)
    {
      uncle_node = inserted_node->parent->parent->left;
      // Unbalance check
      if (uncle_node != nullptr && uncle_node->color == 1)
      {
        uncle_node->color = 0;
        inserted_node->parent->color = 0;
        inserted_node->parent->parent->color = 1;
        inserted_node = inserted_node->parent->parent;
      }
        // Unbalance check
      else
      {
        // Sets uncle
        if (inserted_node == inserted_node->parent->left)
        {
          inserted_node = inserted_node->parent;
          rightRotate(inserted_node);
        }
        // Unbalance check
        inserted_node->parent->color = 0;
        inserted_node->parent->parent->color = 1;
        leftRotate(inserted_node->parent->parent);
      }
    } else
    {
      // Setting uncle
      uncle_node = inserted_node->parent->parent->right;
      // Unbalance check
      if (uncle_node != nullptr && uncle_node->color == 1)
      {
        uncle_node->color = 0;
        inserted_node->parent->color = 0;
        inserted_node->parent->parent->color = 1;
        inserted_node = inserted_node->parent->parent;
      } else
      {
        // Unbalance check
        if (inserted_node == inserted_node->parent->right)
        {
          inserted_node = inserted_node->parent;
          leftRotate(inserted_node);
        }
        // Unbalance check
        inserted_node->parent->color = 0;
        inserted_node->parent->parent->color = 1;
        rightRotate(inserted_node->parent->parent);
      }
    }
    // if top then stop
    if (inserted_node == root)
    {
      break;
    }
  }
  root->color = 0;
}

// This function rotates the given node left and is used by rebalanceTree
void RBTree::leftRotate(Node *node_to_rotate)
{
  Node *temp = node_to_rotate->right;
  node_to_rotate->right = temp->left;

  if (temp->left != nullptr)
  {
    temp->left->parent = node_to_rotate;
  }
  temp->parent = node_to_rotate->parent;

  if (node_to_rotate->parent == nullptr)
  {
    this->root = temp;
  } else if (node_to_rotate == node_to_rotate->parent->left)
  {
    node_to_rotate->parent->left = temp;
  } else
  {
    node_to_rotate->parent->right = temp;
  }
  temp->left = node_to_rotate;
  node_to_rotate->parent = temp;
}

// This function rotates the given node left and is used by rebalanceTree()
void RBTree::rightRotate(Node *node_to_rotate)
{
  Node *temp = node_to_rotate->left;
  node_to_rotate->left = temp->right;

  if (temp->right != nullptr)
  {
    temp->right->parent = node_to_rotate;
  }

  temp->parent = node_to_rotate->parent;

  if (node_to_rotate->parent == nullptr)
  {
    this->root = temp;
  } else if (node_to_rotate == node_to_rotate->parent->right)
  {
    node_to_rotate->parent->right = temp;
  } else
  {
    node_to_rotate->parent->left = temp;
  }
  temp->right = node_to_rotate;
  node_to_rotate->parent = temp;
}

// This function goes through the tree and deletes each node
void RBTree::deleteTree(Node *node)
{
  if (node != nullptr)
  {
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
    node = nullptr;
  }
}