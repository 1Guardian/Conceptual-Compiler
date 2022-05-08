#ifndef TOKEN_H
#define TOKEN_H

#include <string>

/*******************************************
tokenID:   	just an enum to denote which
			token is being assigned at any
			point and time in the scanner.
*******************************************/
enum tokenID {DEADTK = -1, KW_tk=0, IDENT_tk=1, NUM_tk=2, OP_TK=3, DELIM_TK=4, EOF_TK=5}; 

/*******************************************
token:   	struct that actually defines the 
			attributes of the found tokens,
			and the actual string that
			corresponds to the token.
*******************************************/
struct token{
	std::string tokenName;
	int lineNumber;
	tokenID tkIdentifier = DEADTK;
};

#endif