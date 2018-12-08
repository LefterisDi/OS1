#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
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

   int typeOfMerc = 0;
   int numOfMercs = 0;


   key_t sem;
   int sem_id;

   key_t shm;
   int shm_id;
   Queue* sharedMem;

   key_t assemblySem;
   int assemblySemID;

   key_t assemblyKey;
   Queue* assemblyQueue;

   while(--argc > 0){

     if (strstr(*argv , "-T") != NULL){
      argv++;
      argc--;
      typeOfMerc = atoi(*argv);
     }
     else if (strstr(*argv , "-N") != NULL){
      argv++;
      argc--;
      numOfMercs = atoi(*argv);
     }

     if (argc > 0){
      argv++;
     }
   }

   sem = get_semkey("sem.Checkkey");
   sem_id = semget(sem , 0 , 0);

   assemblySem = get_semkey("sem.Assemblykey");
   assemblySemID = semget(assemblySem , 0 , 0);

   if (assemblySemID < 0){
      perror("assemblySem recovery error");
      exit(1);
   }

   switch (typeOfMerc) { //depending on what type of Mercs it accepts , it is given a queue to take items from
                           //and a queue to push items to
      case 1:
         shm = get_shmemkey("shmem.Checkkey1");
         sharedMem = connectQueue(shm);
         assemblyKey = get_shmemkey("shmem.Assemblykey1");
         assemblyQueue = connectQueue(assemblyKey);
         break;

      case 2:
         shm = get_shmemkey("shmem.Checkkey2");
         sharedMem = connectQueue(shm);
         assemblyKey = get_shmemkey("shmem.Assemblykey2");
         assemblyQueue = connectQueue(assemblyKey);
         break;

      case 3:
         shm = get_shmemkey("shmem.Checkkey3");
         sharedMem = connectQueue(shm);
         assemblyKey = get_shmemkey("shmem.Assemblykey3");
         assemblyQueue = connectQueue(assemblyKey);
         break;
   }

   for(int i = 0 ; i < numOfMercs ; i++){

      if(sem_down(sem_id , typeOfMerc-1) < 0){
         exit(1);
      }//it takes its own semaphore down , which was previously brought up from the
      // paint department(done in order to signal that an item was inserted in the queue for checking)

      if(sem_down(sem_id , 3 + typeOfMerc-1) < 0){//takes the receive queue semaphore down
         exit(1);
      }

      Merc* mercp = popFromQ(sharedMem);

      if(sem_up(sem_id , 3 + typeOfMerc-1) < 0){ //'ups' the receives queue
         exit(1);
      }

      if(sem_down(assemblySemID , 3 + typeOfMerc-1) < 0){// takes the delivery queue down (queue connected with assembly dep)
         exit(1);
      }

      insertToQ(assemblyQueue , *mercp);

      if(sem_up(assemblySemID , 3 + typeOfMerc-1) < 0){//brings it up again
         exit(1);
      }

      if(sem_up(assemblySemID , typeOfMerc-1) < 0){//brings assembly department's semaphore up to signal that an item was inserted to the queue
         exit(1);                                  //and it can be picked up
      }

      proccessingTime(typeOfMerc);

   }

   exit(0);
}


void proccessingTime(int typeOfMerc){
   double sleepTime;
   switch (typeOfMerc) {
      case 1:
         sleepTime = 3 * 1000;
         usleep((useconds_t)sleepTime);
         break;
      case 2:
         sleepTime = 8 * 1000;
         usleep((useconds_t)sleepTime);
         break;
      case 3:
         sleepTime = 5 * 1000;
         usleep((useconds_t)sleepTime);
         break;
   }
}
