#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>


#include "constructionDep.h"
#include "../semaphores/semCtrl.h"
#include "../sharedMemory/shmCtrl.h"

int main(int argc , char* argv[]){

  int typeOfMerc = 0;
  int numOfMercs = 0;

  key_t sem;
  int sem_id;

  key_t shm;
  int shm_id;
  Merc* sharedMem;

  sem = get_semkey("sem.Paintkey");
  sem_id = semget(sem , 0 , 0);

  shm = get_shmemkey("shmem.Paintkey");
  shm_id = shmget(shm , sizeof(Merc) , 0666);


  if((sharedMem = (Merc*)shmat(shm_id , 0 , 0)) < 0){
     perror("shmat error!");
     exit(1);
  }

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



  for(int i = 0 ; i < numOfMercs ; i++){

     Merc merc1;
     merc1.type = typeOfMerc;


     if(sem_down(sem_id , 0) == 0){
        printf("Down : %d\n" , typeOfMerc);
     }

     *sharedMem = merc1;

     if(sem_up(sem_id , 1) == 0){
       printf("Up 2ND: %d\n" , typeOfMerc);
     }

  }





  exit(0);
  return 0;
}
