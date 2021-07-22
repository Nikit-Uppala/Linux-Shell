#include<stdlib.h>
#include<string.h>
#include "linkedlist.h"
LL* push(LL* head,int id,char* name,int jbn)
{
    if(head==NULL)
    {
        head = (LL*)malloc(sizeof(LL));
        head->pid = id;
        head->jobNumber=jbn;
        strcpy(head->name,name);
        head->next=NULL;
        return head; 
    }
    LL* temp = head;
    while(temp->next!=NULL)
        temp=temp->next;
    temp->next = (LL*)malloc(sizeof(LL));
    temp->next->next = NULL;
    temp->next->pid = id;
    temp->next->jobNumber=jbn;
    strcpy(temp->next->name,name);
    return head;
}
LL* get(LL* head,int pid,int jbn)
{
    if(head==NULL)
        return NULL;
    LL* temp = head;
    while(temp!=NULL && temp->pid!=pid && temp->jobNumber!=jbn)
        temp=temp->next;
    return temp;
}
LL* pop(LL* head,LL* node)
{
    if(head==NULL)
        return NULL;
    if(node==head)
    {
        head = node->next;
        free(node);
        return head;
    }
    LL* temp = head;
    LL* next = temp->next;
    while(next!=node)
    {
        temp = temp->next;
        next = next->next;
    }
    temp->next=next->next;
    free(next);
    return head;
}