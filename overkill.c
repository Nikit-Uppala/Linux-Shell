#include "overkill.h"
#include<signal.h>
#include<stdlib.h>
void overkill(LL** head,int* exitCode)
{
    LL* temp = *head;
    while(temp!=NULL)
    {
        kill(temp->pid,SIGKILL);
        temp = temp->next;
    }
}