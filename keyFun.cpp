#include <iostream>
#include <cstring>
#include <regex>
#include <cassert>
#include "keyFun.h"
#include "outPut.h"
#include "regAlloc.h"
#include "dataArea.h"


using namespace::std;
class regRe;
class ManageDataArea;

void changeComma(string &str){
//  cout << "i have benn call changeComma" << endl;
    for(int i = 0; i < str.size(); i++){
    if(str[i] == ',' || str[i] == ':' ||		\
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
  string regName("0x" + to_string(indexNumPara));// + "H");
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
  outPutOrg("org", "0x0000");
  outPutJump("bra", "Preprocess");
  //outPutJump("bra", "begin");
  outPutOrg("org", "0x0008");
  outPutJump("bra", "Hint");
  //  outPutLabel("begin");
  outPutOrg("org", "0x0020");
  outPutLabel("Preprocess");
}

/*the interrupt of the program
 */
void programHint(){
  outPutLabel("Hint");
  outPutOrg("retfie", "1");
  outPutOrg("end", " ");
}

/*This fun alloca register for the IR's operator %x
 */
void allocaReg(string opDes){
  //assert(!(storeMap.find(opDes) == storeMap.end()));
  regRe objAlloc = regRe();
  string regName = objAlloc.allocaReg();
  
  create_Op_In_StoreMap(opDes, regName);
}

/*after alloca a real reg for operator, then store there map
 *relation in StoreMap
 */
void create_Op_In_StoreMap(string opName, string regName){
  if(storeMap.find(opName) != storeMap.end())
    return;
    storeMap.insert(make_pair(opName, regName));
  //storeMap.insert(pair<string, string>(opName, regName));
  //  storeMap[opName] = regName;
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

void coreAdd_And(splitWord wordCon, string namewf, string namelw){
  if(wordCon.opCol.size() <= 2){ //Instr: a = R + Constant or reverse
    string opDes = wordCon.opCol[0];
    string opSrc1 = wordCon.vaCol[4];
    string opSrc2 = wordCon.vaCol[5];
    string value1, value2;
    regex reg("\%.*");
    if(regex_match(opSrc1, reg)){
      value1 = getRegValue(opSrc1);
    } else{
      value1 = opSrc1;
    }
    if(regex_match(opSrc2, reg)){  //Instr: a = Constant + R
      value2 = getRegValue(opSrc2);
      outPut("movf", value2);
      outPut(namelw, value1);
      allocaReg(opDes);
      string regName = getRegValue(opDes);
      outPut("movwf", regName);
      return; 
    } else {                      //Instr: a = R + Constant
      if(!opSrc2.compare("-1") && !namelw.compare("xorlw")){
	//outPut("movf", value1);
	outPut("comf", value1); //Instr: ~A
	allocaReg(opDes);
	string regName = getRegValue(opDes);
	outPut("movwf", regName);
      } else if(!opSrc2.compare("true") && !namelw.compare("xorlw")){
	allocaReg(opDes);              //Instr: B = !A
	outPut("movlw", "1");
	outPut(namelw, getRegValue(opSrc1));
	outPut("movwf", getRegValue(opDes));
      } else {
	value2 = opSrc2;
	outPut("movf", value1);
	outPut(namelw, value2);
	allocaReg(opDes);
	string regName = getRegValue(opDes);
	outPut("movwf", regName);
      }
    }
  } else if(wordCon.opCol.size() >= 3){ //Instr: a = R + R
    string opDes = wordCon.opCol[0];
    string opSrc1 = wordCon.opCol[1];
    string opSrc2 = wordCon.opCol[2];

    string value1 = getRegValue(opSrc1);
    string value2 = getRegValue(opSrc2);
    allocaReg(opDes);
    string regName = getRegValue(opDes);

    outPut("movf", value1);
    outPut(namewf, value2);
    outPut("movwf", regName);
  }
}



void tranceLoad(splitWord wordCon){

  if(find(wordCon.vaCol.begin(), wordCon.vaCol.end(), "getelementptr") != wordCon.vaCol.end()){ //get element from data from
    
    string opDes = wordCon.vaCol[0];
    allocaReg(opDes); //aloca acutal register
    string regName = getRegValue(opDes); //get the actual reg of opDes
    string strNum = wordCon.vaCol[17];
   
    int num = getNum(strNum);
  
    ManageDataArea *MDA = ManageDataArea::getInstance();
  
    string addr = MDA->getValAddrInMap(wordCon.vaCol[13]);
  
    MDA->loadValue(addr, num);
  
    outPut("movf", "tablat");
    outPut("movwf", regName);
    
    
  } else { //get element from register

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
    create_Op_In_StoreMap(opDes, regName);
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
      outPut("movf", regNameSrc1);
      outPut("movwf", regNameSrc2);
    } else if(numPara != 0){//deal with fun para
      string regNameSrc1 = getRegValue_funPara(opSrc1);
      string regNameSrc2 = getRegValue(opSrc2);
      outPut("movf", regNameSrc1);
      outPut("movwf", regNameSrc2);
    } 
    return;
  } else {    //Instr: store Constant to R
    string regNameSrc = getRegValue(opSrc2);
    outPut("movlw", opSrc1);
    outPut("movwf", regNameSrc);
  }
}

void tranceAlloca(splitWord wordCon){
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
      outPut("movf", value2);
      outPut("addlw", value1);
      allocaReg(opDes);
      string regName = getRegValue(opDes);
      outPut("movwf", regName);
      return; 
    } else {                      //Instr: a = R + Constant
      if(!opSrc2.compare("-1")){   //for descrement. change add to sub
	outPut("movf", value1);
	outPut("sublw", "1");
	allocaReg(opDes);
	string regName = getRegValue(opDes);
	outPut("movwf", regName);
      } else {
	value2 = opSrc2;
	outPut("movf", value1);
	outPut("addlw", value2);
	allocaReg(opDes);
	string regName = getRegValue(opDes);
	outPut("movwf", regName);
      }
    }
  } else if(wordCon.opCol.size() >= 3){ //Instr: a = R + R
    string opDes = wordCon.opCol[0];
    string opSrc1 = wordCon.opCol[1];
    string opSrc2 = wordCon.opCol[2];

    string value1 = getRegValue(opSrc1);
    string value2 = getRegValue(opSrc2);
    allocaReg(opDes);
    string regName = getRegValue(opDes);

    outPut("movf", value1);
    outPut("addwf", value2);
    outPut("movwf", regName);
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
      outPut("movf", value2);
      outPut("sublw", opSrc1);
      allocaReg(opDes);
      string regName = getRegValue(opDes);
      outPut("movwf", regName);
      return;
    } else {                     //Instr: a = R - constant
      allocaReg(opDes);
      string regName = getRegValue(opDes);
      value2 = opSrc2;
      outPut("movlw", value2);
      outPut("subwf", value1);
      outPut("movwf", regName);
    }
  } else if(wordCon.opCol.size() >= 3){ //Instr: a = R - R
    string opDes = wordCon.opCol[0];
    string opSrc1 = wordCon.opCol[1];
    string opSrc2 = wordCon.opCol[2];
    
    allocaReg(opDes);
    string regNameSrc1 = getRegValue(opSrc1);
    string regNameSrc2 = getRegValue(opSrc2);
    string regNameDes = getRegValue(opDes);
    outPut("movf", regNameSrc2);
    outPut("subwf", regNameSrc1);
    outPut("movwf", regNameDes);
  }
}

/*because the core havn't campare instr,
 *we comlete this instr via sub instr.
 *when use the sub instr for the two oprator, 
 *check the sigal bit in STATUS register.
 */ 
void tranceFcmp(splitWord wordCon){
  string instrName = wordCon.vaCol[3];
  if(!instrName.compare("sgt") || !instrName.compare("ogt")){
    tranceSub(wordCon);
    lastInstrName = "gt";
  } else if(!instrName.compare("slt") || !instrName.compare("olt")){
    tranceSub(wordCon);
    lastInstrName = "lt";
  } else if(!instrName.compare("sle") || !instrName.compare("ole")){ 
    tranceSub(wordCon);
    lastInstrName = "le";
  } else if(!instrName.compare("sge") || !instrName.compare("oge")){
    tranceSub(wordCon);
    lastInstrName = "ge";
  } else if(!instrName.compare("ne") || !instrName.compare("one")){
    tranceSub(wordCon);
    lastInstrName = "ne";
  } else if(!instrName.compare("eq") || !instrName.compare("oeq")){
    tranceSub(wordCon);
    lastInstrName = "eq";
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
    
    string beforeOpBlock1 = wordCon.vaCol[4];
    string beforeOpBlock2 = wordCon.vaCol[6];
  
    allocaReg(beforeOpBlock1);
    allocaReg(beforeOpBlock2);
    outPut("movlw", "0");
    outPut("movwf", getRegValue(beforeOpBlock1));
    outPut("movwf", getRegValue(beforeOpBlock2));


    string opBlock1 = wordCon.vaCol[4].substr(1);
    string opBlock2 = wordCon.vaCol[6].substr(1);
    
    if(!instrName.compare("le")){         //Instr: a <= b
      outPut("btfsc", "STATUS", 0, 1);
      outPutJump("bra", opBlock1);
      outPutJump("bra", opBlock2);
    } else if(!instrName.compare("ge")){  //Instr: a >= b
      outPut("btfss", "STATUS", 0, 1);
      outPutJump("bra", opBlock1);
      outPutJump("bra", opBlock2);

    } else if(!instrName.compare("lt")){ //Instr: a < b
      string testName = wordCon.vaCol[2];
      string regNameTest = getRegValue(testName);
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest);
      outPut("btfss", "STATUS", 0, 1);
      outPutJump("bra", opBlock2);
      outPutJump("bra", opBlock1);
    
    } else if(!instrName.compare("gt")){ //Instr: a > b
      string testName = wordCon.vaCol[2];
      string regNameTest = getRegValue(testName);
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest);
      outPut("btfsc", "STATUS", 0, 1);
      outPutJump("bra", opBlock2);
      outPutJump("bra", opBlock1);
    } else if(!instrName.compare("ne")){ //Instr: a != b ?
      string testName = wordCon.vaCol[2];
      string regNameTest = getRegValue(testName);
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest);
      outPutJump("bra", opBlock1);
      outPutJump("bra", opBlock2);
    } else if(!instrName.compare("eq")){ //Instr: a == b?
      string testName = wordCon.vaCol[2];
      string regNameTest = getRegValue(testName);
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest);
      outPutJump("bra", opBlock2);
      outPutJump("bra", opBlock1);
    }
    lastInstrName = " ";
  } else if(wordCon.opCol.size() <= 1){ //Instr: br label %x
    string opBlock = wordCon.opCol[0].substr(1);
    outPutJump("bra", opBlock);
  }
}

/*  
 */
void tranceZext(splitWord wordCon){
    string opDes = wordCon.opCol[0];
    string opSrc1 = wordCon.vaCol[4];
    string regNameTest = getRegValue(opSrc1);
    allocaReg(opDes);
    string regName = getRegValue(opDes);
    string instrName = lastInstrName;

    if(!instrName.compare("eq")){//regNam=(regNameTest==0)?1:0;
      outPut("movlw", "1");                
      outPut("movwf", regName);          
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest); 
      outPut("movwf", regName);
    } else if(!instrName.compare("ne")){//regName=(regNameTest==0)?0:1;
      outPut("movlw", "1");
      outPut("movwf", regName);
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest);
      outPutJump("bra", "next");
      outPut("movwf", regName);
      outPutLabel("next");
    } else if(!instrName.compare("gt")){ //Instr: (a > b)
      outPut("movlw", "1");
      outPut("movwf", regName);
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest);
      outPut("btfsc", "STATUS", 0, 1);
      outPut("movwf", regName);   
    } else if(!instrName.compare("lt")){ //Instr: (a < b)
      outPut("movlw", "1");
      outPut("movwf", regName);
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest);
      outPut("btfss", "STATUS", 0, 1);
      outPut("movwf", regName);
    } else if(!instrName.compare("ge")){ //Instr: (a >= b)
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest);
      outPut("btfss", "STATUS", 0, 1);
      outPutJump("bra", "next2");
      outPutJump("bra", "next1");
      outPutLabel("next1");
      outPut("movlw", "0");
      outPut("movwf", regName);
      outPutJump("bra", "next3");
      outPutLabel("next2");
      outPut("movlw", "1");
      outPut("movwf", regName);
      outPutLabel("next3");
    } else if(!instrName.compare("le")){ //Instr: (a <= b)
      outPut("movlw", "0");
      outPut("cpfseq", regNameTest);
      outPut("btfsc", "STATUS", 0, 1);
      outPutJump("bra", "next2");
      outPutJump("bra", "next1");
      outPutLabel("next1");
      outPut("movlw", "0");
      outPut("movwf", regName);
      outPutJump("bra", "next3");
      outPutLabel("next2");
      outPut("movlw", "1");
      outPut("movwf", regName);
      outPutLabel("next3");
    }
    lastInstrName = " ";
}

/*print the label as a signal of jump statement
 */
void tranceLabel(splitWord wordCon){
  string labelName = wordCon.vaCol[2];
  outPutLabel(labelName);
  
  labelName = "%" + labelName;
  if(getRegValue(labelName).compare(" ")){
    string regName = getRegValue(labelName);
    outPut("movlw", "1");
    outPut("movwf", regName);
  }
 
}

/*global var 
 */
void tranceGlobal(splitWord wordCon){
  string opDes = wordCon.vaCol[0];
  allocaReg_global(opDes);
  string opSrc = wordCon.vaCol[4];
  string regNameSrc = getRegValue_global(opDes);
  outPut("movlw", opSrc);
  outPut("movwf", regNameSrc);   
}

void tranceDefine(splitWord wordCon){

  if(firstFun == true){ //used for main
    firstFun = false;
    outPutJump("bra", "begin");
    outPutLabel("begin");
    outPutJump("bra","main");
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
	  outPut("movf", regName);
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
      storeMap[opRet] = "0x1"; //all return value store in 0x1H
    }

  }
}

/* and operator
 */
void tranceAnd(splitWord wordCon){
  coreAdd_And(wordCon, "andwf", "andlw");
}

/* or operator
 */
void tranceOr(splitWord wordCon){
  coreAdd_And(wordCon, "iorwf", "iorlw");
}

/* xor operator
 */
void tranceXor(splitWord wordCon){
  coreAdd_And(wordCon, "xorwf", "xorlw");
}

/* << operator
 */
void tranceShl(splitWord wordCon){
  coreAdd_And(wordCon, "rlncf", "rlncf");
}

/* >> operator
 */
void tranceAshr(splitWord wordCon){
  coreAdd_And(wordCon, "rrncf", "rrncf");
}

/*&& and || operator
 */
void trancePhi(splitWord wordCon){
  string index_op = wordCon.vaCol[5]; 
  string opDes = wordCon.vaCol[0];
  allocaReg(opDes);
  string regDes = getRegValue(opDes);
  string opSrc1 = wordCon.vaCol[9];
  string opSrc2 = wordCon.vaCol[10];
    
  if(!index_op.compare("true")){ //Instr: for || operator
    regex reg("\%.+");
    if(regex_match(opSrc1, reg)){
      string regOpSrc1 = getRegValue(opSrc1);
      string regOpSrc2 = getRegValue(opSrc2);
      outPut("movlw", "0");
      outPut("cpfseq", regOpSrc2);
      outPutJump("bra", "next1");
      outPut("cpfseq", regOpSrc1);
      outPutJump("bra", "next1");
      outPutJump("bra", "next2");
      
      outPutLabel("next1");
      outPut("movlw", "1");
      outPut("movwf", regDes);
      outPutJump("bra", "next3");
      outPutLabel("next2");
      outPut("movlw", "0");
      outPut("movwf", regDes);
      outPutLabel("next3");
    } else {
      if(!opSrc1.compare("true")){
	outPut("movlw", "1");
	outPut("movwf", regDes);
      } else if(!opSrc2.compare("false")){
	outPut("movlw", "0");
	outPut("cpfseq", getRegValue(opSrc2));
	outPutJump("bra", "next1");
	outPut("movwf", regDes);
	outPutJump("bra", "next2");
	outPutLabel("next1");
	outPut("movlw", "1");
	outPut("movwf", regDes);
	outPutLabel("next2");
      }

    }
  } else if(!index_op.compare("false")){ //Instr: for && operator
    regex reg("\%.+");
    if(regex_match(opSrc1, reg)){
      string regOpSrc1 = getRegValue(opSrc1);
      string regOpSrc2 = getRegValue(opSrc2);
      outPut("movlw", "1");
      outPut("movwf", regDes);
      
      outPut("movlw", "0");
      outPut("cpfseq", regOpSrc1);
      outPutJump("bra", "next1");
      outPutJump("bra", "next2");
      outPutLabel("next1");
      outPut("cpfseq", regOpSrc2);      
      outPutJump("bra", "next3");
      outPutJump("bra", "next2");

      outPutLabel("next2");
      outPut("movlw", "0");
      outPut("movwf", regDes);
      outPutLabel("next3");
    
    } else {
      if(!opSrc1.compare("false")){
	outPut("movlw", "0");
	outPut("movwf", regDes);
      } else if(!opSrc1.compare("true")){
	outPut("movlw", "0");
	outPut("cpfseq", getRegValue(opSrc2));
	outPutJump("bra", "next1");
	outPut("movwf", regDes);
	outPutJump("bra", "next2");
	outPutLabel("next1");
	outPut("movlw", "1");
	outPut("movwf", regDes);
        outPutLabel("next2");
      }
    }
    
  }

}

/*Instr for array.
 *there are two places to store the element of array
 *the first place is the register
 *the other is the data area which in the programmer area
 *
 */
void tranceConstant(splitWord wordCon){
#define  VEC wordCon.vaCol
  //inter constant array and global constant array
  if((find(VEC.begin(), VEC.end(), "internal") != VEC.end()) &&	           (find(VEC.begin(), VEC.end(), "x") != VEC.end())  
    ||
    ((find(VEC.begin(), VEC.end(), "constant") != VEC.end()) && 
    (VEC[1] == "=") && (VEC[4] == "x"))){

    int arrayElemNum = getArrayElemNum(wordCon);
    ManageDataArea *MDA = ManageDataArea::getInstance();
    //store the begin addr
    MDA->createMap(wordCon.vaCol[0], arrayElemNum ); 
    vector<string> dataVec;
    
    //absort data from wordCon
    getDataFromInstr(dataVec, wordCon);
    
    if(dataVec.size() != 0){
      for(auto elem: dataVec){   //store data to data area
        MDA->storeValue(elem);
      }
    }

  }else if(find(wordCon.vaCol.begin(), wordCon.vaCol.end(), 
		"unnamed_addr") != wordCon.vaCol.end()){
    
  }else if(find(wordCon.vaCol.begin(), wordCon.vaCol.end(), "x") 
	   != wordCon.vaCol.end()){ //for global costant array
  /*
    int arrayElemNum = getArrayElemNum(wordCon);

    ManageDataArea *MDA = ManageDataArea::getInstance();
    //store the begin addr
    MDA->createMap(wordCon.vaCol[0], arrayElemNum ); 

    vector<string> dataVec;

    getDataFromInstr(dataVec, wordCon); //absort data from array instr


    if(dataVec.size() != 0){
      for(auto elem: dataVec){   //store data to data area
        MDA->storeValue(elem);
      }
      }
  */
  } else { //for constant val
    
  }
#undef VEC 
}

void getDataFromInstr(vector<string> &dataVec, splitWord wordCon){
  //typedef wordCon.vaCol vec;
  auto index = find(wordCon.vaCol.begin(), wordCon.vaCol.end(), "x");
  int num = getArrayElemNum(wordCon);
  index += 2;
  
  if((*index)[0] == 'c'){ //char array
  string data = (*(index)).substr(2);
    for(int i = 0; i < data.size()-1; i++){
      dataVec.push_back(to_string(data[i]));
    }
  } else {   //int array

    index += 1;
    for(int i = 0; i < num; i++){
      if(i == num - 1){ //remove ']' in 8]
	string val = *index;
	int valSize = val.size();
	val = val.substr(0, valSize - 1);
	dataVec.push_back(val);
      } else {
	dataVec.push_back(*index);
	index += 2;
      }
    }
  }
}


int getArrayElemNum(splitWord wordCon){
  auto index = find(wordCon.vaCol.begin(), wordCon.vaCol.end(), "x");
  
  string temp(*(index - 1));
  
  string temp2(temp.substr(1));
  
  int num = getNum(temp2);
  return num;
}

int getNum(string strVal){
  int res = 0;

  for(int i = strVal.size()-1, j = 0; i >= 0; i--, j++){
    int dif = strVal[i] - '0';
    int val = dif * pow(10, j);
    res += val;
  }
  return res;
}


void tranceRet(splitWord wordCon){
  // string opDes = wordCon.opCol[0];
  string opDes = wordCon.vaCol[2];
  regex reg("\%.+");
  if(regex_match(opDes, reg)){ //return a constant or a variable 
    string addr = storeMap.find(opDes)->second;
    outPut("movf", addr);
  } else {
    string opSrc = opDes;
    outPut("movlw", opSrc);
  }
  outPut("movwf", "0x1");
  
}
