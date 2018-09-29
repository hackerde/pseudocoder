#include "Block.h"

int Block::COUNT = 0;

Block::Block() 
{
// default constructor
	text.push_back("");
	code = 0;
	args = 0;
}

Block::Block( string Text, int Code, int Args ) 
{
// constructor
	size_t loc1 = 0, loc2 = 0, len = 0;
	string temp;
	while (Text.find("_", loc1) != string::npos) {
		loc1 = Text.find("_", loc1);
		len = loc1 - loc2;
		temp = Text.substr(loc2, len);
		text.push_back(temp);
		text.push_back("____________________");
		loc1 += 20;						// change this if size of input field changes
		loc2 = loc1;
	}
	text.push_back(Text.substr(loc1));

	args = Args;
	code = Code;
	for (int i = 0; i < args; i++)
		inputs.push_back("");
}

Block::Block( const Block& block ) 
{
	text = block.text;
	code = block.code;
	inputs = block.inputs;
	args = block.args;
	inputs.clear();
	for (int i = 0; i < args; i++)
		inputs.push_back("");			// recreate a block but not the same inputs
}

void Block::setOutputString() 
{
// function to decide what to write to the file depending on what the block is
	switch (code)
	{
		// single line blocks

		case 1:
			outputString = "cout << \"" + getInputs()[0] + "\";";
			break;

		case 2:
			outputString = getInputs()[0] + " " + getInputs()[1] + ";";
			break;

		case 3:
			outputString = getInputs()[0] + "* " + getInputs()[1] + ";";
			break;

		case 4:
			outputString = "cin >> " + getInputs()[0] + ";";
			break;

		case 5:
			outputString = "cout << " + getInputs()[0] + ";";
			break;

		case 6:
			outputString = getInputs()[0] + " = " + getInputs()[1] + ";";
			break;

		case 7:
			outputString = "#DEFINE " + getInputs()[0] + " " + getInputs()[1] + ";";
			break;

		case 8:
			outputString = "getline( cin, " + getInputs()[0] + " );";
			break;

		case 9:
			outputString = getInputs()[0] + " " + getInputs()[2] + "[" + getInputs()[1] + "];";
			break;

		case 10:
			outputString = getInputs()[2] + " = " + "new " + getInputs()[0] + "[" + getInputs()[1] + "];";
			break;

		case 11:
			outputString = "delete [] " + getInputs()[0] + ";";
			break;

		case 12:
			outputString = "*" + getInputs()[0] + " = " + getInputs()[1] + ";";
			break;

		case 13:
			outputString = getInputs()[0] + " = " + "&" + getInputs()[1] + ";";
			break;

		case 14:
			outputString = getInputs()[1] + " += " + getInputs()[0] + ";";
			break;

		case 15:
			outputString = getInputs()[1] + " -= " + getInputs()[0] + ";";
			break;

		case 16:
			outputString = getInputs()[0] + " *= " + getInputs()[1] + ";";
			break;

		case 17:
			outputString = getInputs()[0] + " /= " + getInputs()[1] + ";";
			break;

		case 18:
			outputString = getInputs()[0] + " %= " + getInputs()[1] + ";";
			break;

		case 19:
			outputString = "break;";
			break;

		case 20:
			outputString = "continue;";
			break;

		case 21:
			outputString = "return " + getInputs()[0] + ";";
			break;

		case 22:
			if (getInputs()[2] == "")
				outputString = getInputs()[0] + "( " + getInputs()[1] + " );";
			else
				outputString = getInputs()[2] + " = " + getInputs()[0] + "( " + getInputs()[1] + " );";
			break;

		case 23:
			outputString = "ofstream " + getInputs()[1] + "( " + getInputs()[0] + " );";
			break;

		case 24:
			outputString = "ifstream " + getInputs()[1] + "( " + getInputs()[0] + " );";
			break;

		case 25:
			outputString = getInputs()[0] + " >> " + getInputs()[1] + ";";
			break;

		case 26:
			outputString = getInputs()[1] + " << " + getInputs()[0] + ";";
			break;

		case 27:
			outputString = "getline( " + getInputs()[0] + ", " + getInputs()[1] + " );";
			break;

		case 28:
			outputString = "cout << endl;";
			break;

		// multiline blocks starts from 31

		case 31:
			// c << COUNT;
			outputString = "for (int i" + to_string(COUNT) + " = 0; i" + to_string(COUNT) + " < " + getInputs()[0] + "; i" + to_string(COUNT) + "++) {";
			COUNT++;
			break;

		case 32:
			outputString = "while (" + getInputs()[0] + ") {";
			break;

		case 33:
			outputString = "if (" + getInputs()[0] + ") {";
			break;

		case 34:
			outputString = "else {";
			break;

		case 35:
			outputString = "else if (" + getInputs()[0] + ") {";
			break;

		case 36:
			outputString = getInputs()[1] + " " + getInputs()[0] + "( " + getInputs()[2] + " ) {";
			break;

	}
	
}

string Block::getText()
{
// converts the array of texts to a single string and returns it
	string toDisplay = "";
	for (int i = 0; i < text.size(); i++)
		toDisplay += text[i];

	return toDisplay;
}