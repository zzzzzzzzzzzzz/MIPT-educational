#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <regex.h>//posix regexp library
#include <string.h>
#include "defines.h"

int main(int argc, char **argv) {
  /*check arguments*/
  if (argc < 2) {
    printf("Waited for the name of file\n");
    _exit(-1);
  } else {
    if (argc > 2) {
      printf("Note that program needs only 1 argument, other will be ingored\n");
    }
    regex_t regex;
    int reti;
    /*Compile regular expression*/
    reti = regcomp(&regex, FILENAMEREGEX, 0);
    if (reti) {
        printf("Could not compile regex\n");
        _exit(-1);
    }
    /* Execute regular expression */
    reti = regexec(&regex, argv[1], 0, NULL, 0);

    if (reti != 0) {
      if (reti == REG_NOMATCH) {
          printf("Incorrect name of file\n");
          _exit(-1);
      }
      else {
          printf("Regex match failed!\n");
          _exit(-1);
      }
    }

    /* Free compiled regular expression*/
    regfree(&regex);
  }

  char *array; //pointer to shared memory
  int shmid; //IPC descriptor to shared memory
  key_t key; //IPC key

  /*IPC key generation, 0 is the number of shared memory examplar*/
  if((key = ftok(KEYGENSTRING,0)) < 0){
    printf("Can\'t generate key\n");
    _exit(-1);
  }

  /*getting semaphores*/
  int semid; // descriptor for ipc semaphores
  struct sembuf semComandante; //structure for semaphore operations
  if((semid = semget(key, 3, 0)) < 0){
    printf("Can\'t get semid, You should run server first\n");
    _exit(-1);
  }

  semComandante.sem_op = -1; //server is busy
  semComandante.sem_flg = 0;
  semComandante.sem_num = 2;
  if(semop(semid, &semComandante, 1) < 0){
    printf("Can\'t wait for condition\n");
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

  /*MAIN PART*/

  semComandante.sem_op = 1;
  semComandante.sem_flg = 0;
  semComandante.sem_num = 1;
  if(semop(semid, &semComandante, 1) < 0){
    printf("Can\'t wait for condition\n");
    _exit(-1);
  }

  int fd; //file descriptor
  if ((fd = open(argv[1], O_RDONLY)) < 0) {
    printf("Can\'t open file!\n");
    _exit(-1);
  } else {
    /*CRITICAL ZONE BEGINS*/
    size_t size;
    int count = 0;
    while (1) {
      semComandante.sem_op = -1;
      semComandante.sem_flg = 0;
      semComandante.sem_num = 1;
      if(semop(semid, &semComandante, 1) < 0){
        printf("Can\'t wait for condition zaza\n");
        _exit(-1);
      }
      if ((size = read(fd, array, SHMEMSIZE)) < 0) {
        printf("Can'\t read information from file\n");
        _exit(-1);
      } else if (size == 0) {
        break;
      }
      semComandante.sem_op = 1;
      semComandante.sem_flg = 0;
      semComandante.sem_num = 0;
      if(semop(semid, &semComandante, 1) < 0){
        printf("Can\'t wait for condition\n");
        _exit(-1);
      }
    }
    /*CRITICAL ZONE ENDS*/
    close(fd);
  }
  /*END OF MAIN PART*/

  semComandante.sem_op = 1; //server is free
  semComandante.sem_flg = 0;
  semComandante.sem_num = 2;
  if(semop(semid, &semComandante, 1) < 0){
    printf("Can\'t wait for condition\n");
    _exit(-1);
  }

  /*detach shared memory*/
  if(shmdt(array) < 0){
    printf("Can't detach shared memory\n");
    _exit(-1);
  }
  return 0;
}
