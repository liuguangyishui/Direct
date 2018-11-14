#include <iostream>
#include <vector>

#ifndef _REGALLOC_H_
#define _REGALLOC_H_

using namespace::std;

static vector<string> ox16 = {"0", "1", "2", "3", "4", "5",
		       "6", "7", "8", "9", "A",
		       "B", "C", "D", "E", "F"};

class regRe{
 public:
  static string allocaReg();
  static int count_RegNum;
  static int RegNum;

};

#endif
