#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <locale>
#include <iostream>

#include "scanner.h"
#include "FSATable.h"

/**************************************************************************************************************************

int getColumn(char input)
    
    This function checks the operator tuple array found in FSATable.h to get the column number of the various operators.
    This was implemented in anticipation of operator separation without string or char comparison needing to be implemented
    when we add actual code generation and preparsing to this module.

**************************************************************************************************************************/
int getColumn(char input){

    //loop over tuple array to find correct column number
    for(int i =0; i < sizeof(operatorColumnNum)/sizeof(operatorColumnNum[0]); i++){
        if(std::get<0>(operatorColumnNum[i]) == input)
            return std::get<1>(operatorColumnNum[i]);
    }

    //not present 
    return -1;
}

/**************************************************************************************************************************

int keyWordFilter(std::string input)
    
    This function checks the keyword array found in FSATable.h to determine if a found keywordtk is a valid keyword.

**************************************************************************************************************************/
int keyWordFilter(std::string input){

    //check every keyword against the input value
    for(int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++){
        if (input == keywords[i])
            return i;
    }

    //not present
    return -1;
}

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
token Scanner(std::ifstream& ifs, int* lineNum){
    token returnTk;
    char currentChar;
    int currentState = 0;
    int opColumn = 0;
    std::string currentWord = "";
    bool first = true;

    //grab initial char and classify
    ifs.get(currentChar);

    //check if we are going to return an eof tk upon start
    if (ifs.eof()){
        returnTk.tokenName = "EOF_TK";
        returnTk.lineNumber = *lineNum;
        returnTk.tkIdentifier = EOF_TK;
        return returnTk;
    }

	while(currentState < 999 && currentState > -1) {

        //check to see if we're at EOF
        if (ifs.eof()){
            returnTk.tokenName = "EOF_TK";
            returnTk.lineNumber = *lineNum;
            returnTk.tkIdentifier = EOF_TK;
            return returnTk;
        }

        //peek next character
        if(!first)
            currentChar = ifs.peek();

        //check the op column
        opColumn = getColumn(currentChar);

        //deal with ignoring comments
        if(currentChar == '*'){
            ifs.get(currentChar);
            ifs.get(currentChar);
            while (currentChar != '*'){
                ifs.get(currentChar);

                //catch the possibility that the comment was never closed
                if (ifs.eof()){
                    returnTk.tokenName = "EOF_TK";
                    returnTk.lineNumber = *lineNum;
                    returnTk.tkIdentifier = EOF_TK;
                    return returnTk;
                }

                //check to see if we should increase the line number count
                if (currentChar == '\n'){
                    *lineNum += 1;
                }
            }
            ifs.get(currentChar);
        }

        //is the character alphabetic?
        if (std::isalpha(currentChar)){
            
            //get the new row we belong to
            if(isupper(currentChar))
                currentState = FSATable[currentState][2];
            else
                currentState = FSATable[currentState][1];
        }

        //is it a digit?
        else if (std::isdigit(currentChar)){

            //get the new row we belong to
            currentState = FSATable[currentState][3];
            
        }

        //check if character is space
        else if (std::isspace(currentChar) || currentChar == '\n' || currentChar == '\r'){
            
            //get the new row we belong to
            currentState = FSATable[currentState][0];
    
        }

        //if it's an operator get associated column number
        else if (opColumn != -1){

            //get the new row we belong to
            currentState = FSATable[currentState][opColumn];

        }

        //is it an EOF?
        else if (static_cast<int>(currentChar) == -1){

            //get the new row we belong to
            currentState = FSATable[currentState][13];
            
        }

        //if it's none of the above, then it's not allowed, error
        else {
            std::cout << "SCANNER ERROR: Unpermitted character: " << currentChar << " presented in file at line " << *lineNum << std::endl;
            returnTk.tkIdentifier = DEADTK;
            //return returnTk;
            safeExit(-1, ifs);
        }

        //update our current char pos
        if(currentState < 999 && currentState > -1){

            //check to see if we should increase the line number count
            if (currentChar == '\n'){
                *lineNum += 1;
            }
            
            //turn the peek into a move
            if(!first)
                ifs.get(currentChar);

            //only add the character to the token value string if it's not a windows-return, or other space char
            if(currentChar != '\r' && !std::isspace(currentChar) && static_cast<int>(currentChar) != -1)
                currentWord += currentChar;
        }
        
        //set our first flag
        first = false;
    }

    //check if we got a valid state or if we got an error
    if(currentState > 999){
        switch(currentState){
            case 1000:
                //check the keyword for existing
                if(keyWordFilter(currentWord) > -1){
                    returnTk.tokenName = currentWord;
                    returnTk.lineNumber = *lineNum;
                    returnTk.tkIdentifier = KW_tk;
                }
                else{
                    std::cout << "SCANNER ERROR: Keyword Token Detected: \"" << currentWord << "\", But No Matching Keyword Found at Line: "<< *lineNum << std::endl;
                    returnTk.tkIdentifier = DEADTK;
                    //return returnTk;
                    safeExit(-1, ifs);
                }
                break;
            case 1001:
                if(currentWord.length() > 1){
                    returnTk.tokenName = currentWord;
                    returnTk.lineNumber = *lineNum;
                    returnTk.tkIdentifier = IDENT_tk;
                }
                else{
                    std::cout << "SCANNER ERROR: Identifier Token Detected: \"" << currentWord << "\", at Line: "<< *lineNum <<" But Identifiers Must Have More Than 1 Character In String." << std::endl;
                    returnTk.tkIdentifier = DEADTK;
                    //return returnTk;
                    safeExit(-1, ifs);
                }
                break;
            case 1010:
                returnTk.tokenName = currentWord;
                returnTk.lineNumber = *lineNum;
                returnTk.tkIdentifier = NUM_tk;
                break;
            case 1100:
                returnTk.tokenName = currentWord;
                returnTk.lineNumber = *lineNum;
                returnTk.tkIdentifier = OP_TK;
                break;
            case 1101:
                returnTk.tokenName = currentWord;
                returnTk.lineNumber = *lineNum;
                returnTk.tkIdentifier = OP_TK;
                break;
            case 1110:
                returnTk.tokenName = "EOF_TK";
                returnTk.lineNumber = *lineNum;
                returnTk.tkIdentifier = EOF_TK;
                break;
        }
    }
    else {
            std::cout << "SCANNER ERROR: Invalid character combination: " << currentWord << " present in file at line " << *lineNum << ". No token matches this string. " << std::endl;
            returnTk.tkIdentifier = DEADTK;
            //return returnTk;
            safeExit(-1, ifs);
        }

    return returnTk;

}