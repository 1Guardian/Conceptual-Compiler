#ifndef ASSEMBLER_h
#define ASSEMBLER_h

#include "node.h"
#include "safeExit.h"
#include <set>
#include <vector>

/**************************************************************************************************************************
void checkSemantics(node_t* tree, std::ifstream& ifs)

    This function serves as the parent of the PreorderChecking Function just because I wanted consistent pointers

**************************************************************************************************************************/
void AssembleCode(node_t* tree, std::ifstream& ifs, std::string name);

/**************************************************************************************************************************
void PreorderChecking(node_t* tree, int level, std::set<std::string>* identifiers, std::ifstream& ifs)

    This function is used to check the static semantics of the binary search tree using preOrder traversal

**************************************************************************************************************************/
void Assembler(node_t* tree, int* tempCT, std::string* code, std::string* tailEnd);

#endif