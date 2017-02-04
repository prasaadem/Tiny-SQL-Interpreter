#ifndef _PARSER_H
#define _PARSER_H

//C++ Headers
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<cstring>
#include<cstdio>

//Storage Manager Headers
#include "Block.h"
#include "Config.h"
#include "Disk.h"
#include "Field.h"
#include "MainMemory.h"
#include "Relation.h"
#include "Schema.h"
#include "SchemaManager.h"
#include "Tuple.h"

//Custom Headers
#include "LineParser.h"
#include "SQLProcessor.h"
#include "LogicalQueryPlan.h"
#include "Process.h"
#include "LQPGenerator.h"
#include "LQPOptimizer.h"
#include "DBConfig.h"

class Parser
{
private:
    LineParser lineParser;
    SQLProcessor sqlprocessor;
    void ParserTable_Create();
    string ParserName_Table();
    void ParserTypeList_Attribute();
    void ParserName_Attribute();
    void ParserType_Data();
    void ParserTable_Drop();
    void ParserStmt_Insert();
    void ParserTuples_Insert();
    void ParserList_Attribute();
    void ParserList_Value();
    void Parser_Value();
    void ParserStmt_Select();
    void ParserList_Select();
    void ParserSubList_Select();
    void ParserList_Table();
    void ParserStmt_Delete();
    void ParserCond_Search();
    void ParserTerm_Boolean();
    void ParserFactor_Boolean();
    void ParserPrimary_Boolean();
    void ParserPredicate_Comparison();
    void Parser_Expression();
    void ParserOperator_Comparison();
    void Parser_Term();
    void Parser_Factor();
    void ParserName_Column();

    void OutputDiskIO(ofstream& qout_file, int output_option);
    void RestartDiskIO();

public:
    Parser();
    void Clear();
    Parser(const char *inputtext);
    Parser(char input);
    void ParserInput(ofstream& qout_file, int out_option);
    void DisplayMemory();
};

typedef LQPGenerator Gen_QueryPlan;
typedef LQPOptimizer Opt_QueryPlan;

#endif
