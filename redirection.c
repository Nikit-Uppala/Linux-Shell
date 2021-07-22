#include "redirection.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
void checkRedirection(char* command[],int* count,char inpFile[],char outFile[],int* app,char* finalcommand[])
{
    int start=0;
    for(int i=0;i<*count;i++)
    {
        if(command[i]==NULL)
            continue;
        if(i<*count-1 && strcmp(command[i],">>")==0)
        {
            int len = strlen(command[i+1]);
            for(int j=0;j<len;j++)
                outFile[j]=command[i+1][j];
            outFile[len]='\0';
            *app=1;
            free(command[i]);
            free(command[i+1]);
            command[i]=NULL;
            command[i+1]=NULL;
            i++;
        }
        else if(i<*count-1 && strcmp(command[i],">")==0)
        {
            int len = strlen(command[i+1]);
            for(int j=0;j<len;j++)
                outFile[j]=command[i+1][j];
            outFile[len]='\0';
            free(command[i]);
            free(command[i+1]);
            command[i]=NULL;
            command[i+1]=NULL;
            i++;
        }
        else if(i<*count-1 && strcmp(command[i],"<")==0)
        {
            int len = strlen(command[i+1]);
            for(int j=0;j<len;j++)
                inpFile[j]=command[i+1][j];
            inpFile[len]='\0';
            free(command[i]);
            free(command[i+1]);
            command[i]=NULL;
            command[i+1]=NULL;
            i++;
        }
        else
        {
            finalcommand[start]=calloc(strlen(command[i]),sizeof(char));
            strcpy(finalcommand[start++],command[i]);
            free(command[i]);
            command[i]=NULL;
        }
    }
    finalcommand[start]=NULL;
    *count = start;
}