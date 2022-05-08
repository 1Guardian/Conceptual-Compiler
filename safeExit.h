#ifndef SAFEEXIT_h
#define SAFEEXIT_h

/**************************************************************************************************************************

void safeExit(int error, std::ifstream& ifs)
    
    This function is simply an extended version of the exit() function provided by C. The only reason this exists is my
    compiler relies on the dumping of stdin to a temporary file, and thus, needs this file stream to be A. closed and B.
    removed from the file system to ensure the file contents do not mess up subsiquent runs.

**************************************************************************************************************************/
void safeExit(int error, std::ifstream& ifs);

#endif