#include <iostream>
#include <cassert>
#include "regAlloc.h"

using namespace::std;

int regRe::RegNum = 512;
int regRe::count_RegNum = 0;

string regRe::allocaReg(){
  if(count_RegNum > RegNum){
    assert("no enough register to alloca!");
  }
  ++count_RegNum;
  string res = "0x";
  
  int RegNum_temp = count_RegNum;
  while(RegNum_temp > 15){
    int mod = RegNum_temp % 16;
    RegNum_temp /= 16;
    res += ox16[mod];
  }
  res += ox16[RegNum_temp];
  return res+"H";
}
