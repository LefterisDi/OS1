#include "semCtrl.h"
#include "semun.h"

key_t create_store_semkey(const char* pathname, int proj_id , const char* keyfile){

   key_t semkey;
   int sem_fd;

   if((semkey = ftok(pathname , proj_id)) < 0){ //creates a semaphore key
     perror("Couldn't create a key with ftok!");
     return(-1);
   }

   sem_fd = open(keyfile , O_WRONLY | O_TRUNC | O_EXCL | O_CREAT , 0644); // it opens a file to save this key

   if (sem_fd < 0){
     perror("Could not open file");
     return(-1);
   }

   if (write(sem_fd , &semkey , sizeof(key_t)) < 0){//stores the key to a file
     perror("Could not write key to file");
     return(-2);
   }

   close(sem_fd);

   return semkey;
}

key_t get_semkey(const char* keyfile){//gets the key from the file

   int sem_fd;
   key_t semkey;

   sem_fd = open(keyfile , O_RDONLY);

   if (sem_fd < 0){
     perror("Could not open file");
     return(-1);
   }

   if (read(sem_fd , &semkey , sizeof(key_t)) != sizeof(key_t)){
     perror("Could not read key from file");
     return(-2);
   }

   return semkey;
}

int set_semval(int semid , unsigned short sem_num){//it basically initializes the semaphore value to 1(up)
   union semun sem_union;
   sem_union.val = 1;
   if (semctl(semid , sem_num , SETVAL , sem_union) < 0){
      return 1;
   }
   return 0;
}

int sem_up(int semid , unsigned short sem_num){//'ups' the semaphore
   struct sembuf semaphoreBuf;
   semaphoreBuf.sem_num = sem_num;
   semaphoreBuf.sem_op = 1;
   semaphoreBuf.sem_flg = 0;
   if (semop(semid , &semaphoreBuf , 1) < 0){
      printf("%d\n", semid);
      perror("semaphore up error!");
      return -1;
   }
   return 0;
}

int sem_down(int semid , unsigned short sem_num){//brings down the semaphore
   struct sembuf semaphoreBuf;
   semaphoreBuf.sem_num = sem_num;
   semaphoreBuf.sem_op = -1;
   semaphoreBuf.sem_flg = 0;
   if (semop(semid , &semaphoreBuf , 1) < 0){
      perror("semaphore down error!");
      return -1;
   }
   return 0;
}

int sem_delete(int semid , int sem_num){//deletes the semaphore
   if(semctl(semid , sem_num , IPC_RMID , 0) < 0){
      perror("semaphore delete error!");
      return -1;
   }
   return 0;
}
