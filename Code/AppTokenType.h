#ifndef _APPTOKENTYPE_H
#define _APPTOKENTYPE_H

#include<string>

#include<map>

using namespace std;

enum
{
      AND = 1,
      BY = 2, 
      COMPOPERATOR = 3,
      COMMA = 4,
      CREATE = 5,
      DELETE = 6,
      DISTINCT = 7,
      DIVISION = 8,
      PERIOD = 9,
      DROP = 10,
      ENCLOSINGSQPAREN = 11,
      ENCLOSINGPAREN = 12,
      FROM = 13,
      IDENTIFIER = 14,
      INSERT = 15,
      INT_T = 16,
      INTO = 17,
      LITERAL = 18,
      MINUS = 19,
      NOT = 20,
      NULL_T =21,
      INTEGER = 22,
      OPENINGSQPAREN = 23,
      OPENINGPAREN = 24,
      OR = 25,
      ORDER = 26,
      ADDITION = 27,
      SELECT = 28,
      SEMICOLON = 29,
      STR20_T = 30,
      MULTIPLY = 31,
      TABLE = 32,
      UNDEFINEDTOK = 33,
      VALUES = 34,
      WHERE = 35
};
#endif
