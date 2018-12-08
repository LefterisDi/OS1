#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "../constructionDep/constructionDep.h"
#include "../semaphores/semCtrl.h"
#include "../sharedMemory/shmCtrl.h"
#include "../queue/queue.h"

void proccessingTime(int);

int main(int argc , char* argv[]){

   double averageWaitTime = 0.0;

   int numOfMercs;

   key_t sem;
   int sem_id;

   key_t shm;
   int shm_id;
   Queue* sharedMem;


   key_t checkSem;
   int checkSemID;

   key_t checkKey1;
   Queue* checkQueue1;

   key_t checkKey2;
   Queue* checkQueue2;

   key_t checkKey3;
   Queue* checkQueue3;

   int shmTime;
   key_t shmemTimeKey;
   struct timeval* shmTimePointer;

   sem = get_semkey("sem.Paintkey");

   sem_id = semget(sem , 0 , 0);

   if (sem_id < 0){
      perror("checkSem recovery error");
      exit(1);
   }

   shm = get_shmemkey("shmem.Paintkey");
   sharedMem = connectQueue(shm);

   checkSem = get_semkey("sem.Checkkey");
   checkSemID = semget(checkSem , 0 , 0);

   if (checkSemID < 0){
      perror("checkSem recovery error");
      exit(1);
   }

   checkKey1 = get_shmemkey("shmem.Checkkey1");
   checkQueue1 = connectQueue(checkKey1);

   checkKey2 = get_shmemkey("shmem.Checkkey2");
   checkQueue2 = connectQueue(checkKey2);

   checkKey3 = get_shmemkey("shmem.Checkkey3");
   checkQueue3 = connectQueue(checkKey3);

   shmemTimeKey = get_shmemkey("shmem.Timekey");
   shmTime = shmget(shmemTimeKey , 0 , 0);
   if((shmTimePointer = (struct timeval*)shmat(shmTime , 0 , 0)) < 0){
     perror("shmat error!");
     return(0);
   }


   while(--argc > 0){

     if (strstr(*argv , "-N") != NULL){
      argv++;
      argc--;
      numOfMercs = atoi(*argv);
     }

     if (argc > 0){
      argv++;
     }
   }

   double elapsedTime;
   struct timeval t2;

   for (int i = 0 ; i < 3*numOfMercs ; i++){


      if(sem_down(sem_id , 1) < 0){//it takes its own semaphore down , which was previously brought up from the
            // construction department(done in order to signal that an item was inserted in the queue for (painting)
         exit(1);
      }


      if(sem_down(sem_id , 0) < 0){ //takes the queue's semaphore down in order to pull the first Merc from it
        exit(1);
     }

      Merc* mercp = popFromQ(sharedMem);

      gettimeofday(&t2 , NULL);

      elapsedTime = (t2.tv_sec - shmTimePointer->tv_sec) * 1000.0;      // sec to ms
      elapsedTime += (t2.tv_usec - shmTimePointer->tv_usec) / 1000.0;  //us to ms

      elapsedTime = elapsedTime - mercp->timeStamp;

      averageWaitTime += elapsedTime;

      if(sem_up(sem_id , 0) < 0){ //ups the queue's semaphore so that the construction department can use it again
         exit(1);
      }


      switch (mercp ->type) {
         case 1:
            if(sem_down(checkSemID , 3) < 0){ // takes down the queue's semaphore(queue between check department and paint dep)
               exit(1);
            }
            if(insertToQ(checkQueue1 , *mercp) < 0){
               perror("Queue Full\n");
               exit(1);
            }
            if(sem_up(checkSemID , 3) < 0){//ups the semaphore
               exit(1);
            }
            if(sem_up(checkSemID , 0) < 0){//ups the semaphore of the particular department which is responsible for handling this type of Mercs
               exit(1);
            }
            break;

         case 2:
            if(sem_down(checkSemID , 4) < 0){// takes down the queue's semaphore(queue between check department and paint dep)
               exit(1);
            }
            if(insertToQ(checkQueue2 , *mercp) < 0){
               perror("Queue Full\n");
               exit(1);
            }
            if(sem_up(checkSemID , 4) < 0){ //ups the semaphore
               exit(1);
            }
            if(sem_up(checkSemID , 1) < 0){//ups the semaphore of the particular department which is responsible for handling this type of Mercs
               exit(1);
            }
            break;

         case 3:
            if(sem_down(checkSemID , 5) < 0){// takes down the queue's semaphore(queue between check department and paint dep)
               exit(1);
            }
            if(insertToQ(checkQueue3 , *mercp) < 0){
               perror("Queue Full\n");
               exit(1);
            }
            if(sem_up(checkSemID , 5) < 0){//ups the semaphore
               exit(1);
            }
            if(sem_up(checkSemID , 2) < 0){//ups the semaphore of the particular department which is responsible for handling this type of Mercs
               exit(1);
            }
            break;
      }

      proccessingTime(mercp ->type); // makes the proccess sleep for some time 
   }

   averageWaitTime = averageWaitTime / (3.0*numOfMercs);


   printf("Average Waiting Time for painting department(in ms) : %f\n", averageWaitTime);

 exit(0);
}

void proccessingTime(int typeOfMerc){
   double sleepTime;
   switch (typeOfMerc) {
      case 1:
         sleepTime = 10 * 1000;
         usleep((useconds_t)sleepTime);
         break;
      case 2:
         sleepTime = 4 * 1000;
         usleep((useconds_t)sleepTime);
         break;
      case 3:
         sleepTime = 7 * 1000;
         usleep((useconds_t)sleepTime);
         break;
   }
}
