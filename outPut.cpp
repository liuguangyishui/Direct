#include <iostream>
#include <fstream>
#include <cassert>
#include "outPut.h"

using namespace::std;

std::ofstream fout;

void open_outputFile(string outputFileName){
  if(!outputFileName.empty()){
    fout.open(outputFileName,ios_base::out);
    if(!fout.is_open())
      cout << "this file is not open" << endl;
      //  assert(!fout.is_open());
  }
  
}

void close_outputFile(){
  if(fout.is_open())
    fout.close();
}

void outPut(string instrName, string op){
  if(outputFileName.empty() && fout.is_open()){
    fout << "\t" << instrName << "\t\t" << op << "\t" \
       << 0 << "\t" << 1 << endl;
  } else {
    cout << "\t" << instrName << "\t\t" << op << "\t"	\
	 << 0 << "\t" << 1 << endl;
  }
}

void outPut(string instrName, string op,int index1 = 0, int index2 = 1){
  if(outputFileName.empty() && fout.is_open()){
    fout << "\t" << instrName << "\t\t" << op << "\t"	\
	 << index1 << "\t" << index2 << endl;
  } else {
    cout << "\t" << instrName << "\t\t" << op << "\t"	\
	 << index1 << "\t" << index2 << endl;
  }
}

void outPutOrg(string instrName, string op){
  if(outputFileName.empty() && fout.is_open()){
    fout << "\t" << instrName << "\t\t" << op << "\t" << endl;
  } else {
    cout << "\t" << instrName << "\t\t" << op << "\t" << endl;
  }
}

void outPutJump(string instrName, string op){
  if(outputFileName.empty() && fout.is_open()){
    fout << "\t" << instrName << "\t\t" << "Label" << "_" << op << endl;
  } else {
    cout << "\t" << instrName << "\t\t" << "Label" << "_" << op << endl;
  }
}

void outPutLabel(string orderName){
  if(outputFileName.empty() && fout.is_open()){
    fout << "\n" << flush;
    fout << "Label" << "_" << orderName << ":" << endl;
  } else {
    cout << "\n" << flush;
    cout << "Label" << "_" << orderName << ":" << endl;
  }
}
