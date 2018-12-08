#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
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

  int shmTime;
  key_t shmemTimeKey;
  struct timeval* shmTimePointer;


  srand(time(NULL));

  sem = get_semkey("sem.Paintkey");
  sem_id = semget(sem , 0 , 0);

  shm = get_shmemkey("shmem.Paintkey");
  sharedMem = connectQueue(shm);

  shmemTimeKey = get_shmemkey("shmem.Timekey");
  shmTime = shmget(shmemTimeKey , 0 , 0);
  if((shmTimePointer = (struct timeval*)shmat(shmTime , 0 , 0)) < 0){
     perror("shmat error!");
     return(0);
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

  double elapsedTime;
  struct timeval t2;

  for(int i = 0 ; i < numOfMercs ; i++){

     double sleepTime = ((((double)rand() / (double)RAND_MAX) + (double)typeOfMerc) / (1.0 + (double)typeOfMerc)) * 0.01; //finds a random sleep time


     usleep((useconds_t)(sleepTime * 1000));

     Merc* merc = malloc(sizeof(Merc));
     merc ->type = typeOfMerc;
     sprintf(merc->ID , "%d", 1000 + i + numOfMercs*merc->type);

     gettimeofday(&t2 , NULL);

     elapsedTime = (t2.tv_sec - shmTimePointer->tv_sec) * 1000.0;      // sec to ms
     elapsedTime += (t2.tv_usec - shmTimePointer->tv_usec) / 1000.0;  //us to ms

     merc->timeStamp = elapsedTime;


     if(sem_down(sem_id , 0) < 0){ // brings its queue semaphore down in order to access the queue
        exit(1);
     }

     if (insertToQ(sharedMem , *merc) < 0){
        perror("Queue Full");
        exit(1);
     }


     if(sem_up(sem_id , 0) < 0){//brings queue semaphore up
        exit(1);
     }

     if(sem_up(sem_id , 1) < 0){//brings paint department's semaphore up to
     //signal that an item is ready and has entered the queue
     exit(1);
     }

     free(merc);
  }



  exit(0);
  return 0;
}
