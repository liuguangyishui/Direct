#include <iostream>
#include <cstring>
#include <assert.h>
#include "keyFun.h"
#include "keyWord.h"
#include "analysisIR.h"
#include "outPut.h"

using namespace::std;

int main(int argv, char **argc){

  string inputFileName;
  for(int i = 0; i < argv; i++){
    if(strcmp(argc[i],"-f") == 0 || strcmp(argc[i],"-F") == 0){
      assert(++i < argv);  //whether have input file
      inputFileName = argc[i];
    }
    if(strcmp(argc[i],"-o") == 0){
      if(++i < argv)
	outputFileName = argc[i];
    }
  }
  open_outputFile(outputFileName);
  open_file(inputFileName);
  close_outputFile();
  
  return 0;

}
