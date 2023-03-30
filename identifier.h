// author: Mate Botond Nemeth
// Last modified: 02/05/2021
// Identifier struct header file

#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// The identifier struct
struct identifier
{
    string name;    // Name of the identifier
    int line_number;    // The line number
    string identifier_type; // Function or variable or array
    string type;    // Variable or array type or function return type
    int reference_count;    // How many times was it referenced
};

// Creating identifiers, takes in a string line as a parameter and an int, the line counter
void identifier_maker (const string line, const int line_counter);

// Printing all the identifiers, takes in a vector of identifiers as parameter
void print_identifiers (const vector<struct identifier> &identifiers);

// Printing the identifiers into a file, takes in a vector of identifiers as parameter
void write_file (const vector<struct identifier> &identifiers);

// Update names, takes in a vector of identifiers as parameter
void update_names (vector<struct identifier> &identifiers);

// Counting the refernces, takes in a ifstream file as a parameter
void count_reference (ifstream &file);

// A vector containing all the identifiers
static vector<struct identifier> identifiers;