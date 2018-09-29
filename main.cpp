// Pseudo Coder by Anway De, Eduardo Ramirez and Rogelio Becerra    Spring 2018
// CSCI 251B - Software Design (Prof. Richey Matthew)

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "BlockList.h"
#include <stdio.h>
#include <stdlib.h>

// Global variables
    sf::Font font;
    map <string, Block*> blockMap;                    // to map strings to blocks
    sf::Text cursorText;                              // "|"
    stringstream userInput;                           // to store input from user
    int blockID = 0;                                  // to track which addBlockButton is clicked
    int pushX = 25, pushY = 25;                       // when creating new blocks
    int d;                                            // mousewheel scroll
    int MAX_LEN = 20;                                 // max length of user input

    // assert: only one menu will be drawn to the screen at a certain time
    sf::RectangleShape* currentMenu = 0;              // pointer to the menu that will be drawn
    sf::RectangleShape blockMenu;                     // menu for blocks

// vectors in the program
    vector <Block*> blocksInProg;                     // Blocks in program
    vector < vector<sf::Text> > textToDisplayParts;   // Texts for the blocks
    vector <sf::RectangleShape> addBlockButtons;      // the addBlock buttons currently on screen
    vector <sf::Text> plusButtons;                    // plus button inside addBlockButton
    vector <sf::Text> listOfCommands;                 // store the strings to be displayed in the menu
    vector <sf::RectangleShape> boxes;                // store the rectangles surrounding the corresponding string
    vector < vector<sf::Vector2f> > inputLocations;   // locations where text can be input
    vector < vector <sf::Text> > inputTexts;          // texts typed in the input fields
    vector <sf::Text> deleteBlockButtons;             // buttons to delete blocks
    vector <sf::RectangleShape> inputBoxes;           // boxes around input locations

    sf::Vector2f mousePosition;                       // stores the position of the mouse
    sf::Vector2f cursor(0, 0);                        // position of cursor on the screen

// buttons ---> Rectangle and Text
    sf::Text saveText;                                // save button
    sf::RectangleShape save;
    sf::Text deleteText;                              // delete all button
    sf::RectangleShape deleteAll;
    sf::Text view;                                    // view code button
    sf::RectangleShape viewButton;
    sf::RectangleShape textBox;                       // textbox for saving name
    sf::Text saveName;
    sf::Text prompt;
    sf::RectangleShape compileAndRun;                 // compile and run button
    sf::Text CAndR;
// for main menu
    sf::Text title;                                   // title of main menu
    sf::RectangleShape button1, button2, button3;              // buttons in main menu
    sf::Text button1Text, button2Text, button3Text;

// windows
    sf::RenderWindow window;
    sf::RenderWindow menuWindow;

sf::Text createText(sf::Font& f, int cs, sf::Color c, string t, sf::Vector2f p)
{
// function to create a text type object
    sf::Text temp;
    temp.setFont(f);
    temp.setCharacterSize(cs);
    temp.setColor(c);
    temp.setString(t);
    temp.setPosition(p);

    return temp;
}

sf::RectangleShape createRect(sf::Vector2f s, sf::Color fc, sf::Color oc, int ot, sf::Vector2f p)
{
// function to create a rectangle type object

    sf::RectangleShape temp;
    temp.setSize(s);
    temp.setFillColor(fc);
    temp.setOutlineColor(oc);
    temp.setOutlineThickness(ot);
    temp.setPosition(p);

    return temp;
}

sf::Text createSFT( string s = "", int cs = 15, sf::Color c = sf::Color::Black ) 
{
// function to create a sf::Text type object with necessary attributes

    sf::Text text;
    text.setString(s);
    text.setFont(font);
    text.setColor(c);
    text.setCharacterSize(cs);

    return text;
}

vector <sf::Text> createBlockText( Block* block, int j ) 
{
// function to create the necessary text to display for a block
    
    vector <string> blockTextArr = block->getTextArr();
    vector <sf::Text> texts;
    int x = addBlockButtons[j].getPosition().x;
    int y = addBlockButtons[j].getPosition().y;
    for (int i = 0; i < blockTextArr.size(); i++)
    {
        sf::Text blockText;
        blockText = createSFT(blockTextArr[i]);
        blockText.setPosition(x, y);
        if (i%2 == 1)
        {
            blockText.setColor(sf::Color(102, 0, 102));
            blockText.setStyle(sf::Text::Underlined);
        }
        texts.push_back(blockText);
    }
    
    return texts;
}

void updateInputBoxLocations(sf::RenderWindow& w)
{
// updates the vector inputBoxes to properly detect mouse clicks and highlight active field

    inputBoxes.clear();

    for (int i = 0; i < inputLocations.size(); i++)
    {
        for (int j = 0; j < inputLocations[i].size(); j++)
        {
            sf::RectangleShape rect(sf::Vector2f(200, 15));
            rect.setFillColor(sf::Color::Transparent);
            rect.setPosition(inputLocations[i][j]);
            inputBoxes.push_back(rect);
        }
    }

    for (int i = 0; i < inputBoxes.size(); i++)
    {
        if (inputBoxes[i].getGlobalBounds().contains(cursor))
            inputBoxes[i].setFillColor(sf::Color::Green);
        w.draw(inputBoxes[i]);
    }
}

int updateVectors( string sel, int k ) 
{
// function to update the contents of the following vectors in the program
// textToDisplayParts
// deleteBlockButtons
// blocksInProg
// inputLocations
// inputTexts

    // create necessary vector entries
    vector <sf::Text> tempText = createBlockText(blockMap[sel], k);
    Block *ptr = new Block(*(blockMap[sel]));
    int y  = tempText[0].getPosition().y;
    int flag = 0;
    int c = ptr->getCode();
    sf::Vector2f pos;
    vector <sf::Vector2f> loc;
    vector <sf::Text> temp;

    for (int i = 0; i < tempText.size()-1; i++)
    {
        pos = tempText[i].findCharacterPos(10000);
        tempText[i+1].setPosition(pos);
    }

    for (int i = 0; i < tempText.size(); i++)
    {
        if (i%2 == 1)
        {
            loc.push_back(tempText[i].getPosition());
            temp.push_back(createSFT());
            temp[temp.size()-1].setPosition(tempText[i].getPosition());
            temp[temp.size()-1].setColor(sf::Color(102, 0, 102));
        }
    }

    sf::Text deleteButton = createSFT("X", 15, sf::Color::Red);
    deleteButton.setPosition(10, addBlockButtons[k].getPosition().y);
    

    // vectors contain one entry for every block in the order which blocks are on the screen    
    for (int i = 0; i < textToDisplayParts.size(); i++)
    {
        if (y < textToDisplayParts[i][0].getPosition().y)
        {
            textToDisplayParts.insert(textToDisplayParts.begin()+i, tempText);
            deleteBlockButtons.insert(deleteBlockButtons.begin()+i, deleteButton);
            blocksInProg.insert(blocksInProg.begin()+i, ptr);
            inputLocations.insert(inputLocations.begin()+i, loc);
            inputTexts.insert(inputTexts.begin()+i, temp);
            flag = 1;
            break;
        }
    }

    if (flag == 0)
    {
        textToDisplayParts.push_back(tempText);
        deleteBlockButtons.push_back(deleteButton);
        blocksInProg.push_back(ptr);
        inputLocations.push_back(loc);
        inputTexts.push_back(temp);
    }
    return c;
}

void handleMenuItemSelection( string selected, int id )
{
// function to update appropriate vectors when a menu item is selected
// vectors updated: addBlockButtons, plusButtons

    int c = updateVectors(selected, id);

    // what to do if a single line block is selected
    // move everything below the block down
    if ( c <= 30 )
    {
        for (int i = 0; i < textToDisplayParts.size(); i++)
        {
            for (int j = 0; j < textToDisplayParts[i].size(); j++)
            {
                if (textToDisplayParts[i][j].getPosition().y > addBlockButtons[id].getPosition().y)
                    textToDisplayParts[i][j].move(0, pushY);
            }
            for (int j = 0; j < inputLocations[i].size(); j++)
            {
                if (inputLocations[i][j].y > addBlockButtons[id].getPosition().y)
                {
                    inputLocations[i][j].y += pushY;
                    inputTexts[i][j].move(0, pushY);
                }
            }
            if (deleteBlockButtons[i].getPosition().y > addBlockButtons[id].getPosition().y)
                deleteBlockButtons[i].move(0, pushY);
        }

        for (int i = 0; i < addBlockButtons.size(); i++)
        {
            if (addBlockButtons[i].getPosition().y > addBlockButtons[id].getPosition().y)
            {
                addBlockButtons[i].move(0, pushY);
                plusButtons[i].move(0, pushY);
            }
        }
        addBlockButtons[id].move(0, pushY);
        plusButtons[id].move(0, pushY);
    }
    else
    {     
    // what to do if a multiline block is selected
    // create another add block button   
        sf::RectangleShape addBlock(sf::Vector2f(20, 20));
        addBlock.setFillColor(sf::Color::Transparent);
        addBlock.setPosition(addBlockButtons[id].getPosition());
        addBlock.move(pushX, pushY);
        addBlock.setOutlineColor(sf::Color(75, 83, 32));
        addBlock.setOutlineThickness(-1);
        sf::Text plus = createSFT("+", 25, sf::Color(75, 83, 32));
        plus.setPosition(addBlock.getPosition());
        plus.move(3, -7);

    // move everything below it down twice the usual
        for (int i = 0; i < textToDisplayParts.size(); i++)
        {
            for (int j = 0; j < textToDisplayParts[i].size(); j++)
            {
                if (textToDisplayParts[i][j].getPosition().y > addBlockButtons[id].getPosition().y)
                    textToDisplayParts[i][j].move(0, 2*pushY);
            }
            for (int j = 0; j < inputLocations[i].size(); j++)
            {
                if (inputLocations[i][j].y > addBlockButtons[id].getPosition().y)
                {
                    inputLocations[i][j].y += 2*pushY;
                    inputTexts[i][j].move(0, 2*pushY);
                }
            }
            if (deleteBlockButtons[i].getPosition().y > addBlockButtons[id].getPosition().y)
                deleteBlockButtons[i].move(0, 2*pushY);
        }

        for (int i = 0; i < addBlockButtons.size(); i++)
        {
            if (addBlockButtons[i].getPosition().y > addBlockButtons[id].getPosition().y)
            {
                addBlockButtons[i].move(0, 2*pushY);
                plusButtons[i].move(0, 2*pushY);
            }
        }

        addBlockButtons[id].move(0, 2*pushY);
        plusButtons[id].move(0, 2*pushY);
        addBlockButtons.push_back(addBlock);
        plusButtons.push_back(plus);
    }
}

void moveCursor()
{
// function to move cursor to next inputLocation
    for (int i = 0; i < inputLocations.size(); i++)
    {
        for (int j = 0; j < inputLocations[i].size(); j++)
        {
            if (cursor == inputLocations[i][j])
            {
                if (j+1 < inputLocations[i].size())
                {
                    cursor = inputLocations[i][j+1];
                    return;
                }
                else
                {
                    cursor = inputLocations[i+1][0];
                    return;
                }
            }
        }  
    }
}

void drawVectors( sf::RenderWindow& w )
{
// function to draw all the necessary vectors the screen
    for (int i = 0; i < textToDisplayParts.size(); i++)
    {
        for (int j = 0; j < textToDisplayParts[i].size(); j++)
        {
            w.draw(textToDisplayParts[i][j]);
        }
    }

    // draw all delete block buttons
    for (int i = 0; i < deleteBlockButtons.size(); i++)
            w.draw(deleteBlockButtons[i]);

    // draw all addBlock buttons
    for (int i = 0; i < addBlockButtons.size(); i++)
    {
        w.draw(addBlockButtons[i]);
        w.draw(plusButtons[i]);
    }

    // draw all user inputs to the screen
    for (int i = 0; i < inputTexts.size(); i++)
    {
        for (int j = 0; j < inputTexts[i].size(); j++)
        {
            w.draw(inputTexts[i][j]);
        }
    }
}

void handleTextEntering( string input )
{
// function to store user input in appropriate block inputs and vectors
// vectors updated: blocksInProg, inputTexts
    vector <string> inp;
    for (int i = 0; i < inputLocations.size(); i++)
    {
        for (int j = 0; j < inputLocations[i].size(); j++)
        {
            if (cursor == inputLocations[i][j])
            {
                inp = blocksInProg[i]->getInputs();
                inp[j] = input;
                blocksInProg[i]->setInput(inp);
                inputTexts[i][j].setString(input);
                return;
            }
        }     
    }
}

void handleSaveButtonPress( string fileName ) 
{
// function to handle what happens when the save button is pressed
    if (fileName == "")             // do nothing if empty
        return;

    fileName = fileName + ".cpp";
    ofstream file(fileName.c_str());

// open a .cpp file and start writing to it
    int track1 = 0, track2 = 0, tabCount = -1, flag = 0, flag1 = 0;
    // all files have the following headers
    file << "#include <iostream>\n#include <string>\n#include <fstream>\nusing namespace std;\n\n";
    for (int i = 0; i < blocksInProg.size(); i++)
    {
        flag1 = 0;
        track2 = track1;
        blocksInProg[i]->setOutputString();             // sets what to write to file for corresponding block
        track1 = textToDisplayParts[i][0].getPosition().x;
        if (track1 > track2)                            // if code if indented add a tab
        {
            tabCount++;
        }
        else if (track1 < track2)                       // if code comes out of indentation
        {
            int diff = (track2 - track1)/pushX;         // how many levels of indentation
            for (int k = 0; k <= diff; k++)             
            {
            // loop to print appropriate number of '}'
                if (blocksInProg[i-1]->getCode() <= 30 && flag1 == 0)
                {
                    tabCount--;
                    flag1 = 1;
                    continue;
                }
                for (int t = 0; t < tabCount; t++)
                {
                    file << "\t";                       // print the appropriate number of tabs per line
                }
                file << "}" << endl;
                tabCount--;
            }
            tabCount++;                                 // the last tabCount-- was extra
        }
        for (int j = 0; j < tabCount; j++)
                file << "\t";
        file << blocksInProg[i]->getOutputString();     // write the block to file
        file << endl;
    }
    while (tabCount >= 0)                               // after all blocks have been written and last block was indented
    {
    // print appropriate number of '}' with correct number of tabs
        if (blocksInProg[blocksInProg.size()-1]->getCode() <= 30 && flag == 0)
        {
            tabCount--;
            flag = 1;
            continue;
        }
        for (int j = 0; j < tabCount; j++)
            file << "\t";
        file << "}" << endl;
        tabCount--;
    }
    file.close();
}

void handleDeleteButtonPress( int id )
{
// function to handle what happens when a deleteBlockButton is pressed
// the following vectors are updated
// textToDisplayParts
// deleteBlockButtons
// blocksInProg
// inputLocations
// inputTexts
// addBlockButtons
// plusButtons

    if (blocksInProg[id]->getCode() <= 30)
    {
    // if block was a single line block, move everything below it up

        for (int i = 0; i < textToDisplayParts.size(); i++)
        {
            for (int j = 0; j < textToDisplayParts[i].size(); j++)
            {
                if (textToDisplayParts[i][j].getPosition().y > deleteBlockButtons[id].getPosition().y)
                    textToDisplayParts[i][j].move(0, (-1)*pushY);
            }
            for (int j = 0; j < inputLocations[i].size(); j++)
            {
                if (inputLocations[i][j].y > deleteBlockButtons[id].getPosition().y)
                {
                    inputLocations[i][j].y -= pushY;
                    inputTexts[i][j].move(0, (-1)*pushY);
                }
            }
        }

        for (int i = 0; i < addBlockButtons.size(); i++)
        {
            if (addBlockButtons[i].getPosition().y > deleteBlockButtons[id].getPosition().y)
            {
                addBlockButtons[i].move(0, (-1)*pushY);
                plusButtons[i].move(0, (-1)*pushY);
            }
        }

        for (int i = 0; i < deleteBlockButtons.size(); i++)
        {
            if (deleteBlockButtons[i].getPosition().y > deleteBlockButtons[id].getPosition().y)
                deleteBlockButtons[i].move(0, (-1)*pushY);
        }

        // delete the block from the vectors
        blocksInProg.erase(blocksInProg.begin()+id);
        textToDisplayParts.erase(textToDisplayParts.begin()+id);
        deleteBlockButtons.erase(deleteBlockButtons.begin()+id);
        inputLocations.erase(inputLocations.begin()+id);
        inputTexts.erase(inputTexts.begin()+id);
    }
    else
    {
    // if it was a multiline block
        while (true)
        {
            // recursively call delete on all single line and multiline blocks under it
            if (id+1 < textToDisplayParts.size() && textToDisplayParts[id][0].getPosition().x < textToDisplayParts[id+1][0].getPosition().x)
                handleDeleteButtonPress(id+1);
            else
            {
            // after that is done, move everything below it up by twice the normal amount
                for (int i = 0; i < addBlockButtons.size(); i++)
                {
                    if (addBlockButtons[i].getPosition().y == deleteBlockButtons[id].getPosition().y+pushY)
                    {
                    // erase the extra button
                        addBlockButtons.erase(addBlockButtons.begin()+i);
                        plusButtons.erase(plusButtons.begin()+i);
                        break;
                    }
                }

                for (int i = 0; i < textToDisplayParts.size(); i++)
                {
                    for (int j = 0; j < textToDisplayParts[i].size(); j++)
                    {
                        if (textToDisplayParts[i][j].getPosition().y > deleteBlockButtons[id].getPosition().y)
                            textToDisplayParts[i][j].move(0, (-2)*pushY);
                    }
                    for (int j = 0; j < inputLocations[i].size(); j++)
                    {
                        if (inputLocations[i][j].y > deleteBlockButtons[id].getPosition().y)
                        {
                            inputLocations[i][j].y -= 2*pushY;
                            inputTexts[i][j].move(0, (-2)*pushY);
                        }
                    }
                }

                for (int i = 0; i < addBlockButtons.size(); i++)
                {
                    if (addBlockButtons[i].getPosition().y > deleteBlockButtons[id].getPosition().y)
                    {
                        addBlockButtons[i].move(0, (-2)*pushY);
                        plusButtons[i].move(0, (-2)*pushY);
                    }
                }

                for (int i = 0; i < deleteBlockButtons.size(); i++)
                {
                    if (deleteBlockButtons[i].getPosition().y > deleteBlockButtons[id].getPosition().y)
                        deleteBlockButtons[i].move(0, (-2)*pushY);
                }


                // delete the block from the vectors
                blocksInProg.erase(blocksInProg.begin()+id);
                textToDisplayParts.erase(textToDisplayParts.begin()+id);
                deleteBlockButtons.erase(deleteBlockButtons.begin()+id);
                inputLocations.erase(inputLocations.begin()+id);
                inputTexts.erase(inputTexts.begin()+id);
                break;
            }
        }
    }
}

string GetStdoutFromCommand( string cmd ) {
// function to execute a shell command and redirect its output back to the program
// source: https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd = cmd + " 2>&1";

    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
    }
    return data;
}

void handleViewButtonPress( string fileName )
{
// function to control what happens when user clicks the view button
// open a new window to show the code
// hide coding window

    if (fileName == "")
        return;

    string output = GetStdoutFromCommand("cat " + fileName + ".cpp");       // using shell command to catch the code!
    sf::Text result = createSFT(output);
    result.move(20, 20);

    window.setVisible(false);
    sf::RenderWindow CodeWindow(sf::VideoMode(1000, 700), fileName, sf::Style::Titlebar|sf::Style::Close);

    while (CodeWindow.isOpen())
    {
        sf::Event event;
        while (CodeWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                CodeWindow.close();

            if (event.type == sf::Event::MouseWheelScrolled)
            {
                d = event.mouseWheelScroll.delta;
                result.move(0, d*10);
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                    result.move(0, 10);
                if (event.key.code == sf::Keyboard::Down)
                    result.move(0, -10);
            }
        }

        CodeWindow.clear(sf::Color::White);

        CodeWindow.draw(result);

        CodeWindow.display();
    }
    window.setVisible(true);
}

void setCursorBarPosition()
{
// control movement of cursor bar
    if (cursor == saveName.getPosition())
    {
        cursorText.setPosition(saveName.findCharacterPos(10000));
        cursorText.move(1, 0);
        return;
    }

    for (int i = 0; i < inputLocations.size(); i++)
    {
        for (int j = 0; j < inputLocations[i].size(); j++)
        {
            if (cursor == inputLocations[i][j])
            {
                cursorText.setPosition(inputTexts[i][j].findCharacterPos(10000));
                cursorText.move(1, -2);
                return;
            }
        }
    }
}

void handleCompileButtonPress( string fileName )
{
    if (fileName == "")
        return;

    // our program is unable to redirect user input back to the terminal, so this avoids running files having 'cin'
    string code = GetStdoutFromCommand("cat " + fileName + ".cpp");
    string output = GetStdoutFromCommand("g++ -Wall -std=c++11 -o " + fileName + " " + fileName + ".cpp");

    if(output == "")
    {
        if (code.find("cin") != string::npos)
            output = "Sorry! Your program has a \"cin\" command. This software cannot RUN programs that take user input.";
        else
            output = GetStdoutFromCommand("./" + fileName);
    }
    
    // create new window and display output
    window.setVisible(false);
    sf::RenderWindow OutputWindow(sf::VideoMode(1000, 700), "Output", sf::Style::Titlebar|sf::Style::Close);

    sf::Text result = createSFT(output);
    result.move(20, 20);

    while (OutputWindow.isOpen())
    {
        sf::Event event;
        while (OutputWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                OutputWindow.close();

            if (event.type == sf::Event::MouseWheelScrolled)
            {
                d = event.mouseWheelScroll.delta;
                result.move(0, d*10);
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                    result.move(0, 10);
                if (event.key.code == sf::Keyboard::Down)
                    result.move(0, -10);
            }
        
        OutputWindow.clear(sf::Color::White);

        OutputWindow.draw(result);

        OutputWindow.display();
        }
    }
    window.setVisible(true);
}

void handleButton2Press(sf::RenderWindow& w)
{
// function to handle what happens when tutorial button is pressed
// creates a new window and hides main menu window

    w.setVisible(false);
    sf::RenderWindow tutorialWindow(sf::VideoMode(1500, 800), "PseudoCoder Tutorial", sf::Style::Titlebar|sf::Style::Close);

    sf::Text tutorialText;
    string toPrint = "", line = "";
    ifstream tut("Tutorial.txt");

    do
    {
        getline(tut, line);
        line = line.substr(0, line.length()-1);
        toPrint += line;
        toPrint += "\n\n";
    }while (line != "");

    tutorialText = createSFT(toPrint);

    while (tutorialWindow.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (tutorialWindow.pollEvent(event))
        {
            // "close requested" event: we close the window
            switch (event.type){

                case sf::Event::Closed:
                    tutorialWindow.close();
                    break;

                case sf::Event::MouseWheelScrolled:
                    d = event.mouseWheelScroll.delta;
                    tutorialText.move(0, d*10);
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Up)
                        tutorialText.move(0, 10);
                    if (event.key.code == sf::Keyboard::Down)
                        tutorialText.move(0, -10);
            }
        }

        tutorialWindow.clear(sf::Color(135,206,250));
        tutorialWindow.draw(tutorialText);
        tutorialWindow.display();
    }
    // main menu reappears on closing tutorial window
    w.setVisible(true);
}

void scroll( int x, int y )
{
    // move everything on screen at the rate pushY/2
    for (int i = 0; i < textToDisplayParts.size(); i++)
    {
        for (int j = 0; j < textToDisplayParts[i].size(); j++)
            textToDisplayParts[i][j].move(x, y);

        deleteBlockButtons[i].move(x, y);

        for (int j = 0; j < inputLocations[i].size(); j++)
        {
                inputLocations[i][j].y += y;
                inputLocations[i][j].x += x;
                inputTexts[i][j].move(x, y);
        }
    }

    for (int i = 0; i < addBlockButtons.size(); i++)
    {
        addBlockButtons[i].move(x, y);
        plusButtons[i].move(x, y);
    }

    cursor.y += y;
    cursor.x += x;

    if (currentMenu == &blockMenu)
    {
        blockMenu.move(x, y);
    }
}

int main()
{

    // map between string from blockMenu to actual blocks
        blockMap["printText"] = &printText;
        blockMap["createVariable"] = &createVariable;
        blockMap["createPointer"] = &createPointer;
        blockMap["takeInput"] = &takeInput;
        blockMap["printVar"] = &printVar;
        blockMap["setVariable"] = &setVariable;
        blockMap["define"] = &define;
        blockMap["takeLineInput"] = &takeLineInput;
        blockMap["createArray"] = &createArray;
        blockMap["createMemory"] = &createMemory;
        blockMap["deleteMemory"] = &deleteMemory;
        blockMap["setPointerVal"] = &setPointerVal;
        blockMap["setPointerAddr"] = &setPointerAddr;
        blockMap["assignAdd"] = &assignAdd;
        blockMap["assignSub"] = &assignSub;
        blockMap["assignMult"] = &assignMult;
        blockMap["assignDiv"] = &assignDiv;
        blockMap["assignMod"] = &assignMod;
        blockMap["breaking"] = &breaking;
        blockMap["continuing"] = &continuing;
        blockMap["forBlock"] = &forBlock;
        blockMap["whileBlock"] = &whileBlock;
        blockMap["ifBlock"] = &ifBlock;
        blockMap["elseBlock"] = &elseBlock;
        blockMap["elseIfBlock"] = &elseIfBlock;
        blockMap["function"] = &function;
        blockMap["returnBlock"] = &returnBlock;
        blockMap["callFunc"] = &callFunc;
        blockMap["openFileToRead"] = &openFileToRead;
        blockMap["openFileToWrite"] = &openFileToWrite;
        blockMap["readFromFile"] = &readFromFile;
        blockMap["writeToFile"] = &writeToFile;
        blockMap["readLineFromFile"] = &readLineFromFile;
        blockMap["printNewLine"] = &printNewLine;

    // font for the text
    font.loadFromFile("Courier.ttf");

    // save button
    saveText = createSFT("Save", 20);
    saveText.setPosition(1414, 582);

    save.setSize(sf::Vector2f(75, 30));
    save.setFillColor(sf::Color::Green);
    save.setOutlineColor(sf::Color::Black);
    save.setOutlineThickness(2);
    save.setPosition(1400, 580);

    // Delete All button
    deleteText = createSFT("Delete All", 20);
    deleteText.setPosition(1348, 662);

    deleteAll.setSize(sf::Vector2f(135, 30));
    deleteAll.setFillColor(sf::Color::Green);
    deleteAll.setOutlineColor(sf::Color::Black);
    deleteAll.setOutlineThickness(2);
    deleteAll.setPosition(1340, 660);

    // View Code Button
    view = createSFT("View Code", 20);
    view.setPosition(1355, 622);

    viewButton.setSize(sf::Vector2f(130, 30));
    viewButton.setFillColor(sf::Color::Green);
    viewButton.setOutlineColor(sf::Color::Black);
    viewButton.setOutlineThickness(2);
    viewButton.setPosition(1345, 620);

    // Save textbox
    saveName = createSFT("code");
    saveName.setPosition(1280, 747);

    textBox.setSize(sf::Vector2f(200, 25));
    textBox.setFillColor(sf::Color::White);
    textBox.setOutlineThickness(3);
    textBox.setOutlineColor(sf::Color::Black);
    textBox.setPosition(1275, 745);

    prompt = createSFT("Enter File Name: ");
    prompt.setPosition(1115 ,747);

    // compile and run button
    compileAndRun.setSize(sf::Vector2f(200, 30));
    compileAndRun.setFillColor(sf::Color::Green);
    compileAndRun.setOutlineThickness(2);
    compileAndRun.setOutlineColor(sf::Color::Black);
    compileAndRun.setPosition(1275, 700);

    CAndR = createSFT("Compile and Run", 20);
    CAndR.setPosition(1280, 702);

    // configs for creating windows
    window.create(sf::VideoMode(1500, 800), "PseudoCoder", sf::Style::Titlebar|sf::Style::Close);
    menuWindow.create(sf::VideoMode(1500, 800), "PseudoCoder", sf::Style::Titlebar|sf::Style::Close);

    // intial block to click on to add blocks

    sf::RectangleShape addBlock(sf::Vector2f(20, 20));
    addBlock.setFillColor(sf::Color::Transparent);
    addBlock.setPosition(30, 25);
    addBlock.setOutlineColor(sf::Color(75, 83, 32));
    addBlock.setOutlineThickness(-1);
    sf::Text plus = createSFT("+", 25, sf::Color(75, 83, 32));
    plus.setPosition(addBlock.getPosition());
    plus.move(3, -7);

    addBlockButtons.push_back(addBlock);
    plusButtons.push_back(plus);

    cursorText = createSFT("|", 15, sf::Color::Black);

    string command, blockName;  // to create the blocks menu
    ifstream blockNames("Blocks.txt");  // file from which the block menu is made
    
    // Creating the Block Menu

    // loop to store menu items in block menu
    while (blockNames >> command) 
    {

        sf::Text text;
        text = createSFT(command, 10);
        listOfCommands.push_back(text);

        sf::RectangleShape selected(sf::Vector2f(120, 15));
        selected.setFillColor(sf::Color::Transparent);
        boxes.push_back(selected);
    }

    // overall rectangle in block menu
    blockMenu.setSize(sf::Vector2f(120, 15*listOfCommands.size()));
    blockMenu.setFillColor(sf::Color(220, 220, 220));


    // buttons for main menu

    title = createText(font, 80, sf::Color::Red, "Pseudo Coder", sf::Vector2f(500, 50));
    title.setStyle(sf::Text::Underlined);

    button1 = createRect(sf::Vector2f(220, 50), sf::Color::Green, sf::Color::Black, 2, sf::Vector2f(650, 230));

    button2 = createRect(sf::Vector2f(220, 50), sf::Color::Green, sf::Color::Black, 2, sf::Vector2f(650, 230));
    button2.move(0, 100);

    button3 = createRect(sf::Vector2f(220, 50), sf::Color::Green, sf::Color::Black, 2, sf::Vector2f(650, 230));
    button3.move(0, 200);

    button1Text = createText(font, 25, sf::Color::Black, "Start Coding", button1.getPosition());
    button1Text.move(20, 10);

    button2Text = createText(font, 25, sf::Color::Black, "Tutorial", button2.getPosition());
    button2Text.move(50, 10);

    button3Text = createText(font, 25, sf::Color::Black, "Exit", button3.getPosition());
    button3Text.move(80, 10);

    // main menu window
    while (true)
    {
        while (menuWindow.isOpen())
        {
            window.setVisible(false);
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (menuWindow.pollEvent(event))
            {
                // "close requested" event: we close the window
                switch (event.type){

                    case sf::Event::Closed:
                        menuWindow.close();
                        return 0;
                        break;

                    case sf::Event::MouseButtonPressed:
                    // find out which button is pressed and do necessary stuff

                        mousePosition = sf::Vector2f(sf::Mouse::getPosition(menuWindow));

                        if (button3.getGlobalBounds().contains(mousePosition))
                        {
                            menuWindow.close();
                            return 0;
                        }

                        if (button1.getGlobalBounds().contains(mousePosition))
                        {
                            // open up coding window, close main window
                            menuWindow.close();
                            window.create(sf::VideoMode(1500, 800), "PseudoCoder", sf::Style::Titlebar|sf::Style::Close);
                        }

                        if (button2.getGlobalBounds().contains(mousePosition))
                        {
                            handleButton2Press(menuWindow);
                        }

                        break;

                    case sf::Event::MouseMoved:
                    // highlight effect on buttons

                        mousePosition = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);

                        if (button1.getGlobalBounds().contains(mousePosition))
                        {
                            button1.setFillColor(sf::Color(50,205,50));
                            button1Text.setColor(sf::Color::White);
                        }
                        else
                        {
                            button1.setFillColor(sf::Color::Green);
                            button1Text.setColor(sf::Color::Black);
                        }

                        if (button2.getGlobalBounds().contains(mousePosition))
                        {
                            button2.setFillColor(sf::Color(50,205,50));
                            button2Text.setColor(sf::Color::White);
                        }
                        else
                        {
                            button2.setFillColor(sf::Color::Green);
                            button2Text.setColor(sf::Color::Black);
                        }

                        if (button3.getGlobalBounds().contains(mousePosition))
                        {
                            button3.setFillColor(sf::Color(50,205,50));
                            button3Text.setColor(sf::Color::White);
                        }
                        else
                        {
                            button3.setFillColor(sf::Color::Green);
                            button3Text.setColor(sf::Color::Black);
                        }

                        break;
                } 
            }

            menuWindow.clear(sf::Color(135,206,250));

            // draw everything
            menuWindow.draw(title);
            menuWindow.draw(button1);
            menuWindow.draw(button2);
            menuWindow.draw(button3);
            menuWindow.draw(button1Text);
            menuWindow.draw(button2Text);
            menuWindow.draw(button3Text);

            menuWindow.display();
        }

        // run the program as long as the window is open
        while (window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                // determine what happened in window and take appropriate action
                    case sf::Event::Closed:
                    // "close requested" event: we close the window

                        window.close();
                        menuWindow.create(sf::VideoMode(1500, 800), "PseudoCoder", sf::Style::Titlebar|sf::Style::Close);
                        break;

                    case sf::Event::MouseButtonPressed:
                    // the mouse is clicked

                        mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));   // find where
                        cursor.x = cursor.y = 0;    // reset cursor
                        userInput.str("");          // reset user input

                        // making buttons blink on click
                        if (save.getGlobalBounds().contains(mousePosition))
                            save.setFillColor(sf::Color::Red);

                        if (viewButton.getGlobalBounds().contains(mousePosition))
                            viewButton.setFillColor(sf::Color::Red);

                        if (deleteAll.getGlobalBounds().contains(mousePosition))
                            deleteAll.setFillColor(sf::Color::Red);

                        if (compileAndRun.getGlobalBounds().contains(mousePosition))
                            compileAndRun.setFillColor(sf::Color::Red);

                        // check if mouse is clicked in right place and any menu is already open
                        for (int i = 0; i < addBlockButtons.size(); i++)
                        {
                            if (addBlockButtons[i].getGlobalBounds().contains(mousePosition) && !(currentMenu))  
                            {
                                blockMenu.setPosition(mousePosition);               // open menu where mouse is clicked
                                if (addBlockButtons[i].getPosition().y > 600)
                                    blockMenu.move(0, -(blockMenu.getSize().y));
                                currentMenu = &blockMenu;                           // blockMenu is open
                                blockID = i;                                        // which addBlockButton was clicked
                                break;
                            }
                            else 
                            {
                                currentMenu = 0;                                    // no menu is open
                            }
                        }

                        // what to do if a menu item is selected
                        for (int i = 0; i < boxes.size(); i++) 
                        {
                            if (boxes[i].getGlobalBounds().contains(mousePosition))
                            {
                                blockName = listOfCommands[i].getString();          // get which item was selected
                                handleMenuItemSelection(blockName, blockID);
                            }
                        }

                        // what to do if user clicks inside a box
                        for (int i = 0; i < textToDisplayParts.size(); i++)
                        {
                            for (int j = 0; j < textToDisplayParts[i].size(); j++)
                            {
                                if (textToDisplayParts[i][j].getGlobalBounds().contains(mousePosition))
                                {
                                    cursor = inputLocations[i][(j-1)/2];            // set cursor on first input location on line
                                }
                            }
                        }

                        // what to do if user clicks on a deleteBlockButton
                        for (int i = 0; i < deleteBlockButtons.size(); i++)
                        {
                            if (deleteBlockButtons[i].getGlobalBounds().contains(mousePosition))
                                handleDeleteButtonPress(i);
                        }

                        // what to do if user clicks on save button
                        if (save.getGlobalBounds().contains(mousePosition))
                        {
                            handleSaveButtonPress(string(saveName.getString()));
                        }

                        // what to do if user clicks on delete all button
                        if (deleteAll.getGlobalBounds().contains(mousePosition))
                        {
                            for (int i = deleteBlockButtons.size()-1; i >= 0; i--)
                                handleDeleteButtonPress(i);
                        }

                        // what to do if user clicks on the textbox
                        if (textBox.getGlobalBounds().contains(mousePosition))
                        {
                            cursor = saveName.getPosition();        // set cursor
                            userInput << string(saveName.getString());      // existing string in the box
                        }

                        // what to do if view button is pressed
                        if (viewButton.getGlobalBounds().contains(mousePosition))
                        {
                            handleViewButtonPress(string(saveName.getString()));
                        }

                        // what to do if compile and run button is pressed
                        if (compileAndRun.getGlobalBounds().contains(mousePosition))
                        {
                            handleCompileButtonPress(string(saveName.getString()));
                        }

                        // user clicks on a input field, set cursor
                        for (int i = 0; i < inputBoxes.size(); i++)
                        {
                            if (inputBoxes[i].getGlobalBounds().contains(mousePosition))
                                cursor = inputBoxes[i].getPosition();
                        }

                        // if cursor is in on an input location
                        for (int i = 0; i < inputLocations.size(); i++)
                        {
                            for (int j = 0; j < inputLocations[i].size(); j++)
                            {
                                if (cursor == inputLocations[i][j])
                                    userInput << string(inputTexts[i][j].getString());      // get existing input
                            }
                        }

                        break;
                       
                    case sf::Event::TextEntered:
                    // if text is entered

                        switch (event.text.unicode) 
                        {
                            case (8):   // backspace
                                if (cursor == saveName.getPosition())
                                {
                                    // in textbox
                                    string str = string(saveName.getString());
                                    userInput.str("");
                                    userInput << str.substr(0, str.length()-1);
                                    saveName.setString(userInput.str());
                                }
                                else if (cursor.x != 0 && cursor.y != 0)
                                {
                                    // in input field
                                    string str = userInput.str();
                                    userInput.str("");
                                    userInput << str.substr(0, str.length()-1);
                                    handleTextEntering(userInput.str());
                                }
                                break;  
                            case (13):  // return
                                break;
                            case (9):   // tab
                                break;
                            default:
                            // set text as user input
                                if (cursor == saveName.getPosition())
                                {
                                    if (userInput.str().length() >= MAX_LEN)
                                        break;
                                    userInput << static_cast<char>(event.text.unicode);
                                    saveName.setString(userInput.str());
                                }
                                else if (cursor.x != 0 && cursor.y != 0)
                                {
                                    if (userInput.str().length() >= MAX_LEN)
                                        break;
                                    userInput << static_cast<char>(event.text.unicode);
                                    handleTextEntering(userInput.str());
                                }
                                break;
                        } 
                        break;

                    case sf::Event::KeyPressed:
                    // if keys are pressed

                        switch (event.key.code)
                        {
                            case (sf::Keyboard::Tab):
                                if (cursor.x != 0 && cursor.y != 0)
                                {
                                    userInput.str("");
                                    moveCursor();
                                    for (int i = 0; i < inputLocations.size(); i++)
                                    {
                                        for (int j = 0; j < inputLocations[i].size(); j++)
                                        {
                                            if (cursor == inputLocations[i][j])
                                                userInput << string(inputTexts[i][j].getString());
                                        }
                                    }
                                }
                                break;
                            // arrow keys used to scroll
                            // controlled scrolling
                            case (sf::Keyboard::Up):
                                if (deleteBlockButtons.size() > 0)
                                {
                                    if (deleteBlockButtons[0].getPosition().y == 25)
                                        break;
                                }
                                else
                                {
                                    if (addBlockButtons[0].getPosition().y == 25)
                                        break;
                                }
                                scroll(0, pushY/2);
                                break;
                            case (sf::Keyboard::Down):
                                if (addBlockButtons[0].getPosition().y <= 225)
                                    break;
                                scroll(0, (-1)*pushY/2);
                                break;
                            case (sf::Keyboard::Left):
                                if (deleteBlockButtons.size() > 0)
                                {
                                    if (deleteBlockButtons[0].getPosition().x == 10)
                                        break;
                                }
                                else
                                {
                                    if (addBlockButtons[0].getPosition().x == 30)
                                        break;
                                }
                                scroll(pushX/2, 0);
                                break;
                            case (sf::Keyboard::Right):
                                scroll((-1)*pushX/2, 0);
                                break;
                        }
                        break;

                    case sf::Event::MouseMoved:
                    // if mouse is moved

                        // highlight appropriate menu item when mouse is moved on it
                        if (currentMenu == &blockMenu) 
                        {
                            for (int i = 0; i < listOfCommands.size(); i++) 
                            {
                            // highlight
                                if (boxes[i].getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                                {
                                    listOfCommands[i].setColor(sf::Color::White);
                                    boxes[i].setFillColor(sf::Color(30, 144, 255));
                                }
                            // unhighlight
                                else 
                                {
                                    listOfCommands[i].setColor(sf::Color::Black);
                                    boxes[i].setFillColor(sf::Color::Transparent);
                                }
                            }
                        }

                        // highlight or unhighlight buttons when mouse is on them
                        if (save.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        {
                            save.setFillColor(sf::Color(50,205,50));
                            saveText.setColor(sf::Color::White);
                        }
                        else
                        {
                            save.setFillColor(sf::Color::Green);
                            saveText.setColor(sf::Color::Black);
                        }

                        if (deleteAll.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        {
                            deleteAll.setFillColor(sf::Color(50,205,50));
                            deleteText.setColor(sf::Color::White);
                        }
                        else
                        {
                            deleteAll.setFillColor(sf::Color::Green);
                            deleteText.setColor(sf::Color::Black);
                        }

                        if (viewButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        {
                            viewButton.setFillColor(sf::Color(50,205,50));
                            view.setColor(sf::Color::White);
                        }
                        else
                        {
                            viewButton.setFillColor(sf::Color::Green);
                            view.setColor(sf::Color::Black);
                        }

                        if (compileAndRun.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        {
                            compileAndRun.setFillColor(sf::Color(50,205,50));
                            CAndR.setColor(sf::Color::White);
                        }
                        else
                        {
                            compileAndRun.setFillColor(sf::Color::Green);
                            CAndR.setColor(sf::Color::Black);
                        }

                        for (int i = 0; i < addBlockButtons.size(); i++)
                        {
                            if (addBlockButtons[i].getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                                addBlockButtons[i].setFillColor(sf::Color(220, 220, 220));
                            else
                                addBlockButtons[i].setFillColor(sf::Color::Transparent);
                        }
                        break;

                    case sf::Event::MouseWheelScrolled:
                    // make window vertically scrollable

                        d = event.mouseWheelScroll.delta;       // +1 or -1 depending on mousewheel up or down
                        // control scrolling up and down
                        if (deleteBlockButtons.size() > 0)
                        {
                            if (d == 1 && deleteBlockButtons[0].getPosition().y == 25)
                                break;
                        }
                        else
                        {
                            if (d == 1 && addBlockButtons[0].getPosition().y == 25)
                                break;
                        }
                        if (d == -1 && addBlockButtons[0].getPosition().y <= 220)
                            break;
                        scroll(0, d*pushY/2);
                        break;

                    default:
                        break;
                }
                    
            }


            // clear the window with weird color
            window.clear(sf::Color(135,206,250));

            // before drawing, always call this to correct user input locations
            updateInputBoxLocations(window);

            // draw all necessary vectors to screen
            drawVectors(window);

            // draw menu only if selected
            if (currentMenu)
                window.draw(*currentMenu);

            // if block menu is open
            if (currentMenu == &blockMenu) 
            {
            // draw all menu items and boxes around the text of menu items
                for (int i = 0; i < listOfCommands.size(); i++) 
                {
                    // set position of where to draw depending on where user clicks
                    listOfCommands[i].setPosition(blockMenu.getPosition().x+3, blockMenu.getPosition().y + 15*i);
                    boxes[i].setPosition(blockMenu.getPosition().x, blockMenu.getPosition().y + 15*i);
                    window.draw(boxes[i]);
                    window.draw(listOfCommands[i]);
                }
            }
            else 
            {
            // move all boxes out of the screen
                for (int i = 0; i < listOfCommands.size(); i++)
                    boxes[i].setPosition(20000, 20000);
            }

            // draw the save button
            window.draw(save);
            window.draw(saveText);
            // draw the delete all button
            window.draw(deleteAll);
            window.draw(deleteText);
            // draw the textbox
            window.draw(textBox);
            window.draw(saveName);
            window.draw(prompt);
            // draw view code button
            window.draw(viewButton);
            window.draw(view);
            // draw compile and run button
            window.draw(compileAndRun);
            window.draw(CAndR);

            if (cursor.x != 0 && cursor.y != 0)
            {
            // user has clicked inside a block or save field
                setCursorBarPosition();
                window.draw(cursorText);
            }

            
            // end the current frame
            window.display();
        }
    }

    return 0;
}