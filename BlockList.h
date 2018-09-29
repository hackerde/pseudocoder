// This file contains all usable blocks in the program. Blocks.txt contains a copy of the names of all the blocks
// Be sure to edit Block.cpp and the blockMap in main.cpp if you add a block here
// Single line blocks are upto #30
// Multiline blocks start from #31
// Change above two lines and appropriate lines in main.cpp accordingly

#include "Block.h"

Block nullBlock;

Block printText("Print the following text: ____________________", 1, 1);
Block createVariable("Create a ____________________ type variable named ____________________", 2, 2);
Block createPointer("Create a pointer to a ____________________ type variable named ____________________", 3, 2);
Block takeInput("Take input and store in the variable ____________________", 4, 1);
Block printVar("Print the value of the variable ____________________", 5, 1);
Block setVariable("Set the value of the variable ____________________ to the expression ____________________", 6, 2);
Block define("Define a constant named ____________________ and set it's value to ____________________", 7, 2);
Block takeLineInput("Take the entire line as a string input and store in the variable named ____________________", 8, 1);
Block createArray("Create an array of type ____________________ and of size ____________________ named ____________________", 9, 3);
Block createMemory("Allocate memory of type ____________________ and of size ____________________ and store it in the variable ____________________", 10, 3);
Block deleteMemory("Deallocate the memory at the pointer named ____________________", 11, 1);
Block setPointerVal("Set the value at the location of the pointer ____________________ to ____________________", 12, 2);
Block setPointerAddr("Make the pointer ____________________ point at the variable ____________________", 13, 2);
Block assignAdd("Add ____________________ to the variable ____________________ and assign the result to itself", 14, 2);
Block assignSub("Subtract ____________________ from the variable ____________________ and assign the result to itself", 15, 2);
Block assignMult("Multiply the variable ____________________ by ____________________ and assign the result to itself", 16, 2);
Block assignDiv("Divide the variable ____________________ by ____________________ and assign the result to itself", 17, 2);
Block assignMod("Divide the variable ____________________ by ____________________ and assign the remainder to itself", 18, 2);
Block breaking("Get out of the current loop", 19, 0);
Block continuing("Without executing the remaining statements of the loop, start the next iteration", 20, 0);
Block returnBlock("Return the value (or the variable named) ____________________", 21, 1);
Block callFunc("Call the function ____________________ with arguments (arg1, arg2,...) ____________________ and store returned value in ____________________", 22, 3);
Block openFileToRead("Open the file named ____________________ to read in data. Name the file locally as ____________________", 23, 2);
Block openFileToWrite("Open the file named ____________________ to write data to it. Name the file locally as ____________________", 24, 2);
Block readFromFile("Read in value from the file locally named ____________________ and store it in the variable named ____________________ ", 25, 2);
Block writeToFile("Write the data in the variable ____________________ to the file locally named ____________________", 26, 2);
Block readLineFromFile("Read in an entire line from the file locally named ____________________ and store it in the variable named ____________________", 27, 2);
Block printNewLine("Print a newline", 28, 0);
Block forBlock("Repeat the following indented step(s) ____________________ times", 31, 1);
Block whileBlock("Repeat the following indented step(s) until the expression ____________________ is false", 32, 1);
Block ifBlock("Check if the expression ____________________ is true. If yes, then execute the indented step(s)", 33,  1);
Block elseBlock("If the expression of the if-block is false, then execute the indented step(s)", 34, 0);
Block elseIfBlock("If the expression of the if-block is false, then check the expression ____________________. If true, execute the indented step(s)", 35, 1);
Block function("Create a function named ____________________ with return type ____________________. Arguments: (type1 var1, type2 var2,...) ____________________", 36, 3);