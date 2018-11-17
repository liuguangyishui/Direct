#include <iostream>
#include <vector>

#ifndef _REGALLOC_H_
#define _REGALLOC_H_

/* the file is for the register allocation
 */

using namespace::std;

static vector<string> ox16 = {"0", "1", "2", "3", "4", "5",
		       "6", "7", "8", "9", "A",
		       "B", "C", "D", "E", "F"};

class regRe{
 public:
  static string allocaReg();
  static string allocaReg_funPara();

  static int count_RegNum;
  static int RegNum;

  //for fun para deliever
  static int count_RegNum_funPara; //
  static int RegNum_funPara;
  static int indexNum;
};

#endif
