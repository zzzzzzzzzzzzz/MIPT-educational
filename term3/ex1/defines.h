#include <stdlib.h>

#define MAX_FILENAME_SIZE 100
#define BUFSIZE 4096
#define SERV_FIFO_NAME "serv.fifo"

union pid_char {
  pid_t number;
  char bytes[sizeof(pid_t)];
};

union int_send {
  int number;
  char bytes[sizeof(int)];
};

struct inform {
  char command[MAX_FILENAME_SIZE];
  union pid_char pid;
};

union letter {
  struct inform def;
  char bytes[sizeof(struct inform)];
};

char* make_seed(pid_t inp) {
  char nums[10] = "0123456789";
  char *result = (char*)malloc(15);
  int j = 0;
  while (inp != 0) {
    int num;
    num = inp % 10;
    int i = 0;
    while (i != num) {
      i++;
    }
    result[j] = nums[i];
    inp /= 10;
    j++;
  }
  strcat(result, ".fifo");
  return result;
}
