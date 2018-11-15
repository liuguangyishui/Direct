#include <iostream>

#ifndef _OUTPUT_H_
#define _OUTPUT_H_
using namespace::std;

//output file name
static string outputFileName("");

void open_outputFile(string);

void close_outputFile();

//print instr without the last index number
void outPut(string, string); 

//print instr with the last index number
void outPut(string, string, int, int);

//print instr that jump to target instr
void outPutJump(string, string);

//print label that use for jump statement
void outPutLabel(string);

#endif
