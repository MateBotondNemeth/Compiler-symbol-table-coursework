// author: Mate Botond Nemeth
// Last modified: 02/05/2021
// Binary Search Tree header file

#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "identifier.h"

using namespace std;

// The node of a tree
class Node
{
    public:
        // Insert data, takes in an identifier struct as a parameter
        void update (identifier &value);

        // Prints the tree in order
        void inorder ();

    private:
        identifier data = {"", 0, "", "", 0};   // Data of the node
	    shared_ptr<Node> left;  // Left branch
	    shared_ptr<Node> right; // Right branch
        Node *root = nullptr;   // Pointer to the root

        // Creates a string of the data of the Node
        string print_data();
        
};
