// C program for array implementation of queue
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A structure to represent a queue

#include "./queue.h"
#include "../constructionDep/constructionDep.h"

struct Queue* connectQueue(key_t queueKey){

   int shmid;
   Queue* queueSharedMem;

   if ((shmid = shmget(queueKey , 0 , 0)) < 0) {
     perror("shmget!\n");
     return(0);
   }

   if((queueSharedMem = (Queue*)shmat(shmid , 0 , 0)) < 0){
      perror("shmat error!");
      return(0);
   }

   return queueSharedMem;
}

Merc* connectArray(Queue* q){ /* because the array is a pointer to shared memory
   and the proccess has to be connected to that shared memory in order to use it , we
   have to implement this function*/
   int arrayID;
   Merc* arraySharedMem;


   if ((arrayID = shmget(q->arrayKey, 0 , 0)) < 0) {
     perror("shmget!\n");
     return(0);
   }

   if((arraySharedMem = (Merc*)shmat(arrayID , 0 , 0)) < 0){
     perror("shmat error!");
     return(0);
   }

   return arraySharedMem;
}


// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned int capacity , key_t queueKey , key_t arrayKey , int* queueID)
{//creates everything needed for the queue to work properly with shared memory

   int shmid;
   Queue* sharedMem;
   Queue* queue;

   if ((shmid = shmget(queueKey, sizeof(Queue) , IPC_CREAT | 0666)) < 0) {
     perror("shmget!\n");
     return(0);
   }


   if((sharedMem = (Queue*)shmat(shmid , 0 , 0)) < 0){
      perror("shmat error!");
      return(0);
   }

   queue = sharedMem;

   int arrayID;
   Merc* arraySharedMem;

   if ((arrayID = shmget(arrayKey, capacity * sizeof(Merc), IPC_CREAT | 0666)) < 0) {
     perror("shmget!\n");
     return(NULL);
   }

   if((arraySharedMem = (Merc*)shmat(arrayID , 0 , 0)) < 0){
     perror("shmat error!");
     return(NULL);
   }

   queue->capacity = capacity;
   queue->front = queue->size = 0;
   queue->rear = capacity - 1;  // This is important, see the enqueue
   queue->array = arraySharedMem;
   queue->arrayKey = arrayKey;
   queue->arrayID = arrayID;
   *queueID = shmid;
   return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue* queue)
{ return (queue->size == queue->capacity);  }

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }

// Function to add an item to the queue.
// It changes rear and size
int insertToQ(struct Queue* queue, Merc item)
{
    if (isFull(queue))
        return -1;


    Merc* array = connectArray(queue);
    queue->rear = (queue->rear + 1)%queue->capacity;
    array[queue->rear] = item;
    queue->size = queue->size + 1;
    return 0;
}

// Function to remove an item from queue.
// It changes front and size
Merc* popFromQ(struct Queue* queue)
{
    if (isEmpty(queue)){
      sleep(10);
      return NULL;
    }
    Merc* array = connectArray(queue);
    Merc* item = &array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;

    queue->size = queue->size - 1;
    return item;
}

int deleteArray(Queue* q){ //deletes queue array
   if(shmctl(q->arrayID , IPC_RMID , 0) < 0){
      perror("shared memory delete error!");
      return -1;
   }
   return 0;
}
