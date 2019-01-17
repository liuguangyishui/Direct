#include <iostream>
#include <fstream>
#include <regex>
#include "keyWord.h"
#include "keyFun.h"

using namespace::std;

void open_file(string &fileName){
  ifstream IRfile(fileName,ios_base::in);
  if(!IRfile.is_open()){
    cout << "Error open file!" << endl;
    return;
  }
  
  //precedure section of the program 
  programBegin();//the beginning of program


  string singleLine;
  while(getline(IRfile, singleLine)){//get every line
    //regex resCom(",");
    //string des(" ");
    //regex_replace(singleLine, resCom, des);
    changeComma(singleLine);
    string singleWord;
    istringstream iss(singleLine);
    //    regex reg1("\%\\d＋\b");        //for operator %x
    regex reg1("\%.+");
    //regex reg2("^\@\w＋\b=\bglobal");//for global var @varName ＝ global
    splitWord wordCon;

    while(iss >> singleWord){        //get every word in line
      if(operateSet.find(singleWord) != operateSet.end()){
	auto index = operateSet.find(singleWord);	
	wordCon.instrName = static_cast<keyWord>(index->second);
      } else if(regex_match(singleWord, reg1)){
	wordCon.opCol.push_back(singleWord);
      } 
      wordCon.vaCol.push_back(singleWord);
    }
    
    if(wordCon.instrName == knull) continue; //this line is invalid

    switch(wordCon.instrName){
    case alloc:              tranceAlloca(wordCon); break; 
    case load:               tranceLoad(wordCon);   break;
    case store:              tranceStore(wordCon);  break;
    case add:                tranceAdd(wordCon);    break;
    case sub:                tranceSub(wordCon);    break;
    case ret:                tranceRet(wordCon);    break;
    case fcmp:               tranceFcmp(wordCon);   break;
    case br:                 tranceBr(wordCon);     break;
    case label:              tranceLabel(wordCon);  break;
    case globa:              tranceGlobal(wordCon); break;
    case defin:              tranceDefine(wordCon); break;
    case call:               tranceCall(wordCon);   break;
    case anda:               tranceAnd(wordCon);    break;
    case ora:                tranceOr(wordCon);     break;
    case xora:               tranceXor(wordCon);    break;
    case shl:                tranceShl(wordCon);    break;
    case ashr:               tranceAshr(wordCon);   break;
    case zext:               tranceZext(wordCon);   break;
    case phi:                trancePhi(wordCon);    break;
    case constant:           tranceConstant(wordCon); break;
    default: break;
    }
  }
  programHint(); //the interrupt of the program 
};


