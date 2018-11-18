#ifndef __SEM_CTRL_H__
#define __SEM_CTRL_H__

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int sem_up(int , unsigned short);
int sem_down(int , unsigned short);
int sem_delete(int , int);
key_t create_store_semkey(const char*, int , const char*);
key_t get_semkey(const char*);
int set_semval(int , unsigned short);
int shm_delete(int);

#endif
