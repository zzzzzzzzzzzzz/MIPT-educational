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
  char buffer[BUFSIZE]; //buffer for information exchange
  int i;
  for (i = 0; i < BUFSIZE; i++) {
    buffer[i] = 0;
  }
  /*Clear mask of files of this process*/
  (void)umask(0);
  if(mknod(SERV_FIFO_NAME, S_IFIFO | 0666, 0) < 0){
    printf("Can\'t create FIFO\n");
    _exit(errno);
  }

  int fd; // file descriptor for serv.fifo
  if((fd = open(SERV_FIFO_NAME, O_RDONLY)) < 0){
    /*If fails*/
    printf("Can\'t open FIFO for reading\n");
    _exit(errno);
  } else {
    printf("Listening...\n");
    size_t size;
    union letter received;
    while (1) {
      int j = 0;
      size = read(fd, received.bytes, sizeof(union letter));
      if (size < 0) {
        printf("Can\'t read from FIFO");
        _exit(errno);
      }
      if (size > 0) {
        /*Sending answer*/
        printf("Sending answer\n");
        printf("Server will send file called %s\n", received.def.command);
        char* answer_fifo_name = make_seed(received.def.pid.number);
        int answer_fifo_descriptor;
        printf("answer fifo %s\n", answer_fifo_name);
        if ((answer_fifo_descriptor = open(answer_fifo_name, O_WRONLY)) < 0) {
          printf("Can\'t open FIFO for writing\n");
          free(answer_fifo_name);
          _exit(errno);
        } else {
          int input_descriptor;
          if ((input_descriptor = open(received.def.command, O_RDONLY)) < 0) {
            printf("Can\'t open file for sending\n");
            free(answer_fifo_name);
            _exit(errno);
          } else {
            while ((size = read(input_descriptor, buffer, BUFSIZE))) {
              if(size < 0) {
                printf("Can\'t read from file\n");
                free(answer_fifo_name);
                _exit(errno);
              }
              size = write(answer_fifo_descriptor, buffer, size);
              if(size < 0) {
                printf("Can\'t write to FIFO\n");
                free(answer_fifo_name);
                _exit(errno);
              }
            }
          }
          close(answer_fifo_descriptor);
          if ((answer_fifo_descriptor = open(answer_fifo_name, O_RDONLY)) < 0) {
            printf("Can\'t open FIFO for writing\n");
            free(answer_fifo_name);
            _exit(errno);
          } else {
            union int_send sum_size;
            size = read(answer_fifo_descriptor, sum_size.bytes, sizeof(union int_send));
            printf("client received %d bytes\n", sum_size.number);
            close(answer_fifo_descriptor);
          }
        }
      }
    }
  }
  return 0;
}
