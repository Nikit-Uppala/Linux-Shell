#include "history.h"
#include<string.h>
void updateHistory(char* historyPath,queue q)
{
    FILE* file = fopen(historyPath,"w");
    list* temp = q.front;
    while(temp!=NULL)
    {
        fprintf(file,"%s\n",temp->command);
        temp = temp->next;
    }
    fclose(file);
}
void executeHistory(char* command[],int num,queue q,int inHistory,int* exitCode)
{
    if(num==1)
    {
        int limit =10;
        list* temp = q.front;
        while(inHistory>limit)
        {
            temp=temp->next;
            inHistory--;
        }
        while(temp!=NULL)
        {
            printf("%s\n",temp->command);
            temp=temp->next;
        }
    }
    else if(num==2)
    {
        int limit;
        if(command[num-1][0]<'0' || command[num-1][0]>'9')
        {
            fprintf(stderr,"history: invalid argument %s\n",command[1]);
            *exitCode=1;
            return;
        }
        if(strlen(command[1])==2 && strcmp("10",command[1])==0)
            limit=10;
        else if(strlen(command[1])==1)
            limit = command[1][0]-'0';
        else
        {
            fprintf(stderr,"argument '%s' provided cannot be greater than 10\n",command[1]);
            *exitCode=1;
            return;
        }
        list* temp=q.front;
        while(inHistory>limit)
        {
            temp=temp->next;
            inHistory--;
        }
        while(temp)
        {
            printf("%s\n",temp->command);
            temp=temp->next;
        }
    }
    else
        *exitCode=1,fprintf(stderr,"history: too many arguments provided\n");
}