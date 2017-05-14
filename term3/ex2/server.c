#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "defines.h"

int main() {
  char *array; //pointer to shared memory
  int shmid; //IPC descriptor to shared memory
  key_t key; //IPC key

  /*IPC key generation, 0 is the number of shared memory examplar*/
  if((key = ftok(KEYGENSTRING,0)) < 0){
    printf("Can\'t generate key\n");
    _exit(-1);
  }

  short int new; //1 means that shared memory created here
  /*Try to create shared memory for key. If shared memory exists for this key, then system call will return negative value. 0666 - read and write for all.*/
  if((shmid = shmget(key, SHMEMSIZE, 0666|IPC_CREAT|IPC_EXCL)) < 0){
    /*if unknown error then shutdown*/
    if(errno != EEXIST){
      printf("Can\'t create shared memory\n");
      _exit(-1);
    } else {
      /*if shared memory already exists then try to get IPC descriptor*/
      if((shmid = shmget(key, SHMEMSIZE, 0)) < 0){
        printf("Can\'t find shared memory\n");
        _exit(-1);
      }
      new = 1;
    }
  }

  /*try to display shared memory to the address field of this process*/
  if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
    printf("Can't attach shared memory\n");
    _exit(-1);
  }
  /*Initialization of memory*/
  if (new) {
    int i;
    for (i = 0; i < SHMEMSIZE; i++) {
      array[i] = 0;
    }
  }

  /*getting semaphores*/
  int semid; // descriptor for ipc semaphores
  struct sembuf semComandante; //structure for semaphore operations
  if((semid = semget(key, 3, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t get semid\n");
    _exit(-1);
  }

  semComandante.sem_op = 1; //server ready and free
  semComandante.sem_flg = 0;
  semComandante.sem_num = 2;
  if(semop(semid, &semComandante, 1) < 0){
    printf("Can\'t wait for condition\n");
    _exit(-1);
  }

  /*MAIN PART*/
  /*CRITICAL ZONE BEGINS*/
  while (1) {
/*
 *    char command[COMMAND_MAX_SIZE];
 *    if(fgets(command, COMMAND_MAX_SIZE, stdin) != NULL) {
 *      if (strcmp(command, SHUTDOWN) == 0) {
 *
 *        [>detach shared memory<]
 *        if(shmdt(array) < 0){
 *          printf("Can't detach shared memory\n");
 *          _exit(-1);
 *        }
 *
 *        [>delete shared memory and semaphores from system<]
 *        shmctl(shmid, IPC_RMID, NULL);
 *        semctl(semid, 0, IPC_RMID, 0);
 *      }
 *    }
 */
    semComandante.sem_op = -1;
    semComandante.sem_flg = 0;
    semComandante.sem_num = 0;
    if(semop(semid, &semComandante, 1) < 0){
      printf("Can\'t wait for condition\n");
      _exit(-1);
    }
    int i;
    for (i = 0; ((i < SHMEMSIZE) && (array[i] != 0)); i++) {
      putchar(array[i]);
      array[i] = 0;
    }
    semComandante.sem_op = 1;
    semComandante.sem_flg = 0;
    semComandante.sem_num = 1;
    if(semop(semid, &semComandante, 1) < 0){
      printf("Can\'t wait for condition\n");
      _exit(-1);
    }
  }
  /*CRITICAL ZONE ENDS*/
  /*END OF MAIN PART*/
  return 0;
}
