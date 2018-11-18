#ifndef __SEMUN_H__
#define __SEMUN_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


union semun{
   int val;
   struct semid_ds *buf;
   unsigned short *array;
};

#endif
