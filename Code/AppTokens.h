#ifndef _APPTOKENS_H
#define _APPTOKENS_H

#include<string>

using namespace std;

class AppTokens
{
	public:
	int tokenType;
	char tokenValue[80];
	AppTokens();
	AppTokens(int tokentype, const char value[]);
};

#endif