#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H
typedef struct linkedlist
{
    int pid;
    int jobNumber;
    char name[101];
    struct linkedlist* next;
}LL;
LL* push(LL*,int,char*,int);
LL* get(LL*,int,int);
LL* pop(LL*,LL*);
#endif