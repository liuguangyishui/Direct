#include <set>
#include <unordered_map>
#include <vector>

#ifndef _KEYWORD_H_
#define _KEYWORD_H_

using namespace::std;
static unordered_map<string,int> operateSet \
  = {{"alloca", 1}, {"load", 2},
     {"store", 3},  {"ret",4},
     {"add",5},     {"sub", 6},
     {"fcmp",7},    {"br", 8},
     {"<label>", 9}};

enum keyWord{
  knull,
  alloc,     //don't use alloca,it is a reserve key
  load,
  store,
  ret,
  add,
  sub,
  
  fcmp,
  br,
  label,
};

struct splitWord{
  keyWord instrName = knull;
  vector<string> opCol;
  vector<string> vaCol;
};


#endif
