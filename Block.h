#ifndef _BLOCK_
#define _BLOCK_

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
using namespace std;

class Block {

private:

	vector <string> text;				// the text the block is going to print out
	// the text is broken up into text to be displayed and the user input fields to have different colours for the user input fields
	vector <string> inputs;				// the user inputs for a block
	int code;							// code to uniquely identify each block
	int args;							// the number of user inputs for the block
	string outputString;				// the string to write to the file
	static int COUNT;					// to keep track of variables for 'for loops'

	// each block may have multiple user inputs. They will be read in as strings and stored in the vector inputs (an array basically)

public:

	Block();										// default constructor
	Block( string, int, int );					// constructor supplied with text, code, args values
	Block( const Block& );							// copy constructor

	//set/add and get methods for all variables
	string getText();
	vector <string> getTextArr() { return text; }	// no setText method needed

	int getCode() { return code; }
	void setCode( int c ) { code = c; }

	int getArgs() { return args; }
	void setArgs( int a ) { args = a; }

	string getOutputString() { return outputString; }
	void setOutputString();

	vector <string> getInputs() { return inputs; }
	void setInput( vector <string> in ) { inputs = in; }
	void addInput( string s ) { inputs.push_back(s); }

};

#endif