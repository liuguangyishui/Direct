#include <iostream>
#include <cassert>
#include "regAlloc.h"

using namespace::std;

int regRe::RegNum = 512;
int regRe::count_RegNum = 9;

int regRe::RegNum_funPara = 8; //for fun para deliever
int regRe::count_RegNum_funPara = 0;

string regRe::allocaReg(){
  if(count_RegNum > RegNum){
    cout << "No enough register to alloca！" << endl;
    assert(count_RegNum <= RegNum);
  }
  ++count_RegNum;
  string res;
  
  int RegNum_temp = count_RegNum;
  while(RegNum_temp > 15){
    int mod = RegNum_temp % 16;
    RegNum_temp /= 16;
    res = ox16[mod] + res;
  }
  res = ox16[RegNum_temp] + res;
  return "0x" + res;// + "H";
}

string regRe::allocaReg_funPara(){
  if(count_RegNum_funPara > RegNum_funPara){
    cout << "No enough register to alloca!" << endl;
    assert(count_RegNum_funPara <= RegNum_funPara);
  }
  ++count_RegNum_funPara;
  string res = "0x";
  
  return res+ox16[count_RegNum_funPara];//+"H";
  
}

