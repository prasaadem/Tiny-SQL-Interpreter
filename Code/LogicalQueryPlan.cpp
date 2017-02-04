#include"LogicalQueryPlan.h"


LogicalQueryPlan::LogicalQueryPlan()
{
	
		left_LogicalQP = NULL;
		right_LogicalQP = NULL;
		construct_QP = NULL;
		isUnique = false;
}

LogicalQueryPlan::LogicalQueryPlan(LogicalQueryPlan*& left, LogicalQueryPlan *&right)
{
		left_LogicalQP = left;
		right_LogicalQP = right;
}

void LogicalQueryPlan::Add_Left_LPQ(LogicalQueryPlan*& lp)
{
		left_LogicalQP = lp;
}
void LogicalQueryPlan::Add_Right_LPQ(LogicalQueryPlan*& rp)
{
		right_LogicalQP = rp;
}

LogicalQueryPlan* LogicalQueryPlan::CreateSelectQueryPlan(SelectOP selectop)
{
		LogicalQueryPlan* lp = new LogicalQueryPlan();
		lp->operationType = SELECT_OP;
		lp->selectOp = selectop;
		return lp;
}

LogicalQueryPlan* LogicalQueryPlan::CreateProjectQueryPlan(ProjectOP projectop)
{
		LogicalQueryPlan* lp = new LogicalQueryPlan();
		lp->operationType = PROJECT;
		lp->projectOp = projectop;
		return lp;
}

LogicalQueryPlan* LogicalQueryPlan::CreateJoinQueryPlan(JOINOP joinop)
{
		LogicalQueryPlan* lp = new LogicalQueryPlan();
		lp->operationType = VECTORPRODUCT;
		lp->productOp = joinop;
		return lp;
}

LogicalQueryPlan* LogicalQueryPlan::CreateProjSelJoinQueryPlan(bool isDistinct, ProjectOP projectop, SelectOP& selectop, JOINOP joinop, vector<MyColumn> ordrBy)
{
		LogicalQueryPlan* lp = new LogicalQueryPlan();
		lp->operationType = Optimum_PROJECT_S_PRODUCT_SELECT;
		lp->projectOp = projectop;
		lp->selectOp = selectop;
		lp->productOp = joinop;
		lp->isUnique = isDistinct;
		lp->order_BY = ordrBy;
		vector<string> from_list = joinop.GetFromList();
		if(from_list.size() >1)
		{
				QPConstructor::set_num_SaveCount(from_list.size() - 2);
				QPConstructor* construct_QP = QPConstructor::Create_optimum_QP(from_list);
				lp->construct_QP = construct_QP;
				lp->operationType = Optimum_PROJECT_M_PRODUCT;
		}
		return lp;
}

LogicalQueryPlan* LogicalQueryPlan::CreateProjJoinQueryPlan(bool isDistinct, ProjectOP projectop, JOINOP joinop, vector<MyColumn> orderBy)
{
		LogicalQueryPlan* lp = new LogicalQueryPlan();
		vector<string> table_names = joinop.GetFromList();
		lp->isUnique = isDistinct;
		lp->order_BY = orderBy;
		if(table_names.size() == 1)
		{
				lp->productOp = joinop;
				lp->projectOp = projectop;
				lp->operationType = Optimum_PROJECT_S_PRODUCT;
		}
		else
		{
				QPConstructor* construct_QP = QPConstructor::Create_optimum_QP(table_names);
				QPConstructor::set_num_SaveCount(table_names.size() - 2);
				//Save temp tables to database
				lp->productOp = joinop;
				lp->projectOp = projectop;
				lp->construct_QP = construct_QP;
				lp->operationType = Optimum_PROJECT_M_PRODUCT;
		}
		return lp;
}

LogicalQueryPlan* LogicalQueryPlan::FetchOptimizedQP()
	{
		LogicalQueryPlan* optimized_plan = new LogicalQueryPlan();
		switch(operationType)
		{
		case PROJECT:
			optimized_plan->projectOp = projectOp;

			if(left_LogicalQP != NULL)
			{
				LogicalQueryPlan*& arguments_lp = left_LogicalQP;
				if(arguments_lp->operationType == VECTORPRODUCT)
				{
					vector<string> table_names = arguments_lp->productOp.GetFromList();
					if(table_names.size() == 1)
					{
						optimized_plan->productOp = arguments_lp->productOp;
						optimized_plan->operationType = Optimum_PROJECT_S_PRODUCT;

					}
				}
				else if(arguments_lp->operationType == SELECT_OP)
				{

					SelectOP& selectop = arguments_lp->selectOp;
					optimized_plan->selectOp = selectop;
					if(arguments_lp->left_LogicalQP != NULL)
					{
						LogicalQueryPlan*& argument_lp2 = arguments_lp->left_LogicalQP;
						if(argument_lp2->operationType == VECTORPRODUCT)
						{
							vector<string> table_names = argument_lp2->productOp.GetFromList();
							if(table_names.size() == 1)
							{
								
								optimized_plan->productOp = argument_lp2->productOp;
								optimized_plan->operationType = Optimum_PROJECT_S_PRODUCT;
							}
						}
						else {
							throw string("Select Argument must have a product");
						}
					}
					else
					{
						throw string("Exception - Select Operation failed - Invalid arguments");
					}
				}
			}
			else
				throw string("Exception - Project Operation failed - Invalid arguments");
			break;
		default:	
			break;
		}
		return optimized_plan;
	}

RelationSender LogicalQueryPlan::Execute()
{
	RelationSender retn_relation;
	switch(operationType)
	{
		case Optimum_PROJECT_S_PRODUCT:
		{
			if(projectOp.fetchcolumn().front().colName == "*")
			{

				string relation = productOp.GetFromList().front();
				vector<Tuple> tuples = TupleOperation::getTuples(isUnique, relation, 0, order_BY);
				retn_relation.marked_For_Delete = false;
				retn_relation.relation_name = relation;
				retn_relation.tuples = tuples;
			}
			else
			{
				string relation = productOp.GetFromList().front();
				string tempTable = TableProcessor::rand_table();
				vector<Tuple> tuples = TupleOperation::getTuples(isUnique, projectOp, selectOp, relation, 0, tempTable, order_BY);
				retn_relation.relation_name = tempTable;
				retn_relation.marked_For_Delete = true;
				retn_relation.tuples = tuples;
			}
			break;
		}
		case Optimum_PROJECT_S_PRODUCT_SELECT:
		{
			if(projectOp.fetchcolumn().front().colName == "*")
			{
				string relation = productOp.GetFromList().front();
				vector<Tuple> tuples = TupleOperation::GetSelectTuples(isUnique, relation, 0, selectOp, order_BY);
				retn_relation.marked_For_Delete = false;
				retn_relation.relation_name = relation;
				retn_relation.tuples = tuples;
			}
			else
			{

				string relation = productOp.GetFromList().front();
				string tempTable = TableProcessor::rand_table();
				vector<Tuple> tuples = TupleOperation::getTuples(isUnique, projectOp, selectOp, relation, 0, tempTable, order_BY);
				retn_relation.relation_name = tempTable;
				retn_relation.marked_For_Delete = true;
				retn_relation.tuples = tuples;
			}
			break;
		}
		case Optimum_PROJECT_M_PRODUCT:
		{
			sendRelation = construct_QP->Compute();
			bool persist = false;
			if(selectOp.Issearch_cond())
				sendRelation.tuples = TupleOperation::ExecuteSelectOP(selectOp, sendRelation);
			sendRelation.tuples = TupleOperation::GetUniqueOrderedTuples(sendRelation.tuples, isUnique, projectOp, sendRelation.relation_name, 0, order_BY);
			if(projectOp.fetchcolumn().front().colName != "*")
				sendRelation = TupleOperation::ExecuteProjectOP(projectOp, productOp, sendRelation, persist);
			retn_relation = sendRelation;
			break;
		}
		default:	
			break;
	}
	return retn_relation;
}

void LogicalQueryPlan::operator delete(void *pointer)
{
		if(pointer!=NULL)
		{
				if(((LogicalQueryPlan*)pointer)->left_LogicalQP != NULL)
				delete ((LogicalQueryPlan*)pointer)->left_LogicalQP;

				if(((LogicalQueryPlan*)pointer)->right_LogicalQP != NULL)
				delete ((LogicalQueryPlan*)pointer)->right_LogicalQP;
				free(pointer);
		}
}
