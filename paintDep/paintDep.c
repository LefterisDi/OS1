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


int main(int argc , char* argv[]){

   int numOfMercs;

   key_t sem;
   int sem_id;

   key_t shm;
   int shm_id;
   Merc* sharedMem;

   sem = get_semkey("sem.Paintkey");
   sem_id = semget(sem , 0 , 0);

   shm = get_shmemkey("shmem.Paintkey");
   shm_id = shmget(shm , 0 , 0);

   if((sharedMem = (Merc*)shmat(shm_id , 0 , 0)) < 0){
      perror("shmat error!");
      exit(1);
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

   for (int i = 0 ; i < 3*numOfMercs ; i++){


      if(sem_down(sem_id , 1) == 0){
        printf("Down 2ND: 2ND\n");
      }

      Merc merc1 = *sharedMem;

      printf("%d\n" , merc1.type);
      sleep(2);

      if(sem_up(sem_id , 0) == 0){
         printf("Up : %d\n\n" , sharedMem ->type);
      }


   }

 exit(0);
 return 0;
}
