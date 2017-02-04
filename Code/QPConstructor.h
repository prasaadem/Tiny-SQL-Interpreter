#ifndef _QPCONSTRUCTOR_H
#define _QPCONSTRUCTOR_H

#include <algorithm>

#include "Block.h"
#include "Config.h"
#include "Disk.h"
#include "Field.h"
#include "MainMemory.h"
#include "Relation.h"
#include "Schema.h"
#include "SchemaManager.h"

#include "Tuple.h"
#include "RelationSender.h"
#include "Process.h"
#include "TupleOperation.h"
#include "TableProcessor.h"
#include "TupleProcessor.h"

enum IDENTIFY_LEAF {TERMINAL = 1, NON_TERMINAL}; 

class QPConstructor
{
private:
		RelationSender sendRelation;
		IDENTIFY_LEAF type;
		QPConstructor* from_left_QP;
		QPConstructor* from_right_QP;
		static int saveToDB;
public:
		QPConstructor(RelationSender send_reln);
		QPConstructor(QPConstructor*& left_QP, QPConstructor*& right_QP);
		RelationSender Compute();
		static QPConstructor* Create_optimum_QP(vector<string> from_list);
		void operator delete(void *pointer);
		static void set_num_SaveCount(int count);
	  static int get_num_SaveCount();
};

#endif
