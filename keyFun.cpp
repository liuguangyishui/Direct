#include <iostream>
#include <cstring>
#include <regex>
#include <cassert>
#include "keyFun.h"
#include "outPut.h"
#include "regAlloc.h"


using namespace::std;
class regRe;

void changeComma(string &str){
  //  cout << "i have benn call changeComma" << endl;
  for(int i = 0; i < str.size(); i++){
    if(str[i] == ',' || str[i] == ':' || \
       str[i] == '(' || str[i] == ')'){
      str[i] = ' ';
    }
    
  }
}

/*This fun return a value that containe the address
 *of regester. opSrc represents IR's operator like
 *%1, %2 and so on
 */
string getRegValue(string opSrc){
  if(storeMap.find(opSrc) == storeMap.end()){
    cout << "The opSrc no valid value!" << endl;
    return " ";
  }
  string res = storeMap.find(opSrc)->second;
  return res;
}

/*This fun return a actual register form the fun para
 *reserve
 */
string getRegValue_funPara(string opSrc){
  if(indexNumPara >= numPara)
    return " ";
  
  ++indexNumPara;
  string regName("0x" + to_string(indexNumPara) + "H");
  if(indexNumPara >= numPara){
    indexNumPara = 0;
    numPara = 0;
  }
  return regName;
}

string getRegValue_global(string opSrc){
  if(globalMap.find(opSrc) == globalMap.end()){
    cout << "The opSrc no valid value in globalMap!" << endl;
    return " ";
  }
  string res = globalMap.find(opSrc)->second;
  return res;
}

/*the beignning of program
 */
void programBegin(){
  outPut("org", "0x0000");
  outPutJump("goto", "%begin");
  outPutLabel("begin");
}

/*This fun alloca register for the IR's operator %x
 */
void allocaReg(string opDes){
  if(storeMap.find(opDes) == storeMap.end()){
    assert("opDes invalid, allocaReg failed!");
  }
  regRe objAlloc = regRe();
  string regName = objAlloc.allocaReg();
  storeMap[opDes] = regName;
}

string allocaReg_funPara(string opSrc){
  regRe objAlloc = regRe();
  string regName = objAlloc.allocaReg_funPara();
  return regName;
}

void allocaReg_global(string opDes){
  regRe objAlloc = regRe();
  string regName = objAlloc.allocaReg();
  globalMap[opDes] = regName;
}

void tranceLoad(splitWord wordCon){
  // cout << "i have benn called tranceLoad" << endl;
  string opSrc = wordCon.vaCol[5];
  string opDes = wordCon.vaCol[0];
  string regName;
  regex reg1("@.+");
  regex reg2("\%.+");
  if(regex_match(opSrc, reg1)){
    regName = getRegValue_global(opSrc);
  } else if(regex_match(opSrc, reg2)){
    regName = getRegValue(opSrc);
  }

  if(storeMap.find(opDes) == storeMap.end()){
    storeMap[opDes] = regName;
  }
}

void tranceStore(splitWord wordCon){

  string opSrc1 = wordCon.vaCol[2];
  string opSrc2 = wordCon.vaCol[4];

  regex res("\%.*");
  if(regex_match(opSrc1, res)){//Instr: store R to R
    if(numPara == 0) { //no fun parameter

      string regNameSrc1 = getRegValue(opSrc1);//get reg from storeMap
      string regNameSrc2 = getRegValue(opSrc2);
      outPut("movf(l)", regNameSrc1);
      outPut("movwf(s", regNameSrc2);
    } else if(numPara != 0){//deal with fun para
      string regNameSrc1 = getRegValue_funPara(opSrc1);
      string regNameSrc2 = getRegValue(opSrc2);
      outPut("movf(l)", regNameSrc1);
      outPut("movwf(s", regNameSrc2);
    } 
    return;
  } else {    //Instr: store Constant to R
    string regNameSrc = getRegValue(opSrc2);
    outPut("movlw", opSrc1);
    outPut("movwf(s", regNameSrc);
  }
}

void tranceAlloca(splitWord wordCon){
  //cout << "i have been called tranceAlloca" << endl;
  string opDes = wordCon.opCol[0];
  allocaReg(opDes);//alloca actual reg for the IR's

  //cout << getRegValue(opDes) << endl;
  //if(storeMap.find(opDes) == storeMap.end()){
  // storeMap[opDes] = " ";
  //}
}

void tranceAdd(splitWord wordCon){
  
  if(wordCon.opCol.size() <= 2){ //Instr: a = R + Constant or reverse
    string opDes = wordCon.opCol[0];
    string opSrc1 = wordCon.vaCol[5];
    string opSrc2 = wordCon.vaCol[6];
    
    string value1, value2;
    regex reg("\%.*");
    if(regex_match(opSrc1, reg)){
      value1 = getRegValue(opSrc1);
    } else{
      value1 = opSrc1;
    }
    if(regex_match(opSrc2, reg)){  //Instr: a = Constant + R
      value2 = getRegValue(opSrc2);
      outPut("movf(l)", value2);
      outPut("addlw", value1);
      allocaReg(opDes);
      string regName = getRegValue(opDes);
      outPut("movwf(s", regName);
      return; 
    } else {                      //Instr: a = R + Constant
      value2 = opSrc2;
      outPut("movf(l)", value1);
      outPut("addlw", value2);
      allocaReg(opDes);
      string regName = getRegValue(opDes);
      outPut("movwf(s", regName);
    }
  } else if(wordCon.opCol.size() >= 3){ //Instr: a = R + R
    string opDes = wordCon.opCol[0];
    string opSrc1 = wordCon.opCol[1];
    string opSrc2 = wordCon.opCol[2];

    string value1 = getRegValue(opSrc1);
    string value2 = getRegValue(opSrc2);
    allocaReg(opDes);
    string regName = getRegValue(opDes);

    outPut("movf(l)", value1);
    outPut("addwf", value2);
    outPut("movwf(s", regName);
  }
  
}
/*In sub Instr,there is important the oprator order in dirfferent 
 *Instr.like: a = 88 - b; a = b - 88; or a = c - b; and a = b - c;
 *so we should use different method.
 */  
void tranceSub(splitWord wordCon){
 
  if(wordCon.opCol.size() <= 2){  //Instr: a = constant - R or reverse
    string opDes = wordCon.opCol[0];
    string opSrc1 = wordCon.vaCol[5];
    string opSrc2 = wordCon.vaCol[6];
    string value1, value2;
    regex res("\%.*");
    if(regex_match(opSrc1, res)){
      value1 = getRegValue(opSrc1);
    } else {
      value1 = opSrc1;
    } 
    if(regex_match(opSrc2, res)){ //Instr: a = constant - R
      value2 = getRegValue(opSrc2);
      outPut("movf(l)", value2);
      outPut("sublw", opSrc1);
      allocaReg(opDes);
      string regName = getRegValue(opDes);
      outPut("movwf(s", regName);
      return;
    } else {                     //Instr: a = R - constant
      allocaReg(opDes);
      string regName = getRegValue(opDes);
      value2 = opSrc2;
      outPut("movlw", value2);
      outPut("subwf", value1);
      outPut("movwf(s", regName);
    }
  } else if(wordCon.opCol.size() >= 3){ //Instr: a = R - R
    string opDes = wordCon.opCol[0];
    string opSrc1 = wordCon.opCol[1];
    string opSrc2 = wordCon.opCol[2];
    
    allocaReg(opDes);
    string regNameSrc1 = getRegValue(opSrc1);
    string regNameSrc2 = getRegValue(opSrc2);
    string regNameDes = getRegValue(opDes);
    outPut("movf(l)", regNameSrc2);
    outPut("subwf", regNameSrc1);
    outPut("movwf(s", regNameDes);
  }
}

/*because the core havn't campare instr,
 *we comlete this instr via sub instr.
 *when use the sub instr for the two oprator, 
 *check the sigal bit in STATUS register.
 */ 
void tranceFcmp(splitWord wordCon){
  string instrName = wordCon.vaCol[3];
  if(instrName.compare("ogt")){
    tranceSub(wordCon);
    lastInstrName = "ogt";
  } else if(instrName.compare("olt")){
    tranceSub(wordCon);
    lastInstrName = "olt";
  } else if(instrName.compare("ole")){
    tranceSub(wordCon);
    lastInstrName = "ole";
  } else if(instrName.compare("oge")){
    tranceSub(wordCon);
    lastInstrName = "oge";
  }
}
/*The BR instr is for the control statement.
 *decide the condition by testing the 0 bit in STATUS register,
 *btfsc: if the bit is 0, then ignore the next statement;
 *btfss: if the bit is 1, then ignore the next statement
 */
void tranceBr(splitWord wordCon){
  if(wordCon.opCol.size() >= 2){ //Instr: for br %x %x %x
    string instrName = lastInstrName;     //decide by last instrName
    if(instrName.compare("ole")){         //Instr: a <= b
      string opBlock1 = wordCon.vaCol[4];
      string opBlock2 = wordCon.vaCol[6];
      outPut("btfsc", "STATUS", 0, 1);
      outPutJump("bra", opBlock1);
      outPutJump("bra", opBlock2);

    } else if(instrName.compare("oge")){  //Instr: a >= b
      string opBlock1 = wordCon.vaCol[4];
      string opBlock2 = wordCon.vaCol[6];
      outPut("btfss", "STATUS", 0, 1);
      outPutJump("bra", opBlock1);
      outPutJump("bra", opBlock2);

    } else if(instrName.compare("olt")){ //Instr: a < b
      string testName = wordCon.vaCol[2];
      string regNameTest = getRegValue(testName);
      string opBlock1 = wordCon.vaCol[4];
      string opBlock2 = wordCon.vaCol[6];
      outPut("movlw", 0);
      outPut("cpfseq", regNameTest);
      outPut("btfss", "STATUS", 0, 1);
      outPutJump("bra", opBlock2);
      outPutJump("bra", opBlock1);
    
    } else if(instrName.compare("ogt")){ //Instr: a > b
      string testName = wordCon.vaCol[2];
      string regNameTest = getRegValue(testName);
      string opBlock1 = wordCon.vaCol[4];
      string opBlock2 = wordCon.vaCol[6];
      outPut("movlw", 0);
      outPut("cpfseq", regNameTest);
      outPut("btfsc", "STATUS", 0, 1);
      outPutJump("bra", opBlock2);
      outPutJump("bra", opBlock1);
    }
    lastInstrName = " ";
  } else if(wordCon.opCol.size() <= 1){ //Instr: br label %x
    string opBlock = wordCon.opCol[0];
    outPutJump("bra", opBlock);
  }
}

/*print the label as a signal of jump statement
 */
void tranceLabel(splitWord wordCon){
  string orderName = wordCon.vaCol[2];
  outPutLabel(orderName);
}

/*global var 
 */
void tranceGlobal(splitWord wordCon){
  string opDes = wordCon.vaCol[0];
  allocaReg_global(opDes);
  string opSrc = wordCon.vaCol[4];
  string regNameSrc = getRegValue_global(opDes);
  outPut("movlw", opSrc);
  outPut("movwf(s", regNameSrc);   
}

void tranceDefine(splitWord wordCon){

  if(firstFun == true){ //used for main
    firstFun = false;
    outPutJump("bra","%main");
  } 
  
  string opDes = wordCon.vaCol[2];
  int index = opDes.find("(");
  string funName = opDes.substr(1, index-1);
  outPutLabel(funName);  //print fun name label

  numPara = 0; //fun para number
  for(auto elem : wordCon.vaCol){ //How many para 
    if(strcmp(elem.c_str(), "signext") == 0){
      ++numPara;
    }
  }

  if(storeMap.size() > 0){
    storeMap.clear();
  }
 
}

void tranceCall(splitWord wordCon){
  if(find(wordCon.vaCol.begin(), wordCon.vaCol.end(), "signext") != \
     wordCon.vaCol.end()){ //this is fun call
    regex reg("\%.+");
    
    for(int i = 0; i < wordCon.vaCol.size(); i++){
      if(strcmp(wordCon.vaCol[i].c_str(), "signext") == 0){
	if(i+1 >= wordCon.vaCol.size()) return;
	string opSrc = wordCon.vaCol[++i];
	
	if(regex_match(opSrc, reg)){
	  string regName = getRegValue(opSrc); 
	  outPut("movf(l)", regName);
	} else {                     //the parameter is an constant
	  outPut("movlw", opSrc);
	}
	string regName_funPara = allocaReg_funPara(opSrc);
	outPut("movwf", regName_funPara);
      }
    }
    string funName = wordCon.vaCol[4];  //change @funName to %funName
    string subfunName = "%" + funName.substr(1);
    outPutJump("call", subfunName);

    //if this fun have return value
    string opRet = wordCon.vaCol[0];
    if(regex_match(opRet, reg)){  
      storeMap[opRet] = "0x1H"; //all return value store in 0x1H
    }

  }
}

void tranceRet(splitWord wordCon){
  // string opDes = wordCon.opCol[0];
  string opDes = wordCon.vaCol[2];
  regex reg("\%.+");
  if(regex_match(opDes, reg)){ //return a constant or a variable 
    string addr = storeMap.find(opDes)->second;
    outPut("movf(l)", addr);
  } else {
    string opSrc = opDes;
    outPut("movlw", opSrc);
  }
  outPut("movwf", "0x1H");
  
}
