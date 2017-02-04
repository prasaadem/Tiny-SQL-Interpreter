#include "Compare.h"

typedef pair<string, string> TColPair;
typedef pair<FIELD_TYPE, Field> FValue;

Compare::Compare(bool value)
{
	myvalue = value;
	c_opt = comp_self;
	exp_left = NULL;
	exp_right = NULL;
}

Compare::Compare(ExpressionAnalysis*& expLeft, string copt, ExpressionAnalysis*& expRight)
{
	if(copt == "=")
		c_opt = comp_equal;
	else if(copt == "<")
		c_opt = comp_less;
	else
		c_opt = comp_greater;
	 exp_left = expLeft;
	 exp_right = expRight;
}

bool Compare::Compute()
{	
	FIELD_TYPE field_type_left;
	FIELD_TYPE field_type_right;
	FValue value_left;
	FValue value_right;
	bool result;
	bool excption = false;
	
	switch(c_opt)
	{
	case comp_less:
					value_left = exp_left->Compute();
					value_right = exp_right->Compute();
					if(value_left.first == INT && value_right.first == INT)
						result = (value_left.second.integer < value_right.second.integer);
					else if(value_left.first == STR20 && value_right.first == STR20)
					{
						if( (*(value_left.second.str)).compare(*(value_right.second.str)) <0)
							result = true;
						else
							result = false;
					}
					else
						excption = true;
					break;
	case comp_greater:
					value_left = exp_left->Compute();
					value_right = exp_right->Compute();
					if(value_left.first == INT && value_right.first == INT)
								result = (value_left.second.integer > value_right.second.integer);
					else if(value_left.first == STR20 && value_right.first == STR20)
					{
								if( (*(value_left.second.str)).compare(*(value_right.second.str)) >0)
									result = true;
								else
									result = false;
					}
					else
						excption = true;
					break;
	case comp_equal:
					value_left = exp_left->Compute();
					value_right = exp_right->Compute();
					if(value_left.first == INT && value_right.first == INT) {

							result = (value_left.second.integer == value_right.second.integer);
					}
					else if(value_left.first == STR20 && value_right.first == STR20)
					{
							
							if( (*(value_left.second.str)).compare(*(value_right.second.str)) == 0)
								result = true;
							else
								result = false;
							
					}
		else
					excption = true;
					break;
	case comp_self:
					result = myvalue;
					break;
	}
	if(excption)
			throw string("Compare between different data types, Error!!");
	return result;
}

void Compare::operator delete(void *pointer)
{
		if(pointer!=NULL)
		{
					if(((Compare*)pointer)->exp_right != NULL)
					{
						delete ((Compare*)pointer)->exp_right;
						((Compare*)pointer)->exp_right = NULL;
					}
					if(((Compare*)pointer)->exp_left != NULL)
					{
						delete ((Compare*)pointer)->exp_left;
						((Compare*)pointer)->exp_left = NULL;
					}
					free(pointer);
					pointer = NULL;
		}
}
