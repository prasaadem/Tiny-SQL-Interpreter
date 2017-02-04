#ifndef _TUPLE_H
#define _TUPLE_H

#include <vector>

#include "Field.h"
#include "Schema.h"

using namespace std;

class SchemaManager;

class Tuple {
  private:
  SchemaManager* schema_manager;
  int schema_index; // points to the schema of the relation which the tuple belongs to
  vector<union Field> fields;  // stores integer and string fields
  // DO NOT use the constructor here. Create an empty tuple only through Schema
  Tuple(SchemaManager* schema_manager, int schema_index);
  static Tuple getDummyTuple(); // for internal use: returns an invalid tuple

  public:
  friend class Relation; // creates a tuple
  friend class Block; // clears the tuple

  bool isNull() const; //returns true if the tuple is invalid
  Schema getSchema() const; // returns the schema of the tuple
  int getNumOfFields() const; // returns the number of fields in the tuple
  int getTuplesPerBlock() const; // returns the number: tuples per block

  void null(); // invalidates the tuple
  bool setField(int offset,string s); // returns false if the type is wrong or out of bound
  bool setField(int offset,int i); // returns false if the type is wrong or out of bound
  bool setField(string field_name, string s); // returns false if the type is wrong or the name is not found
  bool setField(string field_name, int i); // returns false if the type is wrong or the name is not found
  union Field getField(int offset) const; // returns INT_MIN if out of bound
  union Field getField(string field_name) const; // returns INT_MIN if the name is not found

  void printTuple() const; // prints the field values without printing the field names
  void printTuple(ostream& out) const;
  void printTuple(bool print_field_names) const; // prints the field values along with field names if TRUE
  void printTuple(bool print_field_names, ostream& out) const;
  friend ostream &operator<<(ostream &out, const Tuple &t);
};
#endif
