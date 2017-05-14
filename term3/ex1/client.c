#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <string.h>
#include "defines.h"
#include <errno.h>

int main(int argc, char **argv) {
  //Initialize array
  key_t key; //ipc key
  if((key = ftok(SERV_FIFO_NAME, 0)) < 0){
    printf("Can\'t generate key\n");
    _exit(errno);
  }

  size_t size;
  (void)umask(0);
  if (argv[1] == NULL) {
    printf("Haven't got filename, exiting...\n");
    _exit(errno);
  }
  if (argv[2] != NULL) {
    printf("Note, that all filenames except first will be ignored\n");
  }
  size_t str_size;
  str_size = strlen(argv[1]);
  if (str_size > MAX_FILENAME_SIZE) {
    printf("filename size is more than max %d, exiting...\n", MAX_FILENAME_SIZE);
    _exit(errno);
  }
  int fd;
  union letter to_send;
  if ((fd = open(SERV_FIFO_NAME, O_WRONLY)) < 0) {
    printf("Can\'t open FIFO for writing\n");
    _exit(errno);
  } else {
    strcpy(to_send.def.command, argv[1]);
    to_send.def.pid.number = getpid();
    char* answer_fifo_name = make_seed(to_send.def.pid.number);
    /*Creating a channel to get the answer*/
    if(mknod(answer_fifo_name, S_IFIFO | 0666, 0) < 0) {
      printf("Can\'t create FIFO to answer\n");
      free(answer_fifo_name);
      _exit(errno);
    }
    size = write(fd, to_send.bytes, sizeof(struct inform));
    if (size  < 0) {
      printf("Can\'t write to FIFO\n");
      free(answer_fifo_name);
      _exit(errno);
    }
    close(fd);
    /*Reading answer*/
    union int_send sum_size;
    sum_size.number = 0;
    int answer_fifo_descriptor;
    if((answer_fifo_descriptor = open(answer_fifo_name, O_RDONLY)) < 0){
      printf("Can\'t open FIFO for reading\n");
      free(answer_fifo_name);
      _exit(errno);
    } else {
      char buffer[BUFSIZE];
      while ((size = read(answer_fifo_descriptor, buffer, BUFSIZE))) {
        if (size < 0) {
          printf("Can'\t read from answer fifo\n");
          free(answer_fifo_name);
          _exit(errno);
        }
        sum_size.number += size;
        size = write(1, buffer, size);//write to stdout
        if (size < 0) {
          printf("Can'\t write to stdout\n");
          free(answer_fifo_name);
          _exit(errno);
        }
      }
      close(answer_fifo_descriptor);
    }
    if((answer_fifo_descriptor = open(answer_fifo_name, O_WRONLY)) < 0){
      printf("Can\'t open FIFO for reading\n");
      free(answer_fifo_name);
      _exit(errno);
    } else {
      size = write(answer_fifo_descriptor, sum_size.bytes, sizeof(union int_send));
      if (size < 0) {
        printf("Can\'t send size inform");
        _exit(errno);
      }
      close(answer_fifo_descriptor);
    }
    unlink(answer_fifo_name);
    free(answer_fifo_name);
  }
  return 0;
}
