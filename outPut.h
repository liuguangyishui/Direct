#include <iostream>

#ifndef _OUTPUT_H_
#define _OUTPUT_H_
using namespace::std;

//print instr without the last index number
void outPut(string, string); 

//print instr with the last index number
void outPut(string, string, int, int);

//print instr that jump to target instr
void outPutJump(string, string);

//print label that use for jump statement
void outPutLabel(string);

#endif
