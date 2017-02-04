#ifndef _COMPARE_H
#define _COMPARE_H

#include <cstdlib>
#include <iostream>

#include "ExpressionAnalysis.h"

enum Comparison_Operator {comp_less, comp_greater, comp_equal, comp_self};

class Compare
{
private:
	bool myvalue;
	Comparison_Operator c_opt;
	ExpressionAnalysis* exp_left;
	ExpressionAnalysis* exp_right;
public:
	Compare(bool value);
	Compare(ExpressionAnalysis*& expLeft, string c_opt, ExpressionAnalysis*& expRight);
	bool Compute();
	void operator delete(void *pointer);
};
#endif
