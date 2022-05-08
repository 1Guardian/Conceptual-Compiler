#ifndef FSATABLE_H
#define FSATABLE_H

#include <tuple>  

/**********************************************************************************************
Final State Numbers:    -001: DEADtk (error)
                        1000: Keywordtk
                        1001: Identifiertk
                        1010: Numbertk
                        1100: Operatortk
                        1101: Operatortk (assumed delimiter for future)
                        1110: EOFtk

**********************************************************************************************/

/**********************************************************************************************
FSATable:   lookup table for the scanner. 
            Columns are the characters and
            rows are the various states.

            Coulmn Key:
            (uL = upperletter, lL = lowercase letter, D= digit, D = digit, regex = illegal): 

            Columns are as given 
            W/S, lL, uL, D, '&', '+', '/', '%', '.', '{', '}', '<', '-', EOF, [^\x00-\x7F]+

            Rows are states ranging from 1 to 9. array[0] is state 1, and array[8] is state 9

**********************************************************************************************/
const int FSATable[9][15] = {
    {   0,	 2,	    1,	    3,	    4,	    4,	    4,	    4,	    4,	    5,	    5,	    6,	    -1,	1110, -1},
    {1000,	 1,	    1000,	1000,	1000,	1000,	1000,	1000,	1000,	1000,	1000,	1000,	-1,	1000, -1},
    {1001,	 2,	    2,	    2,	    1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	-1,	1001, -1},
    {1010,	1010,	1010,	3,	    1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	-1,	1010, -1},
    {1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	-1,	1100, -1},
    {1101,	1101,	1101,	1101,	1101,	1101,	1101,	1101,	1101,	1101,	1101,	1101,	-1,	1101, -1},
    {  -1,	-1,	    -1,	    -1,	    -1,	    -1,	    -1,	    -1,	    -1,	    -1,	    -1,	    7,	     8,	-1,	  -1},
    {1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	-1,	1100, -1},
    {1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	1100,	-1,	1100, -1},
};

/**********************************************************************************************
Keywords Table: Simple array of keywords 
                that are present in the
                language this scanner is
                made for. List gets checked 
                when a keywordtk is assigned. 
**********************************************************************************************/
const std::string keywords[] = {"Again", "If", "Assign", "Move", "Show", "Flip", "Name", "Home", "Do", "Spot", "Place", "Here", "There"};

/**********************************************************************************************
Operator Table: This list is just an array
                of tuples that holds the 
                character and the column
                number that it corresponds
                with in the FSATable. Checked
                when an operator char is
                found in the scanner.
**********************************************************************************************/
std::tuple<char, int> operatorColumnNum[] = {std::make_tuple('&', 4), std::make_tuple('+', 5), std::make_tuple('/', 6), 
                                  std::make_tuple('%', 7), std::make_tuple('.', 8), std::make_tuple('{', 9), 
                                  std::make_tuple('}', 10), std::make_tuple('<', 11), std::make_tuple('-', 12)};

#endif