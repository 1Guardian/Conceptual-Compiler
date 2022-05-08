#include "node.h"
#include "Assembler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <set>

/**************************************************************************************************************************
void checkSemantics(node_t* tree, std::ifstream& ifs, std::string name)

    This function serves as the parent of the PreorderChecking Function just because I wanted consistent pointers

**************************************************************************************************************************/
void AssembleCode(node_t* tree, std::ifstream& ifs, std::string name){

    //a.asm file
    //FIXME: CHANGE TO HAVE DYNAMIC NAME
    std::fstream outFile;
    outFile.open(name, std::ios::out);

    //two strings, one to hold the code being generated, and one to hold the ending
    std::string code = "Start: NOOP\n";
    std::string tailEnd = "End: NOOP\nSTOP\n";
    
    //variable to hold temp value appending
    int tempCT = 0;

    //call the assembler
    Assembler(tree, &tempCT, &code, &tailEnd);

    std::cout << "Assembler: Successful Assembly Code Generation!" <<std::endl;;
    //write to file
    for(int i = 0; i < code.size(); i++)
        outFile.put(code[i]);

    for(int i = 0; i < tailEnd.size(); i++)
        outFile.put(tailEnd[i]);

    outFile.close();
}

/**************************************************************************************************************************
void PreorderChecking(node_t* tree, int level, std::set<std::string>* identifiers, std::ifstream& ifs)

    This function is used to check the static semantics of the binary search tree using preOrder traversal

**************************************************************************************************************************/
void Assembler(node_t* tree, int* tempCT, std::string* code, std::string* tailEnd){

    //string for unspooling annoying nested functions
    std::string intermediate ="";

    if(tree == nullptr)
        return;


    //check all children nodes
    for(int i=0; i < 8; i++){

        //check if node is active
        if (tree->children[i].used != false){

            //check if node is nested
            if(tree->children[i].innerNode != nullptr){
                
                Assembler(tree->children[i].innerNode, tempCT, code, tailEnd);
            }

            //else handle token
            else{

                if(/*tree->children[i+1].innerToken.tkIdentifier == IDENT_tk*/true){
                    
                    //checking for NAME /* A, S1 */
                    if(tree->children[i].innerToken.tokenName == "Name"){

                        //add to end of file
                        *tailEnd += tree->children[i+1].innerToken.tokenName + " 0 \n";

                        //add redundant stuff 
                        *code += "LOAD 0\n";
                        *code += "STORE " + tree->children[i+1].innerToken.tokenName + "\n";


                    }

                    //checking for SPOT /* S2 */
                    else if (tree->children[i].innerToken.tokenName == "Spot"){
                        
                        //determine if variable or number
                        if (tree->children[i+1].innerToken.tkIdentifier == IDENT_tk){

                            //Read From screen variable
                            *code += "READ " + tree->children[i+1].innerToken.tokenName + "\n";

                            //add to end of file
                            *tailEnd += tree->children[i+1].innerToken.tokenName + " 0 \n";

                        }
                        else{

                            //load num into acc
                            *code += "LOAD " + tree->children[i+1].innerToken.tokenName + "\n";
                        
                        }

                    }

                    //checking for SHOW /* K, E */
                    else if (tree->children[i].innerToken.tokenName == "Show"){
                        
                        //Write to screen variable
                        *code += "WRITE " + tree->children[i+1].innerToken.tokenName + "\n";

                    }

                    //checking for MOVE /* K */
                    else if (tree->children[i].innerToken.tokenName == "Move"){
                        
                        //Load Value to acc
                        *code += "LOAD " + tree->children[i+1].innerToken.tokenName + "\n";

                    }

                    //checking for FLIP /* L */
                    else if (tree->children[i].innerToken.tokenName == "Flip"){
                        
                        //load value from acc, mult by -1, and store
                        *code += "LOAD " + tree->children[i+1].innerToken.tokenName + "\n";
                        *code += "MULT -1\n";
                        *code += "STORE " + tree->children[i+1].innerToken.tokenName + "\n";
                        *code += "LOAD " + tree->children[i+1].innerToken.tokenName + "\n";

                    }

                    //checking for HERE, There /* G */
                    //THIS IS A LOOP
                    else if (tree->children[i].innerToken.tokenName == "Here"){

                        //increment tempCT
                        *tempCT += 1;

                        //add temp variable in format Temp# to end of file
                        *tailEnd += "Temp" + std::to_string(*tempCT) + " 0 \n";
                        
                        //load tmp to acc and sub to find control bool
                        *code += "LoopST" + std::to_string(*tempCT) + ": NOOP" + "\n";
                        *code += "LOAD Temp" + std::to_string(*tempCT) + "\n";
                        *code += "SUB " + tree->children[i+1].innerToken.tokenName + "\n";
                        *code += "BRZERO LoopEnd" + std::to_string(*tempCT) + "\n";

                        //action to take
                        *code += "WRITE " + tree->children[i+1].innerToken.tokenName + "\n";

                        //increate temp and repeat
                        *code += "LOAD Temp" + std::to_string(*tempCT) + "\n";
                        *code += "ADD 1\n";
                        *code += "STORE Temp" + std::to_string(*tempCT) + "\n";
                        *code += "BR LoopST" + std::to_string(*tempCT) + "\n";

                        //ending of loop
                        *code += "LoopEnd" + std::to_string(*tempCT) + ": NOOP" + "\n";

                    }

                    //checking for Assign /* J */
                    //THIS IS A NEST
                    else if (tree->children[i].innerToken.tokenName == "Assign"){
                        
                        //store what we know will be needed when the function unspools itself
                        intermediate = "";
                        intermediate += "STORE " + tree->children[i+1].innerToken.tokenName + "\n";

                        //increment 'i' position tracker by 2 to get us to the nested portion
                        i += 2;

                        //recurse
                        Assembler(tree->children[i].innerNode, tempCT, code, tailEnd);

                        //write the intermediate code back
                        *code += intermediate;

                    }

                    //checking for Assign /* H */
                    else if (tree->children[i].innerToken.tokenName == "/"){

                        //look ahead in the tree and grab necessary data and write to asm file
                        *code += "LOAD " + tree->children[i+1].innerNode->children[0].innerToken.tokenName + "\n";
                        *code += "SUB 1\n";

                        //increment 'i' position tracker by 2 to get us to the end of this branch
                        i += 2;

                    }

                    //checking for If /* F1 */
                    else if (tree->children[i].innerToken.tokenName == "If"){

                        //increment tempCT
                        *tempCT += 1;

                        //store what we know will be needed when the function unspools itself
                        intermediate = "";
                        bool lessThan = false;

                        //Load variable for manip
                        *code += "LOAD " + tree->children[i+1].innerToken.tokenName + "\n";
                        
                        //look ahead to get operation
                        if(tree->children[i+2].innerNode->children[0].innerToken.tokenName == "<<")
                            lessThan = true;

                        //look ahead to get number
                        //take only one number if only one is present
                        if(tree->children[i+3].innerNode->children[1].innerToken.tokenName == ".")
                            *code += "SUB " + tree->children[i+3].innerNode->children[0].innerToken.tokenName + "\n";

                        //otherwise get ready for math
                        else{

                            //get numbers
                            int first = std::stoi(tree->children[i+3].innerNode->children[0].innerToken.tokenName);
                            int second = std::stoi(tree->children[i+3].innerNode->children[2].innerToken.tokenName);

                            //get operation
                            if(tree->children[i+3].innerNode->children[1].innerNode->children[0].innerToken.tokenName == "+")
                                first = first + second;
                            else if(tree->children[i+3].innerNode->children[1].innerNode->children[0].innerToken.tokenName == "%")
                                first = first / second;
                            else 
                                first = first * second;

                            //add to code
                            *code += "SUB " + std::to_string(first) + "\n";
                        }

                        //add to operation conditional
                        if(lessThan){
                            *code += "BRNEG Enter" + std::to_string(*tempCT) + "\n";
                            *code += "BR Exit" + std::to_string(*tempCT) + "\n";
                            *code += "Enter" + std::to_string(*tempCT) + ": NOOP\n";
                        }

                        else{
                            *code += "BRZPOS Enter" + std::to_string(*tempCT) + "\n";
                            *code += "BR Exit" + std::to_string(*tempCT) + "\n";
                            *code += "Enter" + std::to_string(*tempCT) + ": NOOP\n";
                        }

                        //now nest to get D and add the exit condition to the code
                        Assembler(tree->children[i+4].innerNode, tempCT, code, tailEnd);
                        *code += "Exit" + std::to_string(*tempCT) + ": NOOP\n";

                        //increment 'i' position tracker by 2 to get us to the end of this branch
                        i = 7;

                    }

                    //checking for Do /* F2 */
                    else if (tree->children[i].innerToken.tokenName == "Do"){

                        //increment tempCT
                        *tempCT += 1;

                        //store what we know will be needed when the function unspools itself
                        intermediate = "";
                        bool lessThan = false;
                        int first;
                        int second = 1;
                        int op; //1 = add 2 = mult 3 = div
                        
                        //look ahead to get operation
                        if(tree->children[i+3].innerNode->children[0].innerToken.tokenName == "<<")
                            lessThan = true;

                        //look ahead to get number
                        //take only one number if only one is present
                        if(tree->children[i+4].innerNode->children[1].innerToken.tokenName == ".")
                            first = std::stoi(tree->children[i+4].innerNode->children[0].innerToken.tokenName);

                        //otherwise get ready for math
                        else{

                            //get numbers
                            first = std::stoi(tree->children[i+4].innerNode->children[0].innerToken.tokenName);
                            second = std::stoi(tree->children[i+4].innerNode->children[2].innerToken.tokenName);

                            //get operation
                            if(tree->children[i+4].innerNode->children[1].innerNode->children[0].innerToken.tokenName == "+")
                                op = 1;
                            else if(tree->children[i+4].innerNode->children[1].innerNode->children[0].innerToken.tokenName == "%")
                                op = 2;
                            else 
                                op = 3;
                        }

                        //add to operation conditional
                        if(lessThan){

                            //add temp variable in format Temp# to end of file
                            *tailEnd += "Temp" + std::to_string(*tempCT) + " " + std::to_string(first) + "\n";
                            
                            //load tmp to acc and sub to find control bool
                            *code += "LoopST" + std::to_string(*tempCT) + ": NOOP" + "\n";
                            *code += "LOAD Temp" + std::to_string(*tempCT) + "\n";

                            //check what operation we're taking
                            if (op == 1)
                                *code += "ADD " + std::to_string(second) + "\n";
                            else if (op == 2)
                                *code += "MULT " + std::to_string(second) + "\n";
                            else
                                *code += "DIV " + std::to_string(second) + "\n";
                            
                            *code += "STORE Temp" + std::to_string(*tempCT) + "\n";
                            *code += "BRZPOS LoopEnd" + std::to_string(*tempCT) + "\n";

                            //Do <D> Except D will never happen in this case
                            Assembler(tree->children[i+2].innerNode, tempCT, code, tailEnd);

                            //increate temp and repeat
                            *code += "BR LoopST" + std::to_string(*tempCT) + "\n";

                            //ending of loop
                            *code += "LoopEnd" + std::to_string(*tempCT) + ": NOOP" + "\n";
                        }

                        else{
                            //add temp variable in format Temp# to end of file
                            *tailEnd += "Temp" + std::to_string(*tempCT) + " " + std::to_string(first) + "\n";
                            
                            //load tmp to acc and sub to find control bool
                            *code += "LoopST" + std::to_string(*tempCT) + ": NOOP" + "\n";
                            *code += "LOAD Temp" + std::to_string(*tempCT) + "\n";

                            //check what operation we're taking
                            if (op == 1)
                                *code += "ADD " + std::to_string(second) + "\n";
                            else if (op == 2)
                                *code += "MULT " + std::to_string(second) + "\n";
                            else
                                *code += "DIV " + std::to_string(second) + "\n";
                            
                            *code += "STORE Temp" + std::to_string(*tempCT) + "\n";
                            *code += "BRNEG LoopEnd" + std::to_string(*tempCT) + "\n";

                            //Do <D> (This will literally never stop happening, no matter what)
                            Assembler(tree->children[i+2].innerNode, tempCT, code, tailEnd);

                            //increate temp and repeat
                            *code += "BR LoopST" + std::to_string(*tempCT) + "\n";

                            //ending of loop
                            *code += "LoopEnd" + std::to_string(*tempCT) + ": NOOP" + "\n";
                        }

                        //increment 'i' position tracker by 2 to get us to the end of this branch
                        i = 7;

                    }

                    //using identifier, just check to see if the identifier already exists
                    else {
                        true;//std::cout << "UNIMPLEMENTED: " << tree->children[i-1].innerToken.tokenName <<std::endl;
                    }
                }
            }
        }

        //else
            //break;
            /*safeExit(-1, outFile);*/
    }
}