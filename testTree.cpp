#include "node.h"
#include "testTree.h"
#include <iostream>
#include <fstream>
#include <set>

/**************************************************************************************************************************
void checkSemantics(node_t* tree, std::ifstream& ifs)

    This function serves as the parent of the PreorderChecking Function just because I wanted consistent pointers

**************************************************************************************************************************/
void checkSemantics(node_t* tree, std::ifstream& ifs){

    //tools needed for quick checking
    std::set<std::string> identifiers;
    std::vector<std::string> identifierList;

    PreorderChecking(tree, 0, &identifiers, &identifierList, ifs);

    std::cout << "Static Semantics Checker: Successful Semantics Check!" << std::endl;
}

/**************************************************************************************************************************
void PreorderChecking(node_t* tree, int level, std::set<std::string>* identifiers, std::ifstream& ifs)

    This function is used to check the static semantics of the binary search tree using preOrder traversal

**************************************************************************************************************************/
void PreorderChecking(node_t* tree, int level, std::set<std::string>* identifiers, std::vector<std::string>* identifierList, std::ifstream& ifs){

    if(tree == nullptr)
        return;

    //check all children nodes
    for(int i=0; i < 8; i++){

        //check if node is active
        if (tree->children[i].used != false){

            //check if node is nested
            if(tree->children[i].innerNode != nullptr){
                PreorderChecking(tree->children[i].innerNode, level+1, identifiers, identifierList, ifs);
            }

            //else handle token
            else{

                if(tree->children[i].innerToken.tkIdentifier == KW_tk){
                    
                    //defining new identifier
                    if(tree->children[i].innerToken.tokenName == "Name" || tree->children[i].innerToken.tokenName == "Spot"){
                        
                        if(tree->children[i+1].innerToken.tkIdentifier == IDENT_tk){

                            //do checking on the list to see if the identifier already exists
                            if (identifiers->find(tree->children[i+1].innerToken.tokenName) == identifiers->end()){

                                //add element if not found 
                                identifiers->insert(tree->children[i+1].innerToken.tokenName);
                                identifierList->push_back(tree->children[i+1].innerToken.tokenName);
                            }

                            else{

                                //if it exists, we have an error. Error out
                                std::cout << "Static Semantics Error (REDEFINING VARIABLE): Indentifier '" + tree->children[i+1].innerToken.tokenName + "' declared twice\n";
                                safeExit(-1, ifs);
                            }
                        }
                    }
                }
                    //using identifier, just check to see if the identifier already exists
                if(tree->children[i].innerToken.tkIdentifier == IDENT_tk){
                    if (identifiers->find(tree->children[i].innerToken.tokenName) == identifiers->end()){

                        std::cout << "Static Semantics Error (UNDEFINED VARIABLE): Indentifier '" + tree->children[i].innerToken.tokenName + "' was never declared before it was used\n";
                        safeExit(-1, ifs);
                    }
                }
            }
        }

        else
            break;
    }
}