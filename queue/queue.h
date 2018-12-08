#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "../constructionDep/constructionDep.h"

typedef struct Queue
{
    int front;
    int rear;
    int size;
    key_t arrayKey;
    int arrayID;
    unsigned int capacity;
    Merc* array;
}Queue;

struct Queue* connectQueue(key_t);
struct Queue* createQueue(unsigned int , key_t , key_t , int*);
int insertToQ(struct Queue*, Merc);
Merc* popFromQ(struct Queue*);
int deleteArray(Queue*);

#endif
