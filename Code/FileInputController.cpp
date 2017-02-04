#include <fstream>
#include <cstring>
#include "FileInputController.h"

char* FileInputController::GetFile(char* inputfile)
{
  
  ifstream in(inputfile);
  if(!in)
  {
      cout<<"Cannot access the inputfile " << inputfile<<endl;
      return NULL;
  }
  char sentencelength[255];
  static char completefile[500000]= " ";
  while(in.getline(sentencelength,255))
  {
      strcat(completefile,sentencelength);
      strcat(completefile,"\n");
  }
  return completefile;
}


string FileInputController::GetFileString(char* inputfile)
{
  ifstream in(inputfile);
  if(!in)
  {
      cout<<"Cannot access the String inputfile " << inputfile<<endl;
      string s = " ";
      return s;
  }

  char sentencelength[255];
  string completefileString= " ";
  while(in.getline(sentencelength,255))
  {
      completefileString.append(sentencelength);
      completefileString.append("\n");
  }
  return completefileString;
}
