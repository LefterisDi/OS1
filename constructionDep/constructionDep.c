#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>


#include "constructionDep.h"
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

  srand(time(NULL));

  sem = get_semkey("sem.Paintkey");
  sem_id = semget(sem , 0 , 0);

  shm = get_shmemkey("shmem.Paintkey");
  sharedMem = connectQueue(shm);


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

     double sleepTime = ((((double)rand() / (double)RAND_MAX) + (double)typeOfMerc) / (1.0 + (double)typeOfMerc)) * 0.4;

     sleep(sleepTime);

     Merc merc;
     merc.type = typeOfMerc;
     sprintf(merc.ID , "%d%d", typeOfMerc , i);


     if(sem_down(sem_id , 0) == 0){
        printf("Down : %d\n" , typeOfMerc);
     }

     insertToQ(sharedMem , merc);

     if(sem_up(sem_id , 1) < 0){
        exit(1);
     }

     if(sem_up(sem_id , 0) == 0){
       printf("Up : %d\n" , typeOfMerc);
     }


  }

  exit(0);
  return 0;
}
