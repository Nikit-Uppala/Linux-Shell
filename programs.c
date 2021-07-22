#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include "programs.h"
#include<stdlib.h>
#include<string.h>
void executeProg(char* prog[],int ctr,int bg,int* bgctr,LL** head,int* currentFG,char pname[],int* exitCode)
{
    char name[1001]="";
    for(int i=0;i<ctr;i++)
    {
        strcat(name,prog[i]);
        strcat(name,(i!=ctr-1)?" ":"\0");
    }
    int len = strlen(name);
    for(int i=0;i<len;i++)
        pname[i]=name[i];
    pname[len]='\0';
    int child=fork();
    if(child==-1)
        perror("Program"),*exitCode=1;
    else if(child==0)
    {
        if(bg==1)
            setpgid(0,0);
        if(execvp(prog[0],prog)==-1)
            perror(prog[0]);
        exit(1);
    }
    else
    {
        if(bg==0)
        {
            *currentFG=child;
            int status;
            waitpid(child,&status,WUNTRACED);
            *currentFG = -1;
        }
        if(bg==1)
        {
            *head=push(*head,child,name,++(*bgctr));
            printf("[%d] %d\n",(*bgctr),child);
        }
    }
    return;
}