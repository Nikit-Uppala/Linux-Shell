#include "kjob.h"
#include<signal.h>
#include<string.h>
#include<stdio.h>
void kjob(char* command[],int count,LL* head,int* exitCode)
{
    if(count!=3)
    {
        fprintf(stderr,"kjob: number of arguments expected-2 given-%d\n",count-1);
        *exitCode=1;
        return;
    }
    int jobnum=0,len=strlen(command[1]);
    for(int i=0;i<len;i++)
    {
        if(command[1][i]<'0' || command[1][i]>'9')
        {
            fprintf(stderr,"%s: invalid job number-%s\n","kjob",command[1]);
            *exitCode=1;
            return;
        }
        jobnum=jobnum*10 + (command[1][i]-'0');
    }
    LL* node = get(head,-1,jobnum);
    if(node==NULL)
    {
        fprintf(stderr,"%s: invalid job number-%d\n","kjob",jobnum);
        *exitCode=1;
        return;
    }
    int signalnum=0;len=strlen(command[2]);
    for(int i=0;i<len;i++)
    {
        if(command[2][i]<'0' || command[2][i]>'9')
        {
            fprintf(stderr,"kjob: invalid signal number-%s\n",command[2]);
            *exitCode=1;
            return;
        }
        signalnum=signalnum*10 + (command[2][i]-'0');
    }
    if(signalnum<1 || signalnum>31)
    {
        fprintf(stderr,"kjob: invalid signal number-%d\n",signalnum);
        *exitCode=1;
        return;
    }
    if(kill(node->pid,signalnum)==-1)
        perror("kjob"),*exitCode=1;
    else
        printf("%d signal sent to job number-%d\n",signalnum,jobnum);
}