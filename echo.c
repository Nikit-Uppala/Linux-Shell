#include<stdio.h>
#include "echo.h"
void executeEcho(char* command[],int argCount,int* exitCode)
{
    if(argCount==1)
    {
        printf("\n");
        return;
    }
    for(int i=1;i<argCount;i++)
        printf("%s%c",command[i],(i!=argCount-1)?' ':'\n');
}