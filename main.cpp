#include <iostream>
#include <cstring>
#include <assert.h>
#include "keyFun.h"
#include "keyWord.h"
#include "analysisIR.h"

using namespace::std;

int main(int argv, char **argc){
  
  for(int i = 0; i < argv; i++){
    if(strcmp(argc[i],"-f") == 0 || strcmp(argc[i],"-F") == 0){
      assert(++i < argv);  //whether have input file
      string fileName = argc[i];
      open_file(fileName);
    }
  }
  return 0;

}
