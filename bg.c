#include "bg.h"
#include<string.h>
#include<signal.h>
#include<stdio.h>
void execute_bg(char* command[],int count,LL* head)
{
    for(int i=1;i<count;i++)
    {
        int jobnum=0,len=strlen(command[i]);
        for(int j=0;j<len;j++)
        {
            if(command[i][j]<'0' || command[i][j]>'9')
            {
                fprintf(stderr,"bg: invalid job number-%s\n",command[i]);
                break;
            }
            jobnum = jobnum*10 + (command[i][j]-'0');
        }
        LL* node = get(head,-1,jobnum);
        if(node==NULL)
        {
            fprintf(stderr,"bg: no job number -%d\n",jobnum);
            continue;
        }
        if(kill(node->pid,SIGCONT)==-1)
            perror("bg");
        else
            printf("[%d]+ %s\n",jobnum,node->name);
    }
}