#include <iostream>
#include "outPut.h"

using namespace::std;

void outPut(string instrName, string op){
  cout << "\t" << instrName << "\t\t" << op << "\t" \
       << 0 << "\t" << 1 << endl;
}

void outPut(string instrName, string op,int index1 = 0, int index2 = 1){
  cout << "\t" << instrName << "\t\t" << op << "\t" \
       << index1 << "\t" << index2 << endl;
}

void outPutJump(string instrName, string op){
  cout << "\t" << instrName << "\t\t" << "Label" << op << endl;
}

void outPutLabel(string orderName){
  cout << "\n" << flush;
  cout << "Label" << "%" << orderName << ":" << endl;
}
