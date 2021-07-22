#include "jobs.h"
#include<stdio.h>
#include<string.h>
void jobs(LL* head,int count,int* exitCode)
{
    if(count!=1)
    {
        fprintf(stderr,"jobs: too many arguments\n");
        *exitCode=1;
        return;
    }
    LL* temp = head;
    char status[1001],stat;
    while(temp!=NULL)
    {
        sprintf(status,"/proc/%d/stat",temp->pid);
        FILE* file = fopen(status,"r");
        if(file==NULL)
        {
            perror("fopen");
            *exitCode=1;
            return;
        }
        for(int i=0;i<3;i++)
            fscanf(file,"%s",status);
        stat = status[0];
        if(stat == 'R' || stat == 'S')
            strcpy(status,"Running");
        else
            strcpy(status,"Stopped");
        fclose(file);
        printf("[%d] %s %s [%d]\n",temp->jobNumber,status,temp->name,temp->pid);
        temp=temp->next;
    }
}