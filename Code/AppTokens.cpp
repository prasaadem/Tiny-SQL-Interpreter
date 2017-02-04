#include <iostream>
#include<cstring>
#include"AppTokens.h"

AppTokens::AppTokens():tokenType(0)
{
  strcpy(tokenValue,"");
}

AppTokens::AppTokens(int tokentype,const char value[])
{
   tokenType = tokentype;
    strcpy(tokenValue,value);
}