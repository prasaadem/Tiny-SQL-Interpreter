#ifndef _LOGICALQUERYPLAN_H
#define _LOGICALQUERYPLAN_H

#include <vector>

#include "Process.h"
#include "TupleOperation.h"
#include "QPConstructor.h"
#include "TableProcessor.h"
#include "RelationSender.h"


enum OPtype {SELECT_OP, PROJECT, ORDERBY, VECTORPRODUCT,
						Optimum_PROJECT_S_PRODUCT, 
						Optimum_PROJECT_S_PRODUCT_SELECT,
						Optimum_PROJECT_M_PRODUCT,
						Optimum_PROJECT_M_PRODUCT_SELECT
						};

class LogicalQueryPlan
{
		private:
				bool isUnique;
				vector<MyColumn> order_BY;
				RelationSender sendRelation;
				OPtype operationType;
				OrderOP orderByOp;
				ProjectOP projectOp;
				SelectOP selectOp;
				JOINOP productOp;
				QPConstructor* construct_QP;
				LogicalQueryPlan* left_LogicalQP;
				LogicalQueryPlan* right_LogicalQP;
		public:
				LogicalQueryPlan();
				LogicalQueryPlan(LogicalQueryPlan*& left, LogicalQueryPlan *&right);
				void Add_Left_LPQ(LogicalQueryPlan*& lp);
				void Add_Right_LPQ(LogicalQueryPlan*& rp);
				static LogicalQueryPlan* CreateSelectQueryPlan(SelectOP selectop);
				static LogicalQueryPlan* CreateProjectQueryPlan(ProjectOP projectop);
				static LogicalQueryPlan* CreateJoinQueryPlan(JOINOP joinop);
				static LogicalQueryPlan* CreateProjSelJoinQueryPlan(bool isDistinct, ProjectOP projectop, SelectOP& selectop, JOINOP joinop, vector<MyColumn> orderBy);
				static LogicalQueryPlan* CreateProjJoinQueryPlan(bool isDistinct, ProjectOP projectop, JOINOP joinop, vector<MyColumn> orderBy);
				LogicalQueryPlan* FetchOptimizedQP();
				RelationSender Execute();
				void operator delete(void *p);
	};
#endif
