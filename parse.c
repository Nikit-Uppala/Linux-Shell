#include<string.h>
#include<stdlib.h>
#include "parse.h"
void parse(char command[],char* delim,char* parsed[],int *ind)
{
    for(char* token=strtok(command,delim);token!=NULL;token=strtok(NULL,delim))
    {
        parsed[*ind] = (char*)calloc(strlen(token),sizeof(char));
        strcpy(parsed[(*ind)++],token);
    }
    parsed[*ind]=NULL;
}