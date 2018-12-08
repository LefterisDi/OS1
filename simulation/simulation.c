#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
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

  int shmTime;
  key_t shmemTimeKey;
  struct timeval* shmTimePointer;

  int shmPaint;
  int semPaint;
  Queue* qPaint;

  int semCheck;
  int shmCheck1;
  int shmCheck2;
  int shmCheck3;
  Queue* qCheck1;
  Queue* qCheck2;
  Queue* qCheck3;


  int semAssembly;
  int shmAssembly1;
  int shmAssembly2;
  int shmAssembly3;
  Queue* qAssembly1;
  Queue* qAssembly2;
  Queue* qAssembly3;



  while(--argc > 0){//gets the number of Mercs per construction department
    if (strstr(*argv , "-N") != NULL){
      argv++;
      argc--;
      numOfItems = atoi(*argv);
    }

    if (argc > 0){
      argv++;
    }
  }

  if ((shmemTimeKey = create_store_shmemkey("./simulation/simulation.c" , 'M' , "shmem.Timekey")) < 0){//creates a key in order to store the starting time
     exit(1);
  }

  if ((shmTime = shmget(shmemTimeKey,sizeof(struct timeval) , IPC_CREAT | 0666)) < 0) { //gets the shared memory for that
     perror("shmget!\n");
     return(0);
  }

  if((shmTimePointer = (struct timeval*)shmat(shmTime , 0 , 0)) < 0){//and lastly gets a pointer to it
     perror("shmat error!");
     return(0);
  }

  gettimeofday(shmTimePointer , NULL);


  if ((shmPaint = create_shm_Dep("./paintDep/paintDep.c" , 'M' , 'A' , "shmem.Paintkey" , 3*numOfItems , &qPaint)) < 0){//creates a shared memory segment for paint dep
     exit(1);
  }

  if ((semPaint = create_sem_Dep("./paintDep/paintDep.c" , 'S' , "sem.Paintkey" , 2)) < 0){//creates the semaphores for it
     exit(1);
  }

  if(sem_down(semPaint , 1) < 0){//it brings the semaphore (counter) of paint department's number of items in queue to 0
     perror("sem down error!");
  }


  if ((shmCheck1 = create_shm_Dep("./checkDep/checkDep.c" , 'M' , 'A' , "shmem.Checkkey1" , numOfItems , &qCheck1)) < 0){//creates a shared memory segment for check dep 1
     exit(1);
  }

  if ((shmCheck2 = create_shm_Dep("./checkDep/checkDep.c" , 'L' , 'B' , "shmem.Checkkey2" , numOfItems , &qCheck2)) < 0){//creates a shared memory segment for check dep 2
     exit(1);
  }

  if ((shmCheck3 = create_shm_Dep("./checkDep/checkDep.c" , 'K' , 'C' , "shmem.Checkkey3" , numOfItems , &qCheck3)) < 0){//creates a shared memory segment for check dep 3
     exit(1);
  }

  if ((semCheck = create_sem_Dep("./checkDep/checkDep.c" , 'S' , "sem.Checkkey" , 6)) < 0){//creates the semaphores for those departments
     exit(1);
  }


  for (unsigned short i = 0; i <= 2 ; i++){//it brings the semaphores (counters) of check departments' number of items in the 3 queues to 0
     if(sem_down(semCheck , i) < 0){
        perror("sem down error!");
     }
  }



  if ((shmAssembly1 = create_shm_Dep("./assemblyDep/assemblyDep.c" , 'M' , 'A' , "shmem.Assemblykey1" , numOfItems , &qAssembly1)) < 0){
     //creates a shared memory segment for assembly dep , in order to be used between check dep 1 and assembly dep
     exit(1);
  }

  if ((shmAssembly2 = create_shm_Dep("./assemblyDep/assemblyDep.c" , 'L' , 'B' ,  "shmem.Assemblykey2" , numOfItems , &qAssembly2)) < 0){
     //creates a shared memory segment for assembly dep , in order to be used between check dep 2 and assembly dep
     exit(1);
  }

  if ((shmAssembly3 = create_shm_Dep("./assemblyDep/assemblyDep.c" , 'K' , 'C' ,  "shmem.Assemblykey3" , numOfItems , &qAssembly3)) < 0){
     //creates a shared memory segment for assembly dep , in order to be used between check dep 3 and assembly dep
     exit(1);
  }

  if ((semAssembly = create_sem_Dep("./assemblyDep/assemblyDep.c" , 'S' , "sem.Assemblykey" , 6)) < 0){
     exit(1);
  }//creates the semaphores


  for (unsigned short i = 0; i <= 2 ; i++){//it brings the semaphores (counters) of check departments' number of items in the 3 queues(between assembly and check) to 0
     if(sem_down(semAssembly , i) < 0){
        perror("sem down error!");
     }
  }



  for (int i = 1 ; i <= 3 ; i++){//passing parameters and executing construction deps
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

  if (fork() == 0){//passing parameters and executing paint dep
    if(execl("./paintDp" ,"./paintDp" , "-N" , numOfMercs , (char *)0) < 0){
      perror("execl error!\n");
      exit(1);
    }
    exit(0);
  }

  for (int i = 1 ; i <= 3 ; i++){//passing parameters and executing check deps
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

  if (fork() == 0){//passing parameters and executing assembly dep
    if(execl("./assemblyDp" ,"./assemblyDp" , "-N" , numOfMercs , (char *)0) < 0){
      perror("execl error!\n");
      exit(1);
    }
    exit(0);
  }

  while ((wpid = wait(&status)) > 0);//waits for all children to finish

  if (shm_delete(shmTime) < 0){ //deletes the shared memory for time
     perror("Error while trying to delete the shared memory");
     return(-1);
  }

  if (delete_shm_Dep(shmPaint , qPaint) < 0){//from here on deletes every shared memory and semaphore created
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


  if (delete_shm_Dep(shmAssembly1 , qAssembly1) < 0){
     exit(1);
  }

  if (delete_shm_Dep(shmAssembly2 , qAssembly2) < 0){
     exit(1);
  }

  if (delete_shm_Dep(shmAssembly3 , qAssembly3) < 0){
     exit(1);
  }

  if (delete_sem_Dep(semAssembly) < 0){
    exit(1);
  }

  if (remove("./shmem.Paintkey") != 0){//from here on deletes all files created for storing keys
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove("./sem.Paintkey") != 0){
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove("./shmem.Checkkey1") != 0){
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove("./shmem.Checkkey2") != 0){
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove("./shmem.Checkkey3") != 0){
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove( "./sem.Checkkey") != 0){
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove( "./shmem.Assemblykey1") != 0){
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove( "./shmem.Assemblykey2") != 0){
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove( "./shmem.Assemblykey3") != 0){
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove("./sem.Assemblykey") != 0){
     perror("Couldn't delete file\n");
     exit(1);
  }
  if (remove("./shmem.Timekey") != 0){
     perror("Couldn't delete file\n");
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

   *q = createQueue(numOfElems , shmemkey , arrayK , &shmid);//creates a queue in shared memory

   return shmid;
}

int delete_shm_Dep(int semid , Queue* q){//deletes the shared memory segment

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

   for (unsigned short i = 0 ; i < semnum ; i++){//initializing the semaphores
      if (set_semval(semid , i) < 0){
         perror("Error while initializing the semaphore");
         return(-3);
      }
   }

   return semid;
}

int delete_sem_Dep(int semid){//deletes semaphores
   if (sem_delete(semid , 0) < 0){
      perror("Error while trying to delete the semaphore");
      return(-1);
   }

   return 0;
}
