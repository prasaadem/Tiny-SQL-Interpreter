//C++ Headers
#include <iostream>
#include <fstream>
#include <exception>
#include<limits>
#include <stdio.h>
#include <time.h>
#include <math.h>

//Custom Headers
#include "FileInputController.h"
#include "Global.h"
#include "Parser.h"

using namespace std;
FileInputController fileInputController;
Parser parser;

int main()
{
    char* input = NULL;
    char line[300];
    string inputOption;
    string outputOption;
    string queryFileName;
    char* cstrQueryFileName;
    char* cstrOutputOption;
    int outOption;

    ofstream outputQueryFile;
    outputQueryFile.open("queryOutput", ofstream::out);

    time_t startTime;
    time_t endTime;
    double seconds;
    bool validInput=false;

    for( ; ; )
    {
        clock_t startTime = clock();

        cout << "### <------------------------------------------------------------------------> ###" << endl;
        cout <<"[1] ---> To Read TinySQL Queries from a File" << endl << "[2] ---> To Read TinySQL Query from the Command Prompt" << endl << "[3] ---> To Exit this Application"; 
        cout << endl;

        cout<<"Please Enter an option from [1] to [3]: ";
        cin>>inputOption;

        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        cout << "### <------------------------------------------------------------------------> ###" << endl;

        if(inputOption == "1")
        {
            cout << "Please Enter the TinySQL Query File Name: ";
            cin >> queryFileName;

            cout << "[1] ---> Print Output to Screen" << endl;
            cout << "[2] ---> Print Output to File (query_output)" << endl;
            cout<<"Please Enter an option from [1] to [2]: ";
            cin >> outputOption;

            cstrOutputOption = new char[outputOption.length() + 1];
            strcpy(cstrOutputOption, outputOption.c_str());
            outOption = atoi(cstrOutputOption);

            cstrQueryFileName = new char[queryFileName.length() + 1];
            strcpy(cstrQueryFileName, queryFileName.c_str());
            input = fileInputController.GetFile(cstrQueryFileName);

            if(outOption == 2) {
                outputQueryFile << "Output of TinySQL Query File: " << queryFileName << endl;
                cout << "### <------------------------------------------------------------------------> ###" << endl;
            }

            if(input == NULL)
            {
                cout<<">> Press any key to quit";
                getchar();
                exit(1);
            }
            validInput = true;
            if(outOption == 1) {
                cout << "### <------------------------------------------------------------------------> ###" << endl;
            }
            else if(outOption == 2) {
                cout << "### <------------------------------------------------------------------------> ###" << endl;
            }
            else {
                validInput = false;
            }
        }
        else if(inputOption == "2")
        {

            cout << ">> Please Enter the TinySQL statement: ";
            cin.getline(line,255);

            cout << "[1] ---> Print Output to Screen" << endl;
            cout << "[2] ---> Print Output to File (query_output)" << endl;
            cout<<"Please Enter an option from [1] to [2]: ";
            cin >> outputOption;

            cstrOutputOption = new char[outputOption.length() + 1];
            strcpy(cstrOutputOption, outputOption.c_str());
            outOption = atoi(cstrOutputOption);

            input = line;
            validInput = true;
            if(outOption == 1 || outOption == 2) 
            {
                cout << "### <------------------------------------------------------------------------> ###" << endl;
            }
            else {
                validInput = false;
            }
        }
        else if(inputOption == "3")
        {
            validInput = true;
            exit(EXIT_SUCCESS);
        }
        else
        {
            validInput = false;
            cout<<"Please enter a valid option" << endl;
        }

        if(validInput) {
            try
            {
                parser = Parser(input);
                parser.ParserInput(outputQueryFile, outOption);
                parser.Clear();
            }
            catch(string error)
            {
                cout<<"An Error occured while working with data: "<<error<<endl;
                parser.Clear();
            }
            catch(...)
            {
                cout<<"We are sorry! An Unknown Error occured"<<endl;
                parser.Clear();
            }
            endTime = clock() - startTime;
            if(outOption == 1) {
                cout << "Total Time Taken in seconds: ";
                cout << ((float)endTime)/(CLOCKS_PER_SEC)<<endl;
            }
            else if(outOption == 2) {
                outputQueryFile << "Total Time Taken in seconds: ";
                outputQueryFile << ((float)endTime)/(CLOCKS_PER_SEC)<<endl;
            }
        }
    }

    outputQueryFile.close();

    getchar();
}