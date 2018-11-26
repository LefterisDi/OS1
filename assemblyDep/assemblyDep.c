#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../constructionDep/constructionDep.h"
#include "../sharedMemory/shmCtrl.h"
#include "../semaphores/semCtrl.h"
#include "../queue/queue.h"

int main(int argc , char* argv[]){

   printf("HEYYOOOOO\n");

   // int numOfMercs;
   //
   // key_t sem;
   // int sem_id;
   //
   // key_t checkSem;
   // int checkSemID;
   //
   // key_t checkKey1;
   // Queue* checkQueue1;
   //
   // key_t checkKey2;
   // Queue* checkQueue2;
   //
   // key_t checkKey3;
   // Queue* checkQueue3;
   //
   // sem = get_semkey("sem.Paintkey");
   //
   // sem_id = semget(sem , 0 , 0);
   //
   // if (sem_id < 0){
   //    perror("checkSem recovery error");
   //    exit(1);
   // }
   //
   // shm = get_shmemkey("shmem.Paintkey");
   // sharedMem = connectQueue(shm);
   //
   // checkSem = get_semkey("sem.Checkkey");
   // checkSemID = semget(checkSem , 0 , 0);
   //
   // if (checkSemID < 0){
   //    perror("checkSem recovery error");
   //    exit(1);
   // }
   //
   // checkKey1 = get_shmemkey("shmem.Checkkey1");
   // checkQueue1 = connectQueue(checkKey1);
   //
   // checkKey2 = get_shmemkey("shmem.Checkkey2");
   // checkQueue2 = connectQueue(checkKey2);
   //
   // checkKey3 = get_shmemkey("shmem.Checkkey3");
   // checkQueue3 = connectQueue(checkKey3);
   //
   //
   // while(--argc > 0){
   //
   //   if (strstr(*argv , "-N") != NULL){
   //    argv++;
   //    argc--;
   //    numOfMercs = atoi(*argv);
   //   }
   //
   //   if (argc > 0){
   //    argv++;
   //   }
   // }
   exit(0);
}
