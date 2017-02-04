#include "Parser.h"
ofstream logoutput;
extern vector<string> VariablesFromTbl;

LogicalQueryPlan *selLPQ;
ExpressionAnalysis* exp_Term = NULL;
ExpressionAnalysis* exp_Factor = NULL;
ExpressionAnalysis* exp_Expression = NULL;

Compare*   pred_Comparison = NULL;
Search* boolPriSearch_cond = NULL;
Search* boolFacSearch_cond = NULL;
Search* boolTerSearch_cond = NULL;
Search* search_cond = NULL;

string tblname = "";
string attrname = "";
enum FIELD_TYPE data_type;
string myval;
MyColumn mycol;


vector<enum FIELD_TYPE> list_DataType;
vector<string> list_AttrName;
vector<string> list_attr;
vector<string> list_value;
vector<MyColumn> list_selsub;
vector<string> list_tbl;

vector<MyColumn> list_ordby;
bool isunique = false;

void Parser::OutputDiskIO(ofstream& qout_file, int out_option)
{
	
	if(out_option == 1) {
		cout<<"Number of Disk I/O: "<<disk.getDiskIOs()<<"   Elapsed time in milliseconds: "<<disk.getDiskTimer()<<endl;
		cout<<"### <------------------------------------------------------------------------> ###"<<endl;
		cout<<endl;
	}
	else if(out_option == 2) {
		qout_file<<"Number of Disk I/O: "<<disk.getDiskIOs()<<"   Elapsed time in milliseconds: "<<disk.getDiskTimer()<<endl;
		qout_file<<"### <------------------------------------------------------------------------> ###"<<endl;
		qout_file<<endl;
	}
	else {}
}

void Parser::RestartDiskIO()
{
	
	disk.resetDiskIOs();
	disk.resetDiskTimer();
}

void Parser::Clear()
{
	
    tblname = "";
    attrname = "";
    list_DataType.clear();
    list_AttrName.clear();
    list_attr.clear();
    myval="";
    list_value.clear();
    mycol.colName = "";
    mycol.tblName= "";
    list_selsub.clear();
    list_tbl.clear();
    isunique = false;
    list_ordby.clear();
    if(selLPQ != NULL)
		delete selLPQ;
	selLPQ = NULL;
    search_cond = NULL;
    exp_Term =NULL;
    exp_Factor = NULL;
    exp_Expression = NULL;
    pred_Comparison = NULL;
    boolPriSearch_cond = NULL;
    boolFacSearch_cond = NULL;
    boolTerSearch_cond = NULL;
    VariablesFromTbl.clear();
  }

Parser::Parser():lineParser(), sqlprocessor()
{
	 
}

Parser::Parser(const char* inputtext):lineParser(inputtext)
{
	
	if(!logoutput.is_open())
	{
		logoutput.open("DBLog.txt");
		if (!logoutput)
		{
  			cerr << "The file DBLog.txt could not be opened" << endl;
  		}
  	}
  	else
  	{
  		logoutput<<"\n\n### <------------------------------------------------------------------------> ###"<<endl;
  	}
}

void Parser::ParserInput(ofstream& qout_file, int out_option)
{
	
    logoutput<<"Analyzing Given Input"<<endl;
    AppTokens mytoken;
	bool not_insert = false;
	bool found_insert = false;
	bool found_insert_select = false;
	 int token_count = 0;
	const char* statement_start;
	const char* select_start;
	RelationSender select_results;
	const char* sql_statement_start;
	
	while(mytoken.tokenType != EOF)
    {
        bool pass = true;
        mytoken = lineParser.GetNextToken();

        switch(mytoken.tokenType)
        {
            case CREATE:
				if(out_option == 1) {
					cout << "CREATE";
				}
				else if(out_option == 2) {
					qout_file << "CREATE";
				}
				else {}
				sql_statement_start = lineParser.getCurrentCharacter();
				if(*sql_statement_start == '\n') {
					sql_statement_start++;
				}
				while(*sql_statement_start != '\n') {
					if(out_option == 1) {
						cout << *sql_statement_start;
					}
					else if(out_option == 2) {
						qout_file << *sql_statement_start;
					}
					else {}
					sql_statement_start++;
				}
				if(out_option == 1) {
					cout << endl;
				}
				else if(out_option == 2) {
					qout_file << endl;
				}
				logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
			    ParserTable_Create();
			    RestartDiskIO();
			    pass = sqlprocessor.CreateTbl(tblname,list_AttrName,list_DataType,qout_file,out_option);
			    OutputDiskIO(qout_file, out_option);
			    Clear();
			    break;

			case DROP:
				if(out_option == 1) {
					cout << "DROP";
				}
				else if(out_option == 2) {
					qout_file << "DROP";
				}
				else {}
				sql_statement_start = lineParser.getCurrentCharacter();
				if(*sql_statement_start == '\n') {
					sql_statement_start++;
				}
				while(*sql_statement_start != '\n') {
					if(out_option == 1) {
						cout << *sql_statement_start;
					}
					else if(out_option == 2) {
						qout_file << *sql_statement_start;
					}
					else {}
					sql_statement_start++;
				}
				if(out_option == 1) {
					cout << endl;
				}
				else if(out_option == 2) {
					qout_file << endl;
				}
				else {}
				logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
		        ParserTable_Drop();
		        RestartDiskIO();
		        pass = sqlprocessor.DropTbl(tblname,qout_file,out_option);
		        OutputDiskIO(qout_file, out_option);
		        Clear();
		        break;

			case SELECT:
				if(out_option == 1) {
					cout << "SELECT";
				}
				else if(out_option == 2) {
					qout_file << "SELECT";
				}
				else {}
				sql_statement_start = lineParser.getCurrentCharacter();
				if(*sql_statement_start == '\n') {
					sql_statement_start++;
				}
				while(*sql_statement_start != '\n') {
					if(out_option == 1) {
						cout << *sql_statement_start;
					}
					else if(out_option == 2) {
						qout_file << *sql_statement_start;
					}
					else {}
					sql_statement_start++;
				}
				if(out_option == 1) {
					cout << endl;
				}
				else if(out_option) {
					qout_file << endl;
				}
				else {}
				logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
		        ParserStmt_Select();
                VariablesFromTbl = list_tbl;
				selLPQ = Gen_QueryPlan::GenerateOptimizedLQPlan(isunique,list_selsub, list_tbl, search_cond,list_ordby);
				RestartDiskIO();
				pass = sqlprocessor.SelectTbl(selLPQ, qout_file, out_option);
		        OutputDiskIO(qout_file, out_option);
		        Clear();
		        break;

    		case DELETE:
		   		if(out_option == 1) {
					cout << "DELETE";
				}
				else if(out_option == 2) {
					qout_file << "DELETE";
				}
				else {}
			   	sql_statement_start = lineParser.getCurrentCharacter();
			   	if(*sql_statement_start == '\n') {
					sql_statement_start++;
			   	}
			   	while(*sql_statement_start != '\n') {
					if(out_option == 1) {
						cout << *sql_statement_start;
					}
					else if(out_option == 2) {
						qout_file << *sql_statement_start;
					}
					else {}
				   	sql_statement_start++;
			   	}
				if(out_option == 1) {
					cout << endl;
				}
				else if(out_option) {
					qout_file << endl;
				}
				else {}
				logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
		        ParserStmt_Delete();
		        VariablesFromTbl.push_back(tblname);
			    RestartDiskIO();
			    pass = sqlprocessor.DeleteTbl(tblname,search_cond);
			    OutputDiskIO(qout_file, out_option);
			    Clear();
			    break;

            case INSERT:
				if(out_option == 1) {
					cout << "INSERT";
				}
				else if(out_option == 2) {
					qout_file << "INSERT";
				}
				else {}
				sql_statement_start = lineParser.getCurrentCharacter();
				if(*sql_statement_start == '\n') {
					sql_statement_start++;
				}
				while(*sql_statement_start != '\n') {
					if(out_option == 1) {
						cout << *sql_statement_start;
					}
					else if(out_option) {
						qout_file << *sql_statement_start;
					}
					else {}
					sql_statement_start++;
				}
				if(out_option == 1) {
					cout << endl;
				}
				else if(out_option) {
					qout_file << endl;
				}
				else {}
				logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
				statement_start = lineParser.getCurrentCharacter();
				while(*lineParser.getCurrentCharacter() != '\n')
				{
					mytoken = lineParser.GetNextToken();
					if(mytoken.tokenType == SELECT) {
						select_start = lineParser.getCurrentCharacter();
						found_insert_select = true;
						break;
					}
					if(mytoken.tokenType == VALUES) {
						found_insert_select = false;
						break;
					}
				}
				if(found_insert_select == true) {
					lineParser.LineParserReset(select_start);
					ParserStmt_Select();
					VariablesFromTbl = list_tbl;
					selLPQ = Gen_QueryPlan::GenerateOptimizedLQPlan(isunique,list_selsub, list_tbl, search_cond,list_ordby);
					RestartDiskIO();
					select_results = sqlprocessor.InsertSelectTbl(selLPQ);
					pass = sqlprocessor.Insert_After_Select_Tbl(select_results.relation_name, select_results.tuples);
					OutputDiskIO(qout_file, out_option);
				}
				else
				{
					lineParser.LineParserReset(statement_start);
					ParserStmt_Insert();
					RestartDiskIO();
	                pass = sqlprocessor.InsertTbl(tblname,list_attr,list_value);
					OutputDiskIO(qout_file, out_option);
				}
		        Clear();
		        break;

			case UNDEFINEDTOK:
				logoutput<<"Found end of the file"<<endl;
				break;

			case EOF:
				logoutput<<"Found EOF"<<endl;
				break;

            default:
				logoutput<<"Found unidentified symbol"<<endl<<mytoken.tokenValue;
            	throw string("Enter a valid starting for the statement: ").append(mytoken.tokenValue);
				break;
        }
        if(pass == false)
        {
          throw string("\nStop !!! Inputfile could not be parsed");
        }
    }
}


void Parser::ParserStmt_Delete()
{
	logoutput<<"Parsing <Delete>"<<endl;
  	AppTokens mytoken = lineParser.GetNextToken();
  	if(mytoken.tokenType == FROM)
  	{
  		ParserName_Table();
  		logoutput<<"Found TableName: "<<tblname<<endl;
  		mytoken = lineParser.GetNextToken();
  		if(mytoken.tokenType == WHERE)
  		{
  			logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
  			ParserCond_Search();
  		}
  		else
  			lineParser.RestorePreviousToken();
  	}
  	else
  		throw string("Looking for the Keyword FROM");
}

void Parser::ParserTable_Create()
{
	logoutput<<"Analyzing <CreateTableStatement>"<<endl;
	AppTokens mytoken;
	mytoken = lineParser.GetNextToken();
	switch(mytoken.tokenType)
	{
	case TABLE:
		logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
		string relName = ParserName_Table();
		mytoken = lineParser.GetNextToken();
		switch(mytoken.tokenType)
		{
		case OPENINGPAREN:
		  logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
			ParserTypeList_Attribute();
			mytoken = lineParser.GetNextToken();
			if(mytoken.tokenType == ENCLOSINGPAREN)
			{
				logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
			}
			else
			{
				throw string("), closing parenthesis expected");
			}
			break;
		default:
			Clear();
			throw string("(, opening parenthesis expected");
		}
		break;
	}
}

string Parser::ParserName_Table()
{
	logoutput<<"Analyzing <TableName>"<<endl;
	AppTokens mytoken;
	mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == IDENTIFIER)
	{
		logoutput<<"Found Tablename IDENTIFIER: " <<mytoken.tokenValue<<endl;
		tblname = mytoken.tokenValue;
		return mytoken.tokenValue;
	}
	else
	{
		throw string("Tablename is expected");
	}
}

void Parser::ParserTypeList_Attribute()
{
	logoutput<<"Analyzing <AttributeTypeList>"<<endl;
	AppTokens mytoken;
	ParserName_Attribute();
	list_AttrName.push_back(attrname);
	ParserType_Data();
	list_DataType.push_back(data_type);
	mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == COMMA)
	{
		logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
		ParserTypeList_Attribute();
	}
	else
		  lineParser.RestorePreviousToken();
}

void Parser::ParserName_Attribute()
{
	logoutput<<"Analyzing <AttributeName>"<<endl;
	AppTokens mytoken;
	mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == IDENTIFIER)
	{
		attrname = mytoken.tokenValue;
		logoutput<<"Found IDENTIFIER: "<<mytoken.tokenValue<<endl;
	}
	else
	{
		throw string("Attribute Name - Identifier required");
	}
}


void Parser::ParserType_Data()
{
	logoutput<<"Analyzing <DataType>"<<endl;
	AppTokens mytoken;
	mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == INT_T)
	{
		logoutput<<"Found DataType:"<<mytoken.tokenValue<<endl;
		data_type = INT;
	}
	else if(mytoken.tokenType == STR20_T)
	{
		logoutput<<"Found DataType:"<<mytoken.tokenValue<<endl;
		data_type = STR20;
	}
	else
	{
		throw string("Data Type Missing!");
	}
}

void Parser::DisplayMemory()
{
}

void Parser::ParserTable_Drop()
{
	logoutput<<"Analyzing <DropTableStatement>"<<endl;
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType== TABLE)
	{
		logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
		ParserName_Table();
		logoutput<<"Table "<<tblname<<"will be dropped"<<endl;
	}
	else
	{
		throw string("Need Keyword TABLE");
	}
}

void Parser::ParserStmt_Insert()
{
	logoutput<<"Analyzing <InsertStatement>"<<endl;
	AppTokens mytoken;
	mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == INTO)
	{
		logoutput<<"Found Keyword: "<<mytoken.tokenValue<<endl;
		string tblname = ParserName_Table();
		mytoken = lineParser.GetNextToken();
		if(mytoken.tokenType == OPENINGPAREN)
		{
			logoutput<<"Found Syntax: "<<mytoken.tokenValue<<endl;
			ParserList_Attribute();
			mytoken = lineParser.GetNextToken();
			if(mytoken.tokenType== ENCLOSINGPAREN)
			{
				logoutput<<"Found Syntax: "<<mytoken.tokenValue<<endl;
				ParserTuples_Insert();
			}
			else
				throw string("Need the Syntax )");
		}
		else
			throw string("Need the syntax (");
	}
	else
	{
		throw string("Need the keyword INTO");
	}
}

void Parser::ParserTuples_Insert()
{
	logoutput<<"Analyzing <InsertTuples>"<<endl;
	AppTokens mytoken = lineParser.GetNextToken();

	if(mytoken.tokenType == VALUES)
	{
		logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
		mytoken = lineParser.GetNextToken();
		if(mytoken.tokenType == OPENINGPAREN)
		{
			logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
			ParserList_Value();
			mytoken = lineParser.GetNextToken();
			if(mytoken.tokenType == ENCLOSINGPAREN)
			{
				logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
				return;
			}
			else
				throw string("Missing syntax )");
		}
		else
			throw string("Missing Syntax (");
	}
	else if(mytoken.tokenType == SELECT)
	{
		logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
	  ParserStmt_Select();
	}
	else
		throw string("Keyword VALUES required");
}

void Parser::ParserList_Attribute()
{
	logoutput<<"Analyzing <AttributeList>"<<endl;
  	ParserName_Attribute();
	list_attr.push_back(attrname);
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == COMMA)
	{
		logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
		ParserList_Attribute();
	}
	else
		lineParser.RestorePreviousToken();
}

void Parser::ParserList_Value()
{
	logoutput<<"Analyzing <ValueList>"<<endl;
	Parser_Value();
	list_value.push_back(myval);
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == COMMA)
	{
		logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
		ParserList_Value();
	}
	else
	   lineParser.RestorePreviousToken();
}

void Parser::Parser_Value()
{
	logoutput<<"Analyzing <Value>"<<endl;
	AppTokens mytoken = lineParser.GetNextToken();
	switch(mytoken.tokenType)
	{
	case LITERAL:
		logoutput<<"Found LITERAL:"<<mytoken.tokenValue<<endl;
		myval =  mytoken.tokenValue;
		break;
	case INTEGER:
		logoutput<<"Found INTEGER:"<<mytoken.tokenValue<<endl;
		myval =  mytoken.tokenValue;
		break;
	case NULL_T:
		logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
		myval =  mytoken.tokenValue;
		break;
	default:
		throw string("Looking for a LITERAL/ INTEGER/ NULL");
	}
}

void Parser::ParserStmt_Select()
{	
	logoutput<<"Analyzing <SelectStatement>"<<endl;
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == DISTINCT)
	{
		logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
		isunique = true;
		ParserList_Select();
	}
	else
	{
		lineParser.RestorePreviousToken();
		ParserList_Select();
	}

	mytoken =lineParser.GetNextToken();
	if(mytoken.tokenType == FROM)
	{
		logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
		ParserList_Table();
		mytoken = lineParser.GetNextToken();
		if(mytoken.tokenType == WHERE)
		{
			logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
			ParserCond_Search();
		}
		else
			lineParser.RestorePreviousToken();

		mytoken =lineParser.GetNextToken();
		if(mytoken.tokenType == ORDER)
		{
			logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
			mytoken = lineParser.GetNextToken();
			if(mytoken.tokenType == BY)
			{
				logoutput<<"Found Keyword:"<<mytoken.tokenValue<<endl;
				ParserName_Column();
				list_ordby.push_back(mycol);
			}
			else
				throw string("Looking for Keyword - BY");
		}
		else
			lineParser.RestorePreviousToken();
	}
	else
		throw string("Looking for Keyword - FROM");
}

void Parser::ParserList_Select()
{
	logoutput<<"Analyzing <SelectList>"<<endl;
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == MULTIPLY)
	{
		mycol.colName = mytoken.tokenValue;
		mycol.tblName = NOT_A_TABLE;
		list_selsub.push_back(mycol);
		logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
	}
	else
	{
		lineParser.RestorePreviousToken();
		ParserSubList_Select();
	}
}

void Parser::ParserSubList_Select()
{
	logoutput<<"Analyzing <ParseSelectSublist>"<<endl;
	ParserName_Column();
	list_selsub.push_back(mycol);
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == COMMA)
	{
		logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
		ParserSubList_Select();
	}
	else
		lineParser.RestorePreviousToken();
}

void Parser::ParserList_Table()
{
	logoutput<<"Found <TableList>"<<endl;
	ParserName_Table();
	list_tbl.push_back(tblname);
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == COMMA)
	{
		logoutput<<"Found Syntax:"<<mytoken.tokenValue<<endl;
	   ParserList_Table();
	}
	else
		lineParser.RestorePreviousToken();
}

void Parser::ParserName_Column()
{	
	logoutput<<"Parsing <ColumnName>"<<endl;
	AppTokens mytoken;
	string colName;
	string tblName1;
	AppTokens mytoken1;
	mytoken1 = lineParser.GetNextToken();
	if(mytoken1.tokenType == IDENTIFIER)
	{
		logoutput<<"Found a Tablename IDENTIFIER: " <<mytoken1.tokenValue<<endl;
		tblName1 = mytoken1.tokenValue;
	}
	else
	{
		throw string("Tablename/Columnname is required");
	}
	mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == PERIOD)
	{
		mytoken = lineParser.GetNextToken();
		if(mytoken.tokenType == IDENTIFIER)
		{
			colName = mytoken.tokenValue;
			logoutput<<"Found Column Name: "<<tblName1<<"."<<colName<<endl;
		}
		else
			throw ("Column-name is required after the PERIOD(.)");
	}
	else
	{
		colName = tblName1;
		tblName1 = NOT_A_TABLE;
		logoutput<<"Found Columname: "<<colName<<endl;
		lineParser.RestorePreviousToken();
	}

	logoutput<<tblName1<<"."<<colName<<endl;
	mycol.tblName = tblName1;
	mycol.colName = colName;
}

void Parser::Parser_Factor()
{
	string col_Name;
	logoutput<<"Analyzing <Factor>"<<endl;
	AppTokens mytoken = lineParser.GetNextToken();
	switch(mytoken.tokenType)
	{
	case IDENTIFIER:
		lineParser.RestorePreviousToken();
		ParserName_Column();
		col_Name = mycol.tblName;
		col_Name.append(".");
		col_Name.append(mycol.colName);
		exp_Factor = new ExpressionAnalysis(col_Name,COLNAME_VALUE);
		break;
	case LITERAL:
		logoutput<<"Found a LITERAL: "<<mytoken.tokenValue<<endl;
		logoutput<< "Literal: "<<mytoken.tokenValue<<endl;
		exp_Factor = new ExpressionAnalysis(mytoken.tokenValue,LITERAL_VALUE);
		break;
	case INTEGER:
		logoutput<<"Found an INTEGER: "<<mytoken.tokenValue<<endl;
		logoutput<< "Integer: "<<mytoken.tokenValue<<endl;
		exp_Factor = new ExpressionAnalysis(mytoken.tokenValue,INTEGER_VALUE);
		break;
	case OPENINGPAREN:
		logoutput<<"Found a Syntax: "<<mytoken.tokenValue<<endl;
		Parser_Expression();
		mytoken = lineParser.GetNextToken();
		if(mytoken.tokenType == ENCLOSINGPAREN)
		{
			logoutput<<"Found a Syntax: "<<mytoken.tokenValue<<endl;
			exp_Factor = exp_Expression;
		}
		else
			throw string("Looking for a Syntax )");
		break;
	default:
		throw string("Need an IDENTIFIER/ LITERAL/ INTEGER or an (EXPRESSION)");
	}

}
void Parser:: Parser_Term()
{
	logoutput<<"Analyzing <Term>"<<endl;
	Parser_Factor();
	exp_Term = exp_Factor;
	ExpressionAnalysis *leftFactorNode = exp_Factor;
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == MULTIPLY)
	{
		logoutput<<"Found an Operator: "<<mytoken.tokenValue<<endl;
		ExpressionAnalysis* left = exp_Term;
		Parser_Term();
		ExpressionAnalysis* right = exp_Term;
		exp_Term = new ExpressionAnalysis(left, mytoken.tokenValue, right);
	}
	else if(mytoken.tokenType == DIVISION)
	{
		logoutput<<"Found an Operator: "<<mytoken.tokenValue<<endl;
		ExpressionAnalysis* left = exp_Term;
		Parser_Term();
		ExpressionAnalysis* right = exp_Term;
		exp_Term = new ExpressionAnalysis(left, mytoken.tokenValue, right);
	}
	else
		lineParser.RestorePreviousToken();
}

void Parser::Parser_Expression()
{
	logoutput<<"Analyzing <Expression>"<<endl;
	Parser_Term();
	exp_Expression =  exp_Term;
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == ADDITION)
	{
		logoutput<<"Found an Operator: "<<mytoken.tokenValue<<endl;
		ExpressionAnalysis* left = exp_Expression;
		Parser_Expression();
		ExpressionAnalysis* right = exp_Expression;
		exp_Expression = new ExpressionAnalysis(left, mytoken.tokenValue, right);
	}
	else if(mytoken.tokenType == MINUS)
	{
		logoutput<<"Found an Operator: "<<mytoken.tokenValue<<endl;
		ExpressionAnalysis* left = exp_Expression;
		Parser_Expression();
		ExpressionAnalysis* right = exp_Expression;
		exp_Expression = new ExpressionAnalysis(left, mytoken.tokenValue, right);
	}
	else
		lineParser.RestorePreviousToken();
}
void Parser::ParserFactor_Boolean()
{	
	logoutput<<"Parsing <BooleanFactor>"<<endl;
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == NOT)
	{
		logoutput<<"Found a Keyword: "<<mytoken.tokenValue<<endl;
		ParserPrimary_Boolean();
		Search* null_node = NULL;
		boolFacSearch_cond = new Search(null_node, mytoken.tokenValue, boolPriSearch_cond);
	}
	else
	{
		lineParser.RestorePreviousToken();
		ParserPrimary_Boolean();
		boolFacSearch_cond = boolPriSearch_cond;
	}
}


void Parser::ParserPrimary_Boolean()
{
	logoutput<<"Parsing <BooleanPrimary>"<<endl;
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == OPENINGSQPAREN)
	{
		logoutput<<"Found a Syntax: "<<mytoken.tokenValue<<endl;
		ParserCond_Search();
		mytoken = lineParser.GetNextToken();
		if(mytoken.tokenType == ENCLOSINGSQPAREN)
		{
			logoutput<<"Found a Syntax: "<<mytoken.tokenValue<<endl;
			boolPriSearch_cond = search_cond;
		}
		else
			throw string("Looking for a Syntax )");
	}
	else
	{
		lineParser.RestorePreviousToken();
		ParserPredicate_Comparison();
		boolPriSearch_cond  = new Search(pred_Comparison);
	}
}

void Parser::ParserPredicate_Comparison()
{
	logoutput<<"Analyzing <Comparision-Predicate>"<<endl;
	Parser_Expression();
	ExpressionAnalysis* left = exp_Expression;
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == COMPOPERATOR)
	{
		logoutput<<"Found a COMPARISON OPERATOR Syntax: "<<mytoken.tokenValue<<endl;
		Parser_Expression();
		ExpressionAnalysis* right = exp_Expression;
		pred_Comparison = new Compare(left, mytoken.tokenValue, right);
	}
	else
		throw string ("Looking for a Logical Operator =,> or <");
}

void Parser::ParserCond_Search()
{	
	ParserTerm_Boolean();
	logoutput<<"Analyzing <Search Condition>"<<endl;
	search_cond = boolTerSearch_cond;
	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == OR)
	{
		logoutput<<"Found a Keyword: OR"<<endl;
		Search* left = search_cond;
	  ParserCond_Search();
		Search* right = search_cond;
		search_cond = new Search(left, mytoken.tokenValue, right);
	}
	else
	  lineParser.RestorePreviousToken();
}

void Parser::ParserTerm_Boolean()
{ 
	logoutput<<"Analyzing <BooleanTerm>"<<endl;
	ParserFactor_Boolean();
	boolTerSearch_cond = boolFacSearch_cond;
  	AppTokens mytoken = lineParser.GetNextToken();
	if(mytoken.tokenType == AND)
	{
		logoutput<<"Found a Keyword: AND"<<endl;
		Search* left = boolTerSearch_cond;
		ParserTerm_Boolean();
		Search* right = boolTerSearch_cond;
		boolTerSearch_cond = new Search(left,mytoken.tokenValue, right);
	}
	else
		lineParser.RestorePreviousToken();
}
