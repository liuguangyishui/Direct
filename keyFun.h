#include <iostream>
#include <unordered_map>
#include "keyWord.h"

#ifndef _KEYFUN_H_
#define _KEYFUN_H_
using namespace::std;

/*storeMap stores the map between the virtual reg 
 *like %x and acutal reg like 0xAH
 */
static unordered_map<string, string> storeMap; 

/* globalMap is store the map between the virtual
 * reg like %x and acutal reg like 0x of the global
 * var. The difference of the storeMap and globalMap
 * is valitabal. storeMap will be cleaned when run out
 * of a fun. But globalMap will be exitance forever.
 */
static unordered_map<string, string> globalMap;

/*During the transform of some statement like the 
 *control statement in tranceBr fun, we should use 
 *the last instr infomation
 */
static string lastInstrName(" ");

/*record how many parameter have been deliver.
 *for the fun getRegValue_funPara
 */
static int indexNumPara = 0;

/*record a fun have how many parameter
 */
static int numPara = 0;

//clear the comma in the singleLine
void changeComma(string&); 

//return the actual reg address(0x) by virtual name(%x)
//from storeMap
string getRegValue(string);   

//alloca actual reg address(0x) for virtual name(%x)
void allocaReg(string);

string allocaReg_funPara(string);

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

//for global var
void tranceGlobal(splitWord);

//for fun 
void tranceDefine(splitWord);

//for call
void tranceCall(splitWord);

//for ret instr
void tranceRet(splitWord);    
#endif
