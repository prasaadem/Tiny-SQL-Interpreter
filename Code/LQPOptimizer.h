#ifndef _LQPOptimizer_H
#define _LQPOptimizer_H

#include<cstdlib>

#include "LogicalQueryPlan.h"

class LQPOptimizer
{
	public:
		static LogicalQueryPlan* LogicalQueryplanOptimizer(LogicalQueryPlan*& lqp);
};

#endif
