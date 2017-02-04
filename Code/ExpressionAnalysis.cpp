#include "ExpressionAnalysis.h"
#include "TupleOperation.h"

map<string,int> ExpressionAnalysis::colMap;
typedef pair<string,string>  TColPair;
typedef pair<FIELD_TYPE,Field> FValue;
extern map<TColPair, FValue> Column_Value_Mapping;

ExpressionAnalysis::ExpressionAnalysis()
{
	
		value = "";
		exp_operator = eSelf;
		exp_left = NULL;
		exp_right = NULL;
}

ExpressionAnalysis::ExpressionAnalysis(string sval, VALUETYPE value_type)
{
		value = sval;
		valuetype = value_type;
		exp_operator = eSelf;
		exp_left = NULL;
		exp_right = NULL;
}

ExpressionAnalysis::ExpressionAnalysis(ExpressionAnalysis*& left_exp, string exp_opt, ExpressionAnalysis*& right_exp)
{
		if(exp_opt == "+")
			exp_operator = ePlus;
		else if(exp_opt == "-")
			exp_operator = eMinus;
		else if(exp_opt == "*")
			exp_operator = eMultiply;
		else
			exp_operator = eDivision;

		exp_left = left_exp;
		exp_right = right_exp;
}

TColPair GetTableColumn(string value)
{
		int position = value.find_first_of('.');
		int len = value.length();
		string table_nam = value.substr(0, position);
		string col_name = value.substr(position+1, len - position -1);
		if(table_nam == NOT_A_TABLE)
			table_nam = TupleOperation::Fetch_Table_Name(col_name);
		return TColPair(table_nam, col_name);
}

FValue ExpressionAnalysis::Compute()
{
		FValue eval_result;
		switch(exp_operator)
		{
				case ePlus:
						eval_result.second.integer = exp_left->Compute().second.integer + exp_right->Compute().second.integer;
						break;
				case eMinus:
						eval_result.second.integer = exp_left->Compute().second.integer - exp_right->Compute().second.integer;
						break;
				case eMultiply:
						eval_result.second.integer = exp_left->Compute().second.integer * exp_right->Compute().second.integer;
						break;
				case eDivision:
						int right_Val;
						right_Val = exp_right->Compute().second.integer;
						if(0 == right_Val)
						{
							throw string("Error !!, Cannot convert STR20 to INT. Error in evaluating. ");
						}
						else
						{
							int left_Val = exp_left->Compute().second.integer;
							eval_result.second.integer = left_Val/right_Val;
						}
						break;
				case eSelf:
						if(valuetype == INTEGER_VALUE)
						{
							eval_result.second.integer = atoi(value.c_str());
							eval_result.first = INT;
						}
						else if(valuetype == COLNAME_VALUE)
						{
							TColPair key = GetTableColumn(value);
							eval_result = Column_Value_Mapping[key];
						}
						else
						{
							eval_result.second.str = &value;
							eval_result.first = STR20;
						}
						break;
				default:
						throw string("Error!!, Could not evaluate the Operator");
				}
				return eval_result;
}

void ExpressionAnalysis::operator delete(void *pointer)
{
		if(pointer!=NULL)
		{
		if(((ExpressionAnalysis*)pointer)->exp_left != NULL)
			delete ((ExpressionAnalysis*)pointer)->exp_left;

		if(((ExpressionAnalysis*)pointer)->exp_right != NULL)
			delete ((ExpressionAnalysis*)pointer)->exp_right;
		free(pointer);
		}
}

EXPOPERATOR ExpressionAnalysis::get_exp_operator() {
	return exp_operator;
}
