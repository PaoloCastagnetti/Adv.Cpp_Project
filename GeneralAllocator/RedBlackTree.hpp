
#pragma once
#include <iostream>
#include "framework.h"
enum class Color { RED, BLACK };

template <typename T>
class GENERALALLOCATOR_API Node {
public:
    T key;
    Color color;
    Node* parent;
    Node* left;
    Node* right;
    /*
        @brief
        Create a red node with a key 
        @param key the key of the node
    */
    Node(T key) : key(key), color(Color::RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

template <typename T>
class GENERALALLOCATOR_API RedBlackTree {
private:
    Node<T>* root;

    /*
        @brief
        Helper function that Rotate the Tree to the left
        @param x Node to Rotate
    */
    void LeftRotate(Node<T>* x);
    /*
        @brief
        Helper function that Rotate the Tree to the right
        @param x Node to Rotate
    */
    void RightRotate(Node<T>* x);
    /*
        @brief
        Helper function that Insert the Node in the right place with the right color
        @param x Node to insert
    */
    void InsertFixup(Node<T>* x);
    /*
        @brief
        Helper function that replace a subtree with another updating the parent
        @param u root Node of the first subtree
        @param v root Node of the second subtree
    */
    void Transplant(Node<T>* u, Node<T>* v);
    /*
        @brief
        Helper function to return the minimum node of the tree
        @param node Node where to start to search the minimum
        @return the minimum node
    */
    Node<T>* Minimum(Node<T>* node);
    /*
        @brief
        Helper function to search a node with a specific key
        @param node Node where to start to search
        @param key the key to search
        @return the searched node if exists
    */
    Node<T>* SearchHelper(Node<T>* node, T key);
public:
    /*
        @brief
        Create a empty tree
    */
    RedBlackTree() : root(nullptr) {}
    /*
        @brief
        Insert the Node with a key
        @param key the key to insert
    */
    void Insert(T key);
    /*
        @brief
        Remove the Node with a key
        @param key the key to remove
    */
    void Remove(T key);
    /*
        @brief
        Remove the Node with a key
        @param key the key to remove
        @return the searched node if exists
    */
    Node<T>* Search(T key);
};

// Implementations of helper functions
template <typename T>
void RedBlackTree<T>::LeftRotate(Node<T>* x) {
    if (!x || !x->right)
        return; // Invalid rotation

    Node<T>* y = x->right; // Set y
    x->right = y->left;   // Turn y's left subtree into x's right subtree

    if (y->left)
        y->left->parent = x;

    y->parent = x->parent; // Link x's parent to y

    if (!x->parent)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;       // Put x on y's left
    x->parent = y;
}

template <typename T>
void RedBlackTree<T>::RightRotate(Node<T>* y) {
    if (!y || !y->left)
        return; // Invalid rotation

    Node<T>* x = y->left; // Set x
    y->left = x->right;  // Turn x's right subtree into y's left subtree

    if (x->right)
        x->right->parent = y;

    x->parent = y->parent; // Link y's parent to x

    if (!y->parent)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;    // Put y on x's right
    y->parent = x;
}

template <typename T>
void RedBlackTree<T>::InsertFixup(Node<T>* x) {
    while (x != root && x->parent && x->parent->color == Color::RED) {
        if (x->parent == x->parent->parent->left) {
            Node<T>* y = x->parent->parent->right; // Uncle
            if (y && y->color == Color::RED) {
                // Case 1: x's uncle is red
                x->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->right) {
                    // Case 2: x's uncle is black, and x is a right child
                    x = x->parent;
                    LeftRotate(x);
                }
                // Case 3: x's uncle is black, and x is a left child
                x->parent->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                RightRotate(x->parent->parent);
            }
        }
        else {
            // Symmetric cases (x's parent is a right child)
            Node<T>* y = x->parent->parent->left; // Uncle
            if (y && y->color == Color::RED) {
                // Case 1: x's uncle is red
                x->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->left) {
                    // Case 2: x's uncle is black, and x is a left child
                    x = x->parent;
                    RightRotate(x);
                }
                // Case 3: x's uncle is black, and x is a right child
                x->parent->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                LeftRotate(x->parent->parent);
            }
        }
    }

    root->color = Color::BLACK; // Ensure the root is black
}

// Implementation of public insertion method
template <typename T>
void RedBlackTree<T>::Insert(T key) {
    Node<T>* newNode = new Node<T>(key);
    Node<T>* current = root;
    Node<T>* parent = nullptr;

    // Find the parent node where the new node should be inserted
    while (current != nullptr) {
        parent = current;
        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    // Set the parent for the new node
    newNode->parent = parent;

    // If the tree is empty, make the new node the root
    if (parent == nullptr) {
        root = newNode;
    }
    else if (key < parent->key) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }

    // Perform fix-up to maintain Red-Black Tree properties
    InsertFixup(newNode);
}
template <typename T>
void RedBlackTree<T>::Remove(T key) {
    Node<T>* node = search(root, key);

    if (!node) {
        std::cout << "Node with key " << key << " not found." << std::endl;
        return;
    }

    Node<T>* y = node; // The node to be removed
    Node<T>* x = nullptr; // Node that replaces y
    Color yOriginalColor = y->color;

    if (!node->left) {
        x = node->right;
        transplant(node, node->right);
    }
    else if (!node->right) {
        x = node->left;
        transplant(node, node->left);
    }
    else {
        y = minimum(node->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == node)
            x->parent = y;
        else {
            transplant(y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }

        transplant(node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    delete node;

    if (yOriginalColor == Color::BLACK)
        deleteFixup(x);
}

// Helper function to transplant a node
template <typename T>
void RedBlackTree<T>::Transplant(Node<T>* u, Node<T>* v) {
    if (!u->parent)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v)
        v->parent = u->parent;
}

// Helper function to find the minimum node in a subtree rooted at a given node
template <typename T>
Node<T>* RedBlackTree<T>::Minimum(Node<T>* node) {
    while (node->left)
        node = node->left;
    return node;
}

// Helper function to search for a node with a specific key
template <typename T>
Node<T>* RedBlackTree<T>::SearchHelper(Node<T>* node, T key) {
    if (!node || node->key == key)
        return node;

    if (key < node->key)
        return SearchHelper(node->left, key);
    else
        return SearchHelper(node->right, key);
}
/*
*/
template <typename T>
Node<T>* RedBlackTree<T>::Search(T key) {
    return SearchHelper(root, key);
}