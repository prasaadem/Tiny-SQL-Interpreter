//Evaluation of Logical operators AND, OR, NOT in the search condition WHERE.
#ifndef _SEARCH_H
#define _SEARCH_H

#include <cstdlib>
#include <string>
#include <iostream>

#include "Compare.h"

enum Find_Operator {find_AND = 1, find_OR, find_NOT, find_SELF};


class Search {
	private:
		Find_Operator find_operator;
		Compare* compare_predicate;
		Search* search_right;
		Search* search_left;
	public:
		Search(Search*& search_left, string f_opt, Search*& search_right);
		Search(Compare*& myvalue);
		bool Compute();
		void operator delete(void *pointer);
};
#endif