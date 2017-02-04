#ifndef LINEPARSER_H
#define LINEPARSER_H

#include<fstream>
#include<iostream>
#include<cstring>

#include"AppTokens.h"
#include"AppKeyWords.h"

using namespace std;

class LineParser
{
    private:
        char inputToParse[500000];
        const char* current_char;
        const char* previous_char;
        static AppKeyWords keyword;

    public:
        LineParser();
        LineParser(const char* inputtext);

        AppTokens GetNextToken();

        char GetNextCharacter();

        void RestorePreviousToken();
        const char* getCurrentCharacter();
        void LineParserReset(const char* reset_character);

};
#endif