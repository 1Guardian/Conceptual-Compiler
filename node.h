#ifndef NODE_H
#define NODE_H

#include <string>
#include "token.h"

//Forward declaration of node_t
class node_t;

class innerTokens{
    public: 
        //used flag
        bool used = false;

        //child members to hold the order
        token innerToken;
        node_t* innerNode = nullptr;
};

class node_t{

    public: 

        //nonterminal label for the node
        std::string label;

        //tokens and children
        innerTokens children[8];

        //function to handle setting up the children
        void setChild(token input){
            for (int i = 0; i < 8; i++){

                //check for empty child to set
                if (children[i].used == false){

                    //update variables and used flag
                    children[i].innerToken = input;
                    children[i].used = true;

                    break;
                }
            }
        }

        //function to handle setting up the children
        void setChild(node_t* input){
            for (int i = 0; i < 8; i++){

                //check for empty child to set
                if (children[i].used == false){

                    //update variables and used flag
                    children[i].innerNode = input;
                    children[i].used = true;

                    break;
                }
            }
        }

        //tokens that are contained within the production
        token token1;
        token token2;
        token token3;
        token token4;

        //other nonterminals spawned off the production
        node_t* child1 = nullptr;
        node_t* child2 = nullptr;
        node_t* child3 = nullptr;
        node_t* child4 = nullptr;

        //simple function to print out all tokens added for simplicity
        std::string getTokens(){
            return (token1.tokenName + " " + token2.tokenName + " " + token3.tokenName + " " + token4.tokenName);
        }
};

#endif