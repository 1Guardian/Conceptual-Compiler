#ifndef PARSER_H
#define PARSER_H

#include "node.h"

/**************************************************************************************************************************

bool chkTk(std::string keyword)
    
    This function simply checks if the stored token's value matches what should be expected from the production rule 
    given it's token ID. If it does, true is returned, otherwise false is returned.

**************************************************************************************************************************/
bool chkTk(std::string keyword);

/**************************************************************************************************************************

bool chkD()
    
    This function simply checks if any of the D nonterminal first sets are satisfied. I created the function simply because
    it looked ugly down below in the nonterminal code, and is much more readable up here.

**************************************************************************************************************************/
bool chkD();

/**************************************************************************************************************************

node_t* getNode(std::string label)
    
    This allocates a new node and labels it according to what is passed in to the function. It then returns the node for use

**************************************************************************************************************************/
node_t* getNode(std::string label);

/**************************************************************************************************************************

node_t* parser(std::ifstream& ifs)
    
    This function kicks off the tree building. It does no real heavy lifting, but calls for the allocation of the first 
    node and then checks to make sure the EOFtk is supplied at the end to print the successful parse message, or error out. 

**************************************************************************************************************************/
node_t* parser(std::ifstream& ifs);

#endif