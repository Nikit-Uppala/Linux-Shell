#include "linkedlist.h"
#ifndef __QUEUE_H
#define __QUEUE_H
typedef struct list
{
    char command[1001];
    struct list* next;
}list;
typedef struct queue
{
    list* front;
    list* back;
}queue;
queue enQueue(queue,char*);
queue deQueue(queue);
#endif