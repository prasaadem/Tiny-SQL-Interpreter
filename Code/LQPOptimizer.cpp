#include"LQPOptimizer.h"

LogicalQueryPlan* LQPOptimizer::LogicalQueryplanOptimizer(LogicalQueryPlan*& logical_query_plan)
{
		return logical_query_plan->FetchOptimizedQP();
}
