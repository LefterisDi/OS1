#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../constructionDep/constructionDep.h"
#include "../sharedMemory/shmCtrl.h"
#include "../semaphores/semCtrl.h"
#include "../queue/queue.h"

int main(int argc , char* argv[]){

   double averageCompletionTime = 0.0;

   int numOfMercs;

   key_t assemblySem;
   int assemblySemID;

   key_t assemblyKey1;
   Queue* assemblyQueue1;

   key_t assemblyKey2;
   Queue* assemblyQueue2;

   key_t assemblyKey3;
   Queue* assemblyQueue3;

   int shmTime;
   key_t shmemTimeKey;
   struct timeval* shmTimePointer;

   assemblySem = get_semkey("sem.Assemblykey"); //takes the id of the semaphores used for shared memory between
   assemblySemID = semget(assemblySem , 0 , 0); // the check department and the assembly department


   if (assemblySemID < 0){
      perror("assemblySem recovery error");
      exit(1);
   }

   assemblyKey1 = get_shmemkey("shmem.Assemblykey1"); //connects to a queue for every type of Merc
   assemblyQueue1 = connectQueue(assemblyKey1);

   assemblyKey2 = get_shmemkey("shmem.Assemblykey2");
   assemblyQueue2 = connectQueue(assemblyKey2);

   assemblyKey3 = get_shmemkey("shmem.Assemblykey3");
   assemblyQueue3 = connectQueue(assemblyKey3);

   shmemTimeKey = get_shmemkey("shmem.Timekey"); //shared memory segment that keeps the time
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

   struct timeval t2;

   for (int i = 0 ; i < numOfMercs ; i++){
      Merc finalMerc;
      Merc* mercp1;
      Merc* mercp2;
      Merc* mercp3;

      double firstMercCreation;
      double elapsedTime;


      for (unsigned short j = 0; j <= 2 ; j++){ //this for waits for every type of
         if(sem_down(assemblySemID , j) < 0){//merc to be available before taking it to create the final product
            perror("sem down error!");
         }
         if(sem_down(assemblySemID , 3 + j) < 0){ //queue semaphore
            exit(1);
         }
         switch (j) {
            case 0:
               mercp1 = popFromQ(assemblyQueue1);
               break;
            case 1:
               mercp2 = popFromQ(assemblyQueue2);
               break;
            case 2:
               mercp3 = popFromQ(assemblyQueue3);
               break;
         }
         if(sem_up(assemblySemID , 3 + j) < 0){
            exit(1);
         }
      }

      firstMercCreation = mercp1 ->timeStamp; //this part finds the Merc created the earliest
      if (mercp2 ->timeStamp < mercp3 ->timeStamp){
         if (mercp2 ->timeStamp < firstMercCreation){
            firstMercCreation = mercp2 ->timeStamp;
         }
      }
      else if (mercp3 ->timeStamp < firstMercCreation){
         firstMercCreation = mercp3 ->timeStamp;
      }

      gettimeofday(&t2 , NULL);

      elapsedTime = (t2.tv_sec - shmTimePointer->tv_sec) * 1000.0;      // sec to ms
      elapsedTime += (t2.tv_usec - shmTimePointer->tv_usec) / 1000.0;  //us to ms


      finalMerc.type = 123;
      sprintf(finalMerc.ID, "%d%d%d", mercp1->ID , mercp2->ID , mercp3->ID);
      finalMerc.timeStamp = elapsedTime;

      averageCompletionTime +=  elapsedTime - firstMercCreation;

      usleep((useconds_t)(10 * 1000));

   }
   averageCompletionTime = averageCompletionTime / (1.0*numOfMercs);

   printf("Average Completion Time per product(in ms) : %f\n",averageCompletionTime );
   exit(0);
}
