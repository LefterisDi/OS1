#ifndef __SHM_CTRL_H__
#define __SHM_CTRL_H__

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

key_t create_store_shmemkey(const char*, int , const char*);
key_t get_shmemkey(const char*);

#endif
