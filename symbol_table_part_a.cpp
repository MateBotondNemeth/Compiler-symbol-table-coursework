// author: Mate Botond Nemeth
// Last modified: 02/05/2021
// C file processing part A

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include "identifier.h"

using namespace std;

// Processes the file, takes in a file name as a parameter
void c_file_processing (char** argv);

// Checks if the token is the type of the function / variable / array, takes in a string as a parameter
int get_type_flag (string token);

int main (int argc , char ** argv)
{
    c_file_processing (argv);

    // Writes the output file
    write_file(identifiers);

    return 0;
}

void c_file_processing (char** argv)
{
    ifstream file(argv[1]);
    string line;
    string token;
    int line_counter = 0;
    int variable_count = 0;
    int function_count = 0;
    int if_count = 0;
    int for_count = 0;
    int while_count = 0;
    int type_flag = 0;
    int variable_flag = 0;
    int function_flag = 0;

    // Iterates through the file
    while (getline(file, line))
    {
        line_counter++;

        // Iterates through each line
        stringstream stream (line);
        while (stream >> token)
        {
            // Flags if the token is a type
            if (type_flag == 0)
                type_flag = get_type_flag(token);

            // Counts the number of functions and makes an identifier of them and of the parameters
            if (type_flag == 1 && token.compare("(") == 0 && variable_flag == 0 && function_flag == 0)
            {
                function_count++;
                function_flag = 1;
                type_flag = 0;
                identifier_maker(line, line_counter);

                // First parameter
                identifier_maker(line.substr(line.rfind("(")), line_counter);

                // Rest of the parameters
                if (line.find(",") != -1)
                {
                    identifier_maker(line.substr(line.rfind(",")), line_counter);
                }
            }

            // Counts the int x = 5 ; type variables and makes an identifier of them
            if (type_flag == 1 && token.compare("=") == 0)
            {
                variable_count++;
                variable_flag = 1;
                type_flag = 0;
                identifier_maker(line, line_counter);
            }

            // Counts the void foo ( int x , int y ) ; type variables and makes an identifier of them
            if (type_flag == 1 && function_flag == 1)
            {
                variable_count++;
                variable_flag = 1;
                type_flag = 0;
            }

            // Counts the if, for and while statements
            if (token.compare("if") == 0)
                if_count++;
            if (token.compare("for") == 0)
                for_count++;
            if (token.compare("while") == 0)
                while_count++;

        }        

        // Counts the int x ; type variables and makes an identifier of them
        if (type_flag == 1)
        {
            variable_count++;
            type_flag = 0;
            identifier_maker(line, line_counter);
        }

        type_flag = 0;
        variable_flag = 0;
        function_flag = 0;
    }

    // Outputs the details of the file
    cout << "Variables: "<< variable_count << endl;
    cout << "Functions: "<< function_count << endl;
    cout << "If statements: "<< if_count << endl;
    cout << "For loops: "<< for_count << endl;
    cout << "While loops: "<< while_count << endl;

    // Updates the names of the identifiers
    update_names (identifiers);

    // Counts the refenrences of the identifiers
    count_reference (file);

    // Closes the file
    file.close();
}

int get_type_flag (string token)
{
    // All possible types
    vector<string> types = {"char", "short", "int", "long", "long long", "float", "double", "void", "struct"};
    int type_flag = 0;

    // Checks if the token is a pointer
    if (token.back() == '*')
        {
            while (token.back() == '*')
                token = token.substr(0, token.length() - 1);
        }
    for (string type : types)
    {
        if (token.compare(type) == 0)
            type_flag = 1;
    }
    
    return type_flag;
}

void identifier_maker (const string line, const int line_counter)
{
    if (line.compare("( )") == 0)
        return;
    
    struct identifier my_identifier;    // Creates an identifier
    my_identifier.line_number = line_counter;

    int type_flag = 0;
    int array_flag = 0;
    int variable_flag = 0;
    int function_flag = 0;
    int name_flag;
    string token;
    stringstream stream (line);
    while (stream >> token)
    {
        // Long long case
        if (type_flag == 1 && name_flag == 0 && token.compare("long") == 0)
            my_identifier.type = "long long";

        // Char [] case
        if (type_flag == 1 && name_flag == 1 && token.compare("[]") == 0)
            my_identifier.type = "char []";

        // Getting the name
        if (type_flag == 1 && name_flag == 0 && token.compare("long") != 0)
        {
            my_identifier.name = token;
            name_flag = 1;
        }

        // Checks if the token is the type of a variable/function/array
        if (name_flag == 0 && type_flag == 0)
        {
            type_flag = get_type_flag(token);
            if (type_flag == 1)
                my_identifier.type = token;
        }

        // Checks if the identifier is an array
        if (type_flag == 0 && token.front() == '[')
            array_flag = 1;
        
        // Getting the function and parameters
        if (type_flag == 1 && token.compare("(") == 0 && variable_flag == 0 && function_flag == 0)
        {
            function_flag = 1;
            type_flag = 0;
            my_identifier.identifier_type = "function";
        }

        // Getting the int x = 5 ; type variables
        if (type_flag == 1 && token.compare("=") == 0 && function_flag == 0)
        {
            variable_flag = 1;
            type_flag = 0;
            my_identifier.identifier_type = "variable";
        }
    }

    // Getting the int x ; type variables
    if (type_flag == 1 && function_flag == 0 && variable_flag == 0)
    {
        type_flag = 0;
        my_identifier.identifier_type = "variable";
    }

    // Getting the arrays
    if (my_identifier.identifier_type.compare("function") != 0 && array_flag == 1)
        my_identifier.identifier_type = "array";

    // Reseting the number of references
    my_identifier.reference_count = 0;

    identifiers.push_back(my_identifier);
}

void print_identifiers (const vector<struct identifier> &identifiers)
{
    for (struct identifier my_identifier : identifiers)
    {
        cout << my_identifier.name << ", line " << my_identifier.line_number << ", " << my_identifier.identifier_type 
        << ", " << my_identifier.type << ", referenced " << my_identifier.reference_count << endl;
    }
}

void write_file (const vector<struct identifier> &identifiers)
{
    // Creating the output file
    ofstream file ("identifiers.txt");

    for (struct identifier my_identifier : identifiers)
        file << my_identifier.name << ", line " << my_identifier.line_number << ", " << my_identifier.identifier_type 
        << ", " << my_identifier.type << ", referenced " << my_identifier.reference_count << endl;

    // Closing the output file
    file.close();
}

void update_names (vector<struct identifier> &identifiers)
{
    string function_name = "   ";

    // Appends the name of the local variables
    for (identifier &my_identifier : identifiers)
    {
        if (function_name.compare("main") != 0 && my_identifier.identifier_type.compare("function") != 0)
        {
            my_identifier.name.append(" (");
            my_identifier.name.append(function_name);
            my_identifier.name.append(")");
        }

        if (my_identifier.identifier_type.compare("function") == 0)
        {
            function_name = my_identifier.name;
        }
    }
}

void count_reference (ifstream &file)
{
    string line;
    string token;
    int line_number = 0;
    string function_name = "   ";
    
    // Scope struct with data consisting of a beginning line number, ending line number and a name
    struct scope
    {
        int begin = 1000;
        int end = 0;
        string name = " ";
    };

    // Vector of scopes
    vector <scope> scopes;

    //reseting the file
    file.clear();
    file.seekg(0, ios::beg);

    // Iterating through the file
    while (getline(file, line))
    {
        line_number++;
        stringstream stream(line);
        while (stream >> token)
        {   
            // Getting the scopes
            if (token.compare("{") == 0)
            {
                scope my_scope;
                my_scope.begin = line_number;
                for (identifier my_identifier : identifiers)
                {
                    if (my_identifier.line_number == line_number - 1 && my_identifier.identifier_type.compare("function") == 0)
                        my_scope.name = my_identifier.name;
                }
                scopes.push_back(my_scope);
            }

            for (identifier &my_identifier : identifiers)
            {
                // Functions
                if (token.compare(my_identifier.name) == 0 && my_identifier.line_number != line_number 
                    && my_identifier.identifier_type.compare("function") == 0)
                    my_identifier.reference_count++;
            }

            // End of the scope
            if (token.compare("}") == 0)
            {
                for (int i = scopes.size() - 1; i >= 0; i--)
                {
                    if (scopes[i].end == 0)
                    {
                        scopes[i].end = line_number;
                        break;
                    }
                }
            }
        }
    }

    line_number = 0;

    //reseting the file
    file.clear();
    file.seekg(0, ios::beg);

    while (getline(file, line))
    {
        line_number++;
        stringstream stream(line);
        while (stream >> token)
        {
            for (scope my_scope : scopes)
            {
                if (line_number >= my_scope.begin - 1 && my_scope.end > line_number && my_scope.name.compare(" ") != 0)
                {
                    function_name = my_scope.name;
                }
            }

            for (identifier &my_identifier : identifiers)
            {
                // Incrementing the reference counter of the identifier
                if (my_identifier.name.find(" ") != std::string::npos && my_identifier.identifier_type.compare("function") != 0)
                {
                    if (my_identifier.name.substr(0, my_identifier.name.find(" ")).compare(token) == 0)
                    {
                        if (my_identifier.name.find(function_name) != std::string::npos)
                        {
                            my_identifier.reference_count++;
                        }
                    }
                }

                if (function_name.compare("main") == 0 && my_identifier.name.compare(token) == 0 && my_identifier.identifier_type.compare("function") != 0)
                {
                    my_identifier.reference_count++;
                }
            }
        }
    }

    // Taking minus 1 from each non-function identifier, since they were counted at declaration too
    for (struct identifier &my_identifier : identifiers)
    {
        if (my_identifier.identifier_type.compare("function") != 0)
            my_identifier.reference_count--;
    }
}
