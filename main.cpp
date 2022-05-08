/******************************************************************************

CS 4280

Tyler Martin

Project 4

*******************************************************************************/
#include <regex.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <set>

#include "testTree.h"
#include "parser.h"
#include "Assembler.h"
#include "node.h"

/**************************************************************************************************************************
void takeInput(std::string* input)

    This function is used to take in the user input if a file is not provided when the program starts running.
    It uses a simple while loop that stops when EOF (ctrl+D for *NIX or ctrl+z for windows) is input. It appends
    each character to the end of the string provided by main.

**************************************************************************************************************************/
void takeInput(std::string* input){
    
    //char we will need to take in input and string for ss-abuse
    char inputChar;
    
    //while loop to take in characters until Ctrl+D is pressed (also blocks me from using std::cin... yuck)
    while((inputChar = getchar()) != EOF){
        input->push_back(inputChar);
    }
}

/**************************************************************************************************************************
void peekEmptyFile(std::ifstream& ifs)

    This function simply checks to see if the generated temp file or the passed file is completely empty.
    returns true if the file is empty, and false if the file is not *strictly* empty. This includes whitespace.

**************************************************************************************************************************/
bool peekEmptyFile(std::ifstream& ifs){
    return ifs.peek() == EOF;
}

int main(int argc, char *argv[])
{   
    //string for taking input and stdin check boolean
    bool checkAtty = true;
    std::string execPrefix = "out";
    std::string filename = "a.asm";

    //variables connected to using files
    std::fstream tmpFile;
    tmpFile.open("tmpFile.txt", std::ios::out | std::ios::app);
    std::string input = "";
    int line = 0;
    std::ifstream ifs;

    //process all command line arguments (file input)
    if(argc > 2){
        std::cout << "Fatal: Improper usage \n Usage: P0 [filename] \n or \n Usage: P0 < [filename] \n or \n Usage: P0" << std::endl;
        return -1;
    }
    else if (argc == 2) {

        //make sure input file wasn't special hidden file
        if (argv[1][0] != '.'){

            //add name to outputname
            filename = "";
            for (int i = 0; argv[1][i] != '.' && i < std::string(argv[1]).size(); i++){
                filename.push_back(argv[1][i]);
            }
            filename += ".asm";
        }

        //read in from parameter passed file
        ifs.open(argv[1]);

        //close tempfile
        tmpFile.close();
    }
    else{

        //checking if a file was provided to stdin
        if(checkAtty){
            if (isatty(fileno(stdin))) {
                //take input if no file was provided
                std::cout << "No Input File Detected: Enter the *lowercase* text you want to build trees with (only alphabet characters).\n Press (CTRL + D) when you're done" << std::endl;
                takeInput(&input);
                std::cout << std::endl;
            }
            else{
                //Taking file stdin as input
                takeInput(&input);
            }
        }

        //writing either version of stdin to a temporary file
        for(int i = 0; i < input.size(); i++)
            tmpFile.put(input[i]);

        tmpFile.close();

        //open the tmp file
        ifs.open("tmpFile.txt");
    }

    //check to see if the file is empty
    if (peekEmptyFile(ifs)){
        std::cout << "Fatal Error: The file or input supplied is empty. The program will now terminate.\n";
        ifs.close();
        remove("tmpFile.txt");
        return 1;
    }

    //run the testScan
    node_t* tree = parser(ifs);

    //check Semantics
    checkSemantics(tree, ifs);

    //write the output assembly
    AssembleCode(tree, ifs, filename);

    //remove the tmpfile when done
    ifs.close();
    remove("tmpFile.txt");

    return 0;
}