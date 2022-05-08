#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include "safeExit.h"

/**************************************************************************************************************************

int getColumn(char input)
    
    This function checks the operator tuple array found in FSATable.h to get the column number of the various operators.
    This was implemented in anticipation of operator separation without string or char comparison needing to be implemented
    when we add actual code generation and preparsing to this module.

**************************************************************************************************************************/
int getColumn(char input);

/**************************************************************************************************************************

int keyWordFilter(std::string input)
    
    This function checks the keyword array found in FSATable.h to determine if a found keywordtk is a valid keyword.

**************************************************************************************************************************/
int keyWordFilter(std::string input);

/**************************************************************************************************************************

token Scanner(std::string input, int lineNum)
    
    This function is the realization of the driver code for the FSATable. It keeps track of the linenumber by updating 
    an integer that is passed to it from the testscanner. In the future, this will be passed from the invoking function
    in the same way. It reads in a character from the ifstream that is passed to it, and then uses std::peek to check
    the next character as the lookahead implemetation. It uses flow control to determine the validity of characters, 
    treating the following if statements as a seive, where any characters that get through are illegal. When a 
    currentState is reached that exceeds 1000, a token is assigned and returned to the calling function. If the 
    currentState is ever -1, the function errors, and returns a dead token to the test scanner, which exits.

**************************************************************************************************************************/
token Scanner(std::ifstream& ifs, int* lineNum);

#endif