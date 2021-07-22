#include<stdlib.h>
#include<stdio.h>
#include "unsetenv.h"
void unsetEnv(char* command[],int count,int* exitCode)
{
    if(count==1)
    {
        fprintf(stderr,"unsetenv: Too less arguments\n");
        *exitCode=1;
        return;
    }
    else if(count>2)
    {
        fprintf(stderr,"unsetenv: Too many arguments\n");
        *exitCode=1;
        return;
    }
    if(unsetenv(command[1])==-1)
    {
        *exitCode=1,perror("unsetenv");
        return;
    }
    printf("environment variable %s destroyed\n",command[1]);
}