#ifndef _EXPRESSIONANALYSIS_H
#define _EXPRESSIONANALYSIS_H

#include<string>
#include<cstdlib>
#include<map>

#include "Block.h"
#include "Config.h"
#include "Disk.h"
#include "Field.h"
#include "MainMemory.h"
#include "Relation.h"
#include "Schema.h"
#include "SchemaManager.h"

#include "Tuple.h"
#include "DBConfig.h"

using namespace std;

typedef pair<string,string>  TColPair;
typedef pair<FIELD_TYPE,Field> FValue;

enum EXPOPERATOR{ePlus=1,eMinus,eMultiply,eDivision,eSelf};
enum VALUETYPE {INTEGER_VALUE,LITERAL_VALUE,COLNAME_VALUE};

class ColumnMapping
{
    map<string,int> colMap;
};

class ExpressionAnalysis
{
    private:
      EXPOPERATOR exp_operator;
      VALUETYPE valuetype;
      string value;
      ExpressionAnalysis *exp_left;
      ExpressionAnalysis *exp_right;

    public:
      static map<string, int> colMap;
      ExpressionAnalysis();
      ExpressionAnalysis(string value, VALUETYPE valuetype);
      ExpressionAnalysis(ExpressionAnalysis*& exp_left,string exp_opt,ExpressionAnalysis*& exp_right);
      FValue Compute();
      void operator delete(void *pointer);
      EXPOPERATOR get_exp_operator();
};
#endif
