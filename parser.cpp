#include <string>
#include <fstream>
#include <iostream>
#include "node.h"
#include "scanner.h"
#include "token.h"
#include "parser.h"

//global tokens && line counter
token tk;
token peek;
int currentLine = 1;

//array of token types
std::string tokenStrings[] = {"KeywordTk", "IdentifierTk", "NumberTk", "OperatorTk", "DelimiterTk", "EOFTk"}; 

/**************************************************************************************************************************

node_t* S | R | E | A | B | C | D | F | G | T | V | H |J | K | L | W | Z (std::ifstream& ifs)
    
    This group of functions are used to check the development of the program provided to the compiler and ensure that 
    the production rules of the language are not violated. Each function acts as a non terminal with the bodies of the
    functions serving as terminal rules and nonterminal rules. These functions create a tree which is retuned to the 
    calling function when complete. Errors that arise are printed and the compiler is terminated.

**************************************************************************************************************************/
node_t* S(std::ifstream& ifs);
node_t* R(std::ifstream& ifs);
node_t* E(std::ifstream& ifs);
node_t* A(std::ifstream& ifs);
node_t* B(std::ifstream& ifs);
node_t* C(std::ifstream& ifs);
node_t* D(std::ifstream& ifs);
node_t* F(std::ifstream& ifs);
node_t* G(std::ifstream& ifs);
node_t* T(std::ifstream& ifs);
node_t* V(std::ifstream& ifs);
node_t* H(std::ifstream& ifs);
node_t* J(std::ifstream& ifs);
node_t* K(std::ifstream& ifs);
node_t* L(std::ifstream& ifs);
node_t* W(std::ifstream& ifs);
node_t* Z(std::ifstream& ifs);

/**************************************************************************************************************************

bool chkTk(std::string keyword)
    
    This function simply checks if the stored token's value matches what should be expected from the production rule 
    given it's token ID. If it does, true is returned, otherwise false is returned.

**************************************************************************************************************************/
bool chkTk(std::string keyword){
    return keyword == (tk.tokenName);
}

/**************************************************************************************************************************

bool chkD()
    
    This function simply checks if any of the D nonterminal first sets are satisfied. I created the function simply because
    it looked ugly down below in the nonterminal code, and is much more readable up here.

**************************************************************************************************************************/
bool chkD(){
    return (tk.tkIdentifier == 3 && chkTk("/") || /* H */
            tk.tkIdentifier == 0 && chkTk("Assign") ||  /* J */
            tk.tkIdentifier == 0 && chkTk("Spot") || /* K */
            tk.tkIdentifier == 0 && chkTk("Flip") ||  /* L */
            tk.tkIdentifier == 0 && chkTk("Show") || /* E */
            tk.tkIdentifier == 3 && chkTk("{")); /* F */
}

/**************************************************************************************************************************

node_t* getNode(std::string label)
    
    This allocates a new node and labels it according to what is passed in to the function. It then returns the node for use

**************************************************************************************************************************/
node_t* getNode(std::string label){

    //make node and label it
    node_t* node = new node_t;
    node->label = label;

    return node;

}

/**************************************************************************************************************************

node_t* parser(std::ifstream& ifs)
    
    This function kicks off the tree building. It does no real heavy lifting, but calls for the allocation of the first 
    node and then checks to make sure the EOFtk is supplied at the end to print the successful parse message, or error out. 

**************************************************************************************************************************/
node_t* parser(std::ifstream& ifs) {

    //create the root node
    node_t* root;

    //call the parser
    tk = Scanner(ifs, &currentLine);

    //set root
    root = S(ifs);

    //make sure parse was good
    if (tk.tkIdentifier == 5){
        std::cout << "Successful Parse!\n";
        return root;
    }
    
    else {
        std::cout << "ERROR: EOFtk WAS EXPECTED, BUT NEVER FOUND\n"; 
        return root;
    }
}

node_t* S(std::ifstream& ifs){
    

    //make the new node
    node_t* p = getNode("S");

    //check for Name keyword
    if (tk.tkIdentifier == 0 && chkTk("Name")){

        //process 'Name'
        p->setChild(tk);

        tk = Scanner(ifs, &currentLine);

        //check for identifier
        if(tk.tkIdentifier == 1){

            //process identifier
            p->setChild(tk);

            tk = Scanner(ifs, &currentLine);

            //check for Spot keyword
            if (tk.tkIdentifier == 0 && chkTk("Spot")){

                //process 'Spot'
                p->setChild(tk);

                tk = Scanner(ifs, &currentLine);

                if(tk.tkIdentifier == 1){

                    //process identifier
                    p->setChild(tk);

                    tk = Scanner(ifs, &currentLine);

                    //Call R
                    p->setChild(R(ifs));

                    //Call E
                    p->setChild(E(ifs));

                    return p;
                }
                else {
                    std::cout << "ERROR: EXPECTED " << tokenStrings[1] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
                    safeExit(-1, ifs);
                }
            }
            else {
                std::cout << "ERROR: EXPECTED 'Spot' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
                safeExit(-1, ifs);
            }   
        }
        else {
            std::cout << "ERROR: EXPECTED " << tokenStrings[1] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }
    else {
        std::cout << "ERROR: EXPECTED 'Name' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* R(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("R");

    //check for Place keyword
    if (tk.tkIdentifier == 0 && chkTk("Place")){
        
        //process 'Place'
        p->setChild(tk);

        tk = Scanner(ifs, &currentLine);

        //Call A
        p->setChild(A(ifs));

        //Call B
        p->setChild(B(ifs));

        //check for Home keyword
        if (tk.tkIdentifier == 0 && chkTk("Home")){

            //process 'Home'
            p->setChild(tk);

            tk = Scanner(ifs, &currentLine);

            return p;
        }
        else {
            std::cout << "ERROR: EXPECTED 'Home' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }
    else {
        std::cout << "ERROR: EXPECTED 'Place' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* E(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("E");

    //check for Show keyword
    if (tk.tkIdentifier == 0 && chkTk("Show")){
        
        //process 'Show'
        p->setChild(tk);

        tk = Scanner(ifs, &currentLine);

        //check for identifier
        if(tk.tkIdentifier == 1){

            //process identifier
            p->setChild(tk);

            tk = Scanner(ifs, &currentLine);

            return p;
        }
        else {
            std::cout << "ERROR: EXPECTED " << tokenStrings[1] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }
    else {
        std::cout << "ERROR: EXPECTED 'Show' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* A(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("A");

    //check for Name keyword
    if (tk.tkIdentifier == 0 && chkTk("Name")){
        
        //process 'Name'
        p->setChild(tk);

        tk = Scanner(ifs, &currentLine);

        //check for identifier
        if(tk.tkIdentifier == 1){

            //process identifier
            p->setChild(tk);

            tk = Scanner(ifs, &currentLine);

            return p;
        }
        else {
            std::cout << "ERROR: EXPECTED " << tokenStrings[1] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }
    else {
        std::cout << "ERROR: EXPECTED 'Name' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* B(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("B");

    //check for period op
    if (tk.tkIdentifier == 3 && chkTk(".")){
        
        //process period
        p->setChild(tk);

        tk = Scanner(ifs, &currentLine);

        p->setChild(C(ifs));

        //check for identifier
        if (tk.tkIdentifier == 3 && chkTk(".")){

            //process identifier
            p->setChild(tk);

            tk = Scanner(ifs, &currentLine);

            p->setChild(B(ifs));

            return p;
        }
        else {
            std::cout << "ERROR: EXPECTED '.' " << tokenStrings[3] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }

    //predict two non-terminals (AKA checking D)
    else if (chkD()){

        p->setChild(D(ifs));

        p->setChild(B(ifs));

        return p;

    }

    //predict epsillon
    else {
        
        //make an epsillon 
        token ep;
        ep.tokenName = "Empty";

        p->setChild(ep);

        return p;
    }
}

node_t* C(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("C");

    //checking D since it includes F and thus will tell us
    if(tk.tkIdentifier == 3 && chkTk("{")){

        p->setChild(F(ifs));

        return p;
    }

    //checking G
    else if (tk.tkIdentifier == 0 && chkTk("Here")){

        p->setChild(G(ifs));

        return p;
    }
    else {
        std::cout << "ERROR: EXPECTED 'Here' " << tokenStrings[0] << " OR '{' " << tokenStrings[3] <<" AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* G(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("G");

    //checking for keyword here
    if (tk.tkIdentifier == 0 && chkTk("Here")){

        p->setChild(tk);

        tk = Scanner(ifs, &currentLine);

        //check for number
        if (tk.tkIdentifier == 2){

            p->setChild(tk);
            
            tk = Scanner(ifs, &currentLine);

            //check for keyword there
            if (tk.tkIdentifier == 0 && chkTk("There")){

                p->setChild(tk);
                
                tk = Scanner(ifs, &currentLine);

                return p;
            }
            else {
                std::cout << "ERROR: EXPECTED 'There' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
                safeExit(-1, ifs);
            }
        }
        else {
            std::cout << "ERROR: EXPECTED " << tokenStrings[2] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }
    else {
        std::cout << "ERROR: EXPECTED 'Here' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* T(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("T");

    //checking op
    if (tk.tkIdentifier == 3 && chkTk("<<")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        return p;
    }

    //checking op
    else if (tk.tkIdentifier == 3 && chkTk("<-")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        return p;
    }

    else {
        std::cout << "ERROR: EXPECTED '<<' or '<- " << tokenStrings[3] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* D(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("D");

    if (tk.tkIdentifier == 3 && chkTk("/")/* H */){

        p->setChild(H(ifs));

        return p;
    }

    else if (tk.tkIdentifier == 0 && chkTk("Assign")/* J */){

        p->setChild(J(ifs));

        return p;
    }

    else if (tk.tkIdentifier == 0 && chkTk("Spot") || tk.tkIdentifier == 0 && chkTk("Move")/* K */){

        p->setChild(K(ifs));

        return p;
    }

    else if (tk.tkIdentifier == 0 && chkTk("Flip")/* L */){

        p->setChild(L(ifs));

        return p;
    }

    else if (tk.tkIdentifier == 0 && chkTk("Show")/* E */){

        p->setChild(E(ifs));

        return p;
    }

    else if (tk.tkIdentifier == 3 && chkTk("{")/* F */){

        p->setChild(F(ifs));

        return p;
    }
        
    else {
        std::cout << "ERROR: NO COMPATIBLE PRODUCTION CAN BE PRODUCED AT LINE " << tk.lineNumber << " WITH TOKEN '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << ".\n";
        safeExit(-1, ifs);
    }
}

node_t* F(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("F");

    //check for leading brace
    if (tk.tkIdentifier == 3 && chkTk("{")){

        p->setChild(tk);

        tk = Scanner(ifs, &currentLine);
    }

    //checking op
    if (tk.tkIdentifier == 0 && chkTk("If")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        if (tk.tkIdentifier == 1){

            p->setChild(tk);
            
            tk = Scanner(ifs, &currentLine);

            p->setChild(T(ifs));

            p->setChild(W(ifs));

            p->setChild(D(ifs));

            //checking for ending brace
            if (tk.tkIdentifier == 3 && chkTk("}")){

                p->setChild(tk);
                
                tk = Scanner(ifs, &currentLine);

                return p;
            }
            else {
                std::cout << "ERROR: EXPECTED '}' " << tokenStrings[3] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
                safeExit(-1, ifs);
            }
        }
        else {
            std::cout << "ERROR: EXPECTED " << tokenStrings[1] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }

    //checking op
    else if (tk.tkIdentifier == 0 && chkTk("Do")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        if (tk.tkIdentifier == 0 && chkTk("Again")){

            p->setChild(tk);
            
            tk = Scanner(ifs, &currentLine);

            p->setChild(D(ifs));

            p->setChild(T(ifs));

            p->setChild(W(ifs));

            //checking for ending brace
            if (tk.tkIdentifier == 3 && chkTk("}")){

                p->setChild(tk);
                
                tk = Scanner(ifs, &currentLine);

                return p;
            }
            else {
                std::cout << "ERROR: EXPECTED '}' " << tokenStrings[3] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
                safeExit(-1, ifs);
            }
        }
        else {
            std::cout << "ERROR: EXPECTED 'Again' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }
    else {
        std::cout << "ERROR: EXPECTED 'Do' OR 'If' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* V(std::ifstream& ifs){
    //make the new node
    node_t* p = getNode("V");

    //check for valid operator
    if (tk.tkIdentifier == 3 && chkTk("+") || chkTk("%") || chkTk("&")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        return p;
    }
    else {
        std::cout << "ERROR: EXPECTED '+' OR '%' OR '&' " << tokenStrings[3] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* H(std::ifstream& ifs){
    //make the new node
    node_t* p = getNode("H");

    if (tk.tkIdentifier == 3 && chkTk("/")){

        p->setChild(tk);

        tk = Scanner(ifs, &currentLine);

        p->setChild(Z(ifs));

        return p;
    }
    else {
        std::cout << "ERROR: EXPECTED '/' " << tokenStrings[3] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* J(std::ifstream& ifs){
    //make the new node
    node_t* p = getNode("J");

    //check for valid keyword
    if (tk.tkIdentifier == 0 && chkTk("Assign")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        //check for identifier
        if(tk.tkIdentifier == 1){

            //process identifier
            p->setChild(tk);

            tk = Scanner(ifs, &currentLine);

            p->setChild(D(ifs));

            return p;
        }
        else {
            std::cout << "ERROR: EXPECTED " << tokenStrings[1] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }
    else {
        std::cout << "ERROR: EXPECTED 'Assign' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* L(std::ifstream& ifs){
    //make the new node
    node_t* p = getNode("L");

    //check for valid keyword
    if (tk.tkIdentifier == 0 && chkTk("Flip")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        //check for identifier
        if(tk.tkIdentifier == 1){

            //process identifier
            p->setChild(tk);

            tk = Scanner(ifs, &currentLine);

            return p;
        }
        else {
            std::cout << "ERROR: EXPECTED " << tokenStrings[1] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }
    else {
        std::cout << "ERROR: EXPECTED 'Flip' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* K(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("K");

    //checking keyword
    if (tk.tkIdentifier == 0 && chkTk("Spot")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        //check for number
        if (tk.tkIdentifier == 2){

            p->setChild(tk);
            
            tk = Scanner(ifs, &currentLine);

            //checking for keyword
            if (tk.tkIdentifier == 0 && chkTk("Show")){

                p->setChild(tk);
                
                tk = Scanner(ifs, &currentLine);

                //check for number
                if (tk.tkIdentifier == 2){

                    p->setChild(tk);
                    
                    tk = Scanner(ifs, &currentLine);

                    return p; 

                }
            }
            else {
                std::cout << "ERROR: EXPECTED 'Show' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
                safeExit(-1, ifs);
            }
        }
        else {
            std::cout << "ERROR: EXPECTED " << tokenStrings[2] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }

    //checking keyword
    else if (tk.tkIdentifier == 0 && chkTk("Move")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        //check for identifier
        if (tk.tkIdentifier == 1){

            p->setChild(tk);
            
            tk = Scanner(ifs, &currentLine);

            //checking for keyword
            if (tk.tkIdentifier == 0 && chkTk("Show")){

                p->setChild(tk);
                
                tk = Scanner(ifs, &currentLine);

                //check for identifier
                if (tk.tkIdentifier == 1){

                    p->setChild(tk);
                    
                    tk = Scanner(ifs, &currentLine);

                    return p; 

                }
                else {
                    std::cout << "ERROR: EXPECTED " << tokenStrings[1] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
                    safeExit(-1, ifs);
                }
            }
        }
        else {
            std::cout << "ERROR: EXPECTED " << tokenStrings[1] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
            safeExit(-1, ifs);
        }
    }
    else {
        std::cout << "ERROR: EXPECTED 'Move' OR 'Spot' " << tokenStrings[0] << " AT LINE " << tk.lineNumber << " BUT GOT '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << " INSTEAD.\n";
        safeExit(-1, ifs);
    }
}

node_t* W(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("W");

    //check for number
    if (tk.tkIdentifier == 2){

        p->setChild(tk);

        tk = Scanner(ifs, &currentLine);
    }

    //checking op
    if (tk.tkIdentifier == 3 && chkTk(".")){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        return p;
    }

    //checking op
    if (tk.tkIdentifier == 3 && chkTk("+") || chkTk("%") || chkTk("&")){

        p->setChild(V(ifs));

        if (tk.tkIdentifier == 2){

            p->setChild(tk);
            
            tk = Scanner(ifs, &currentLine);

            return p;
        }
    }
    else {
        std::cout << "ERROR: NO COMPATIBLE PRODUCTION CAN BE PRODUCED AT LINE " << tk.lineNumber << " WITH TOKEN '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << ".\n";
        safeExit(-1, ifs);
    }
}

node_t* Z(std::ifstream& ifs){

    //make the new node
    node_t* p = getNode("Z");

    //checking identifier
    if (tk.tkIdentifier == 1){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        return p;
    }

    //checking number
    else if (tk.tkIdentifier == 2){

        p->setChild(tk);
        
        tk = Scanner(ifs, &currentLine);

        return p;
    }

    else {
        std::cout << "ERROR: NO COMPATIBLE PRODUCTION CAN BE PRODUCED AT LINE " << tk.lineNumber << " WITH TOKEN '" << tk.tokenName << "' " << tokenStrings[tk.tkIdentifier] << ".\n";
        safeExit(-1, ifs);
    }
}