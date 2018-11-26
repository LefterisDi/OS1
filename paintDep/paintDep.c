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


int main(int argc , char* argv[]){

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

   for (int i = 0 ; i < 3*numOfMercs ; i++){


      if(sem_down(sem_id , 1) < 0){
         exit(1);
      }

      Merc* mercp = popFromQ(sharedMem);

      printf("%d\n" , mercp->type);

      switch (mercp ->type) {
         case 1:
            insertToQ(checkQueue1 , *mercp);
            if(sem_up(checkSemID , 0) < 0){
               exit(1);
            }
            break;

         case 2:
            insertToQ(checkQueue2 , *mercp);
            if(sem_up(checkSemID , 1) < 0){
               exit(1);
            }
            break;

         case 3:
            insertToQ(checkQueue3 , *mercp);
            if(sem_up(checkSemID , 2) < 0){
               exit(1);
            }
            break;
      }

   }

 exit(0);
 return 0;
}
