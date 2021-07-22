#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/utsname.h>
#include<sys/wait.h>
#include<string.h>
#include "cd.h"
#include "strip.h"
#include "checkhome.h"
void executeCD(char* command[],char home[],char pwd[],char prev[],int argCount,int* exitCode)
{
    if(argCount>2)
    {
        fprintf(stderr,"cd: too many arguments\n");
        *exitCode = 1;
        getcwd(pwd,1001);
    }
    if(argCount==1)
    {
        if(chdir(home)==-1)
            perror("cd"),*exitCode=1;
        else
        {
            int len = strlen(pwd);
            for(int i=0;i<len;i++)
                prev[i] = pwd[i];
            prev[len]='\0';
            getcwd(pwd,1001);
        }
        return;
    }
    if(strcmp(command[1],"-")==0)
    {
        if(strcmp(prev,"\0")==0)
            fprintf(stderr,"cd: no previous directory set\n"),*exitCode = 1;
        else
        {
            if(chdir(prev)==-1)
            {
                perror("cd");
                *exitCode=1;
                return;
            }
            printf("%s\n",prev);
            int len = strlen(pwd);
            for(int i=0;i<len;i++)
                prev[i] = pwd[i];
            prev[len]='\0';
            getcwd(pwd,1001);
        }
        return;
    }
    char path[1001];
    checkhome(home,command[1],path);
    if(chdir(path)==-1)
    {
        perror("cd");
        *exitCode = 1;
        return;
    }
    int len = strlen(pwd);
    for(int i=0;i<len;i++)
        prev[i] = pwd[i];
    prev[len]='\0';
    getcwd(pwd,1001);
}