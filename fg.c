#include "fg.h"
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<wait.h>
void fg(char* command[],int count,LL** head,int* bgctr,int* currentFG,char pname[],int* exitCode)
{
    int jobnum=0,len=strlen(command[1]);
    for(int i=0;i<len;i++)
    {
        if(command[1][i]<'0' || command[1][i]>'9')
        {
            fprintf(stderr,"fg: invalid job number-%s\n",command[1]);
            *exitCode=1;
            return;
        }
        jobnum=jobnum*10 + (command[1][i]-'0');
    }
    LL* node = get(*head,-1,jobnum);
    if(node==NULL)
    {
        fprintf(stderr,"fg: invalid job number-%d\n",jobnum);
        *exitCode=1;
        return;
    }
    pid_t pid = node->pid;
    len = strlen(node->name);
    for(int i=0;i<len;i++)
        pname[i]=node->name[i];
    pname[len]='\0';
    printf("%s\n",pname);
    if(node->jobNumber==*bgctr)
        (*bgctr)--;
    *currentFG = pid;
    int status;
    signal(SIGTTOU,SIG_IGN);
    kill(pid,SIGCONT);
    tcsetpgrp(0,getpgid(pid));
    signal(SIGTTOU,SIG_DFL);
    waitpid(pid,&status,WUNTRACED);
    if(WIFSTOPPED(status))
    {
        *head = push(*head,pid,node->name,++(*bgctr));
        printf("[%d]\tStopped\t%s[%d]\n",*bgctr,node->name,pid);
    }
    signal(SIGTTOU,SIG_IGN);
    tcsetpgrp(0,getpgid(getpid()));
    signal(SIGTTOU,SIG_DFL);
    *currentFG = -1;
    *head = pop(*head,node);
    return;
}