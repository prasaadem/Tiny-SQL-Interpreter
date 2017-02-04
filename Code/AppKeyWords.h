#ifndef _APPKEYWORDS_H
#define _APPKEYWORDS_H

#include<string>

#include "AppTokenType.h"

class AppKeyWords{
public:

  AppKeyWords()
  {
      keywordMapping();
  }

  int KeywordToken(string value)
  {
      
      return mappedKeyword[value];
  }

  private:
  
  map<string,int> mappedKeyword;

  void keywordMapping()
  {
      mappedKeyword["SELECT"]=SELECT;
      mappedKeyword["DISTINCT"] = DISTINCT;
      mappedKeyword["FROM"] = FROM;
      mappedKeyword["WHERE"]=WHERE;
      mappedKeyword["AND"] = AND;
      mappedKeyword["NOT"]=NOT;
      mappedKeyword["OR"]=OR;
      mappedKeyword["ORDER"]=ORDER;
      mappedKeyword["BY"] = BY;
      mappedKeyword["CREATE"] = CREATE;
      mappedKeyword["TABLE"]=TABLE;
      mappedKeyword["INSERT"]=INSERT;
      mappedKeyword["INTO"]=INTO;
      mappedKeyword["VALUES"]=VALUES;
      mappedKeyword["DELETE"] = DELETE;
      mappedKeyword["DROP"] = DROP;
      mappedKeyword["INT"]=INT_T;
      mappedKeyword["NULL"] = NULL_T;
      mappedKeyword["STR20"]=STR20_T;

  }
};

#endif