#ifndef _FILEINPUTCONTROLLER_H
#define _FILEINPUTCONTROLLER_H

#include<iostream>
#include<string>
#include<fstream>

using namespace std;

class FileInputController
{
private:
    char inputfile[100];
public:
    char* GetFile(char* inputfile);
    string GetFileString(char* inputfile);
};
#endif
