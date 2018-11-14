#include <iostream>
#include <unordered_map>
#include "keyWord.h"

#ifndef _KEYFUN_H_
#define _KEYFUN_H_
using namespace::std;

static unordered_map<string, string> storeMap;
static string lastInstrName(" ");

//clear the comma in the singleLine
void changeComma(string&); 

//return the actual reg address(0x) by virtual name(%x)
//from storeMap
string getRegValue(string);   

//alloca actual reg address(0x) for virtual name(%x)
void allocaReg(string);

//the beginning of program, deal with the enter and inerrupt
void programBegin();

//for load instr
void tranceLoad(splitWord);   

//for store instr
void tranceStore(splitWord);  

//for alloca instr
void tranceAlloca(splitWord); 

//for add instr
void tranceAdd(splitWord);    

//for sub instr
void tranceSub(splitWord);    

//for cmp instr
void tranceFcmp(splitWord);   

//for jump instr
void tranceBr(splitWord);     

//for label instr
void tranceLabel(splitWord);  

//for ret instr
void tranceRet(splitWord);    
#endif
