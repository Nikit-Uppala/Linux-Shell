#include "setenv.h"
#include<stdio.h>
#include<stdlib.h>
void setEnv(char* command[],int count,int* exitCode)
{
    if(count==1)
    {
        fprintf(stderr,"setenv: Too less arguments\n");
        *exitCode=1;
        return;
    }
    if(count>3)
    {
        fprintf(stderr,"setenv: Too many arguments\n");
        *exitCode=1;
        return;
    }
    if(count==2)
    {
        if(setenv(command[1],"",1)==-1)
        {
            *exitCode=1,perror("setenv");
            return;
        }
        printf("Environment variable %s set to empty string\n",command[1]);
    }
    else if(count==3)
    {
        if(setenv(command[1],command[2],1)==-1)
        {
            *exitCode=1,perror("setenv");
            return;
        }
        printf("Environment variable %s set to %s\n",command[1],command[2]);
    }
}