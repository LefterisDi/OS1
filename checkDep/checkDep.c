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

   int typeOfMerc = 0;
   int numOfMercs = 0;

   key_t sem;
   int sem_id;

   key_t shm;
   int shm_id;
   Queue* sharedMem;

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


   switch (typeOfMerc) {
      case 1:
         shm = get_shmemkey("shmem.Checkkey1");
         sharedMem = connectQueue(shm);
         break;

      case 2:
         shm = get_shmemkey("shmem.Checkkey2");
         sharedMem = connectQueue(shm);
         break;

      case 3:
         shm = get_shmemkey("shmem.Checkkey3");
         sharedMem = connectQueue(shm);
         break;
   }

   for(int i = 0 ; i < numOfMercs ; i++){

      if(sem_down(sem_id , typeOfMerc-1) != 0){
         exit(1);
      }

      Merc* mercp = popFromQ(sharedMem);

      printf("CHECK %d\n" , mercp->type);
   }

}
