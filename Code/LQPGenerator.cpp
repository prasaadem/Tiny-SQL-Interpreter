#include "LQPGenerator.h"

LogicalQueryPlan* LQPGenerator::
GenerateLQPlan(vector<MyColumn> project_list, vector<string> from_list, Search*& search_cond)
{
	
			JOINOP prodOp_obj(from_list);
			LogicalQueryPlan *lp_crossproduct = LogicalQueryPlan::CreateJoinQueryPlan(prodOp_obj);
			LogicalQueryPlan *lp_select = NULL;
			if(search_cond != NULL)
			{
					SelectOP SelectOp_obj(search_cond);
					LogicalQueryPlan *lp_select = LogicalQueryPlan::CreateSelectQueryPlan(SelectOp_obj);
					lp_select->Add_Left_LPQ(lp_crossproduct);
			}

			ProjectOP projectOp_obj(project_list);
			LogicalQueryPlan *lp_Project = LogicalQueryPlan::CreateProjectQueryPlan(projectOp_obj);

			if(search_cond != NULL)
			{
				lp_Project->Add_Left_LPQ(lp_select);
			}
			else
			{
				lp_Project->Add_Left_LPQ(lp_crossproduct);
			}
			return lp_Project;
}

LogicalQueryPlan* LQPGenerator::
GenerateOptimizedLQPlan(bool isUnique, vector<MyColumn> project_list, vector<string> from_list, Search*& search_cond, vector<MyColumn> orderby_list)
{
			LogicalQueryPlan* Optimized_QP;
			JOINOP prodOp_obj(from_list);
			ProjectOP projectOp_obj(project_list);
			if(search_cond != NULL)
			{
				SelectOP SelectOp_obj(search_cond);
				Optimized_QP = LogicalQueryPlan::CreateProjSelJoinQueryPlan(isUnique, projectOp_obj, SelectOp_obj, prodOp_obj, orderby_list);
			}
			else {
				Optimized_QP = LogicalQueryPlan::CreateProjJoinQueryPlan(isUnique, projectOp_obj, prodOp_obj, orderby_list);
			}
			return Optimized_QP;
}
