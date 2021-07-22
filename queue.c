#include<stdlib.h>
#include<string.h>
#include "queue.h"
queue enQueue(queue q,char* command)
{
    if(q.front==NULL && q.back==NULL)
    {
        q.front = (list*)malloc(sizeof(list));
        q.front->next = NULL;
        strcpy(q.front->command,command);
        q.back = q.front;
        return q;
    }
    list* newNode = (list*)malloc(sizeof(list));
    newNode->next = NULL;
    strcpy(newNode->command,command);
    q.back->next = newNode;
    q.back = q.back->next;
    return q;
}
queue deQueue(queue q)
{
    if(!q.front && !q.back)
        return q;
    if(q.front == q.back && q.front!=NULL)
    {
        q.back = NULL;
        free(q.front);
        q.front = NULL;
        return q;
    }
    list* temp = q.front->next;
    free(q.front);
    q.front = temp;
    return q;
}