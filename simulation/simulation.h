#ifndef __SIMULATION_H__
#define __SIMULATION_H__

int delete_sem_Dep(int);
int create_sem_Dep(char* , int , char* , int);
int delete_shm_Dep(int , Queue*);
int create_shm_Dep(char* , int , int , char* ,unsigned int , Queue**);

#endif
