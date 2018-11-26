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
#include "simulation.h"

int main(int argc , char* argv[]){

  pid_t wpid;
  int status = 0;
  char typeOfMerc[10];
  char numOfMercs[10];
  int numOfItems = 0;

  int shmPaint;
  int semPaint;
  Queue* qPaint;

  int shmCheck1;
  int shmCheck2;
  int shmCheck3;
  int semCheck;
  Queue* qCheck1;
  Queue* qCheck2;
  Queue* qCheck3;




  while(--argc > 0){
    if (strstr(*argv , "-N") != NULL){
      argv++;
      argc--;
      numOfItems = atoi(*argv);
    }

    if (argc > 0){
      argv++;
    }
  }

  if ((shmPaint = create_shm_Dep("./paintDep/paintDep.c" , 'M' , 'A' , "shmem.Paintkey" , 3*numOfItems , &qPaint)) < 0){
     exit(1);
  }

  if ((semPaint = create_sem_Dep("./paintDep/paintDep.c" , 'S' , "sem.Paintkey" , 2)) < 0){
     exit(1);
  }

  if(sem_down(semPaint , 1) < 0){
     perror("sem down error!");
  }

  if ((shmCheck1 = create_shm_Dep("./checkDep/checkDep.c" , 'M' , 'A' , "shmem.Checkkey1" , numOfItems , &qCheck1)) < 0){
     exit(1);
  }

  if ((shmCheck2 = create_shm_Dep("./checkDep/checkDep.c" , 'L' , 'B' , "shmem.Checkkey2" , numOfItems , &qCheck2)) < 0){
     exit(1);
  }

  if ((shmCheck3 = create_shm_Dep("./checkDep/checkDep.c" , 'K' , 'C' , "shmem.Checkkey3" , numOfItems , &qCheck3)) < 0){
     exit(1);
  }

  if ((semCheck = create_sem_Dep("./checkDep/checkDep.c" , 'S' , "sem.Checkkey" , 3)) < 0){
     exit(1);
  }


  for (unsigned short i = 0; i <= 2 ; i++){
     if(sem_down(semCheck , i) < 0){
        perror("sem down error!");
     }
  }


  for (int i = 1 ; i <= 3 ; i++){
    sprintf(typeOfMerc, "%d", i);
    sprintf(numOfMercs, "%d", numOfItems);
    if (fork() == 0){
      if (execl("./constructionDp" , "./constructionDp" ,  "-T" , typeOfMerc , "-N", numOfMercs , (char *)0) < 0){
        perror("execl error!\n");
        exit(1);
      }
      exit(0);
    }
  }

  if (fork() == 0){
    if(execl("./paintDp" ,"./paintDp" , "-N" , numOfMercs , (char *)0) < 0){
      perror("execl error!\n");
      exit(1);
    }
    exit(0);
  }

  for (int i = 1 ; i <= 3 ; i++){
    sprintf(typeOfMerc, "%d", i);
    sprintf(numOfMercs, "%d", numOfItems);
    if (fork() == 0){
      if (execl("./checkDp" , "./checkDp" ,  "-T" , typeOfMerc , "-N" , numOfMercs , (char *)0) < 0){
        perror("execl error!\n");
        exit(1);
      }
      exit(0);
    }
  }

  while ((wpid = wait(&status)) > 0);


  if (delete_shm_Dep(shmPaint , qPaint) < 0){
     exit(1);
  }

  if (delete_sem_Dep(semPaint) < 0){
    exit(1);
  }

  if (delete_shm_Dep(shmCheck1 , qCheck1) < 0){
     exit(1);
  }

  if (delete_shm_Dep(shmCheck2 , qCheck2) < 0){
     exit(1);
  }


  if (delete_shm_Dep(shmCheck3 , qCheck3) < 0){
     exit(1);
  }

  if (delete_sem_Dep(semCheck) < 0){
    exit(1);
  }
  exit(0);
}


int create_shm_Dep(char* filename , int proj_id , int proj_id_array , char* keyfname ,unsigned int numOfElems , Queue** q){
   /*shared memory variables*/
   key_t shmemkey;
   key_t arrayK;
   int shmid;

   /*creates and stores the shared memory key to a file , so that the children can get access to it*/

   if ((shmemkey = create_store_shmemkey(filename , proj_id , keyfname)) < 0){
      return(-1);
   }

   if((arrayK = ftok(filename , proj_id_array)) < 0){
     perror("Couldn't create a key with ftok!");
     return(-1);
   }

   *q = createQueue(numOfElems , shmemkey , arrayK , &shmid);

   return shmid;
}

int delete_shm_Dep(int semid , Queue* q){

   if(deleteArray(q) < 0){
      perror("Error while trying to delete the shared memory");
      return -1;
   }


   if (shm_delete(semid) < 0){
      perror("Error while trying to delete the shared memory");
      return(-1);
   }
   return 0;
}

int create_sem_Dep(char* filename , int proj_id , char* keyfname , int semnum){

   /*semaphore variables*/
   key_t semkey;
   int semid;
   int semflgs = IPC_EXCL | IPC_CREAT | 0666;

   /*creates and stores the semaphore key to a file , so that the children can get access to it*/

   if ((semkey = create_store_semkey(filename , proj_id , keyfname)) < 0){
      return(-1);
   }

   if ((semid = semget(semkey , semnum , semflgs)) < 0){
      perror("semget error!\n");
      return(-2);
   }

   for (unsigned short i = 0 ; i < semnum ; i++){
      if (set_semval(semid , i) < 0){
         perror("Error while initializing the semaphore");
         return(-3);
      }
   }

   return semid;
}

int delete_sem_Dep(int semid){
   if (sem_delete(semid , 0) < 0){
      perror("Error while trying to delete the semaphore");
      return(-1);
   }

   return 0;
}
