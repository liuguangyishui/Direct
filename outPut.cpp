#include <iostream>
#include <fstream>
#include <cassert>
#include "outPut.h"

using namespace::std;

std::ofstream fout;

void open_outputFile(string outputFileName){
  if(!outputFileName.empty()){
    fout.open(outputFileName,ios_base::out);
    if(fout.is_open())
    //   assert(!fout.is_open());
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

void outPutJump(string instrName, string op){
  if(outputFileName.empty() && fout.is_open()){
    fout << "\t" << instrName << "\t\t" << "Label" << op << endl;
  } else {
    cout << "\t" << instrName << "\t\t" << "Label" << op << endl;
  }
}

void outPutLabel(string orderName){
  if(outputFileName.empty() && fout.is_open()){
    fout << "\n" << flush;
    fout << "Label" << "%" << orderName << ":" << endl;
  } else {
    cout << "\n" << flush;
    cout << "Label" << "%" << orderName << ":" << endl;
  }
}
