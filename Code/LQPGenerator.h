//Generates Optimized Query plan.
#ifndef _LQPGENERATOR_H
#define _LQPGENERATOR_H

#include <cstdlib>

#include "LogicalQueryPlan.h"
#include "Search.h"
#include "Process.h"

class LQPGenerator
{
	public:
		static LogicalQueryPlan* GenerateLQPlan(vector<MyColumn> project_list, vector<string> from_list, Search*& search_cond);
		static LogicalQueryPlan* GenerateOptimizedLQPlan(bool isUnique, vector<MyColumn> project_list, vector<string> from_list, Search*& search_cond, vector<MyColumn> orderby_list);
};
#endif
