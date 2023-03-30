// author: Mate Botond Nemeth
// Last modified: 02/05/2021
// Binary Search Tree

#include <iostream>
#include <memory>
#include <string>
#include "tree.h"
#include "identifier.h"

using namespace std;

void Node::update (identifier &value)
{
	// Sets root
	if (root == nullptr)
		root = this;
	
	// Fills data if it is empty
	if (Node::data.name.compare("") == 0)
	{
		Node::data = value;
		Node::left = shared_ptr<Node> (new Node());
		Node::right = shared_ptr<Node> (new Node());
	}
	else
	{
		// If the name of the value is alphabetically smaller then it puts the value at the left branch
		if (value.name.compare(Node::data.name) < 0)
		{
			Node::left->update(value);
			Node::left->root = this->root;
		}
		// If the name of the value is alphabetically bigger then it puts the value at the right branch
		else if (value.name.compare(Node::data.name) > 0)
		{
			Node::right->update(value);
			Node::right->root = this->root;
		}
		// If the input value is the same as the data in the node it increments the reference counter
		else if (value.name.compare(Node::data.name) == 0)
			Node::data.reference_count++;
	}
}


void Node::inorder ()
{
	if (Node::data.name.compare("") != 0)
	{
		Node::left->inorder();
		cout << Node::print_data() << endl; 
		Node::right->inorder();
	}
}

string Node::print_data()
{
	string identifier_string;	// A line of the symbol_table as a string
	
	identifier_string = Node::data.name;
	identifier_string.append(", line ");
	identifier_string.append(to_string(Node::data.line_number));
	identifier_string.append(", ");
	identifier_string.append(Node::data.identifier_type);
	identifier_string.append(", ");
	identifier_string.append(Node::data.type);
	identifier_string.append(", referenced ");
	identifier_string.append(to_string(Node::data.reference_count));
	

	return identifier_string;
}
