#ifndef _RELATIONSENDER_H
#define _RELATIONSENDER_H

#include<vector>
#include<string>

#include"Tuple.h"

using namespace std;

struct RelationSender
{
	string relation_name;
	bool marked_For_Delete;
	vector<Tuple> tuples;
};

#endif