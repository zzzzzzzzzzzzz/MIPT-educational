#include "queue.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/*stat() returns the structure below*/
/*
 *struct stat {
 *  dev_t st_dev; [> устройство, на котором расположен файл <]
 *  ino_t st_ino; [> номер индексного узла для файла <]
 *  mode_t st_mode; [> тип файла и права доступа к нему <]
 *  nlink_t st_nlink; [> счетчик числа жестких связей <]
 *  uid_t st_uid; [> идентификатор пользователя владельца <]
 *  gid_t st_gid; [> идентификатор группы владельца <]
 *  dev_t st_rdev; [> тип устройства для специальных файлов устройств <]
 *  off_t st_size; [> размер файла в байтах (если определен для данного типа файлов) <]
 *  unsigned long st_blksize; [> размер блока для файловой системы <]
 *  unsigned long st_blocks; [> число выделенных блоков <]
 *  time_t st_atime; [> время последнего доступа к файлу <]
 *  time_t st_mtime; [> время последней модификации файла <]
 *  time_t st_ctime; [> время создания файла <]
 *};
 */

#define IF_NOT_DIR(PATH) \
  if (S_ISLNK(info.st_mode)) {\
    standartFormat(&info, "l");\
    char *real = NULL;\
    printf("%s", PATH);\
    printf("->%s", realpath(PATH, real));\
    free(real);\
  } \
  if (S_ISREG(info.st_mode)) { \
    standartFormat(&info, "-"); \
    printf("%s", PATH);\
  } \
  if (S_ISFIFO(info.st_mode)) { \
    standartFormat(&info, "p");\
    printf("%s", PATH);\
  } \
  printf("\n");


char *getMonth(int number) {
  char **months = (char**)malloc(12*8);
  int i;
  for (i = 0; i < 12; i++) {
    months[i] = (char*)malloc(4);
  }
  months[0] = "Янв.";
  months[1] = "Фев.";
  months[2] = "Март";
  months[3] = "Апр.";
  months[4] = "Май ";
  months[5] = "Июнь";
  months[6] = "Июль";
  months[7] = "Авг.";
  months[8] = "Сен.";
  months[9] = "Окт.";
  months[10] = "Ноя.";
  months[11] = "Дек.";
  return months[number];
}

void ShowRights(mode_t mode) {
  if (mode & S_IRUSR) {
    printf("r");
  } else {
    printf("-");
  }
  if (mode & S_IWUSR) {
    printf("w");
  } else {
    printf("-");
  }
  if (mode & S_IXUSR) {
    printf("x");
  } else {
    printf("-");
  }
  if (mode & S_IRGRP) {
    printf("r");
  } else {
    printf("-");
  }
  if (mode & S_IWGRP) {
    printf("w");
  } else {
    printf("-");
  }
  if (mode & S_IXGRP) {
    printf("x");
  } else {
    printf("-");
  }
  if (mode & S_IROTH) {
    printf("r");
  } else {
    printf("-");
  }
  if (mode & S_IWOTH) {
    printf("w");
  } else {
    printf("-");
  }
  if (mode & S_IXOTH) {
    printf("x");
  } else {
    printf("-");
  }
}

void spacesMaker(int spaces_left, int amount) {
  unsigned int check = amount;
  do {
    check /= 10;
    spaces_left--;
  } while (check != 0);

  while (spaces_left > 0) {
    spaces_left--;
    printf(" ");
  }
}

void standartFormat(struct stat *info, char * type) {
  printf("%s", type);
  ShowRights(info->st_mode);
  printf(" %lu ", info->st_nlink);
  printf(" %d %d ", info->st_gid, info->st_uid);
  printf(" %lu", info->st_size);
  spacesMaker(10, info->st_size);
  struct tm *time = gmtime(&info->st_mtime);
  printf("%s", getMonth(time->tm_mon));
  printf(" %d", time->tm_mday);
  spacesMaker(3, time->tm_mday);
  if (time->tm_hour+3 < 10) {
    printf("0");
  }
  printf("%d:", time->tm_hour+3);
  if (time->tm_min < 10) {
    printf("0");
  }
  printf("%d  ", time->tm_min);
}

pathname MakePath(const pathname current, const char * middle, const char * end) {
  pathname result;
  size_t newsize = strlen(current)+strlen(middle)+strlen(end)+1;
  result = calloc(newsize,1);
  strcat(result, current);
  strcat(result, middle);
  strcat(result, end);
  return result;
}

void Operate(pathname current) {
  struct stat info;
  if (stat(current, &info) < 0) {
    printf("Can\'t get information\n");
    printf("%s\n", strerror(errno));
    _exit(errno);
  } else {
    if (S_ISDIR(info.st_mode)) {
      printf("%s:\n", current);
      DIR *dir;
      if ((dir = opendir(current)) != NULL) {
        struct dirent *internal;
        struct QUEUE folders;
        QUEUEcreate(&folders);
        size_t blocks_summary = 0;
        while ((internal = readdir(dir)) != NULL) {
          if ((internal->d_name[0] != '.') && (strcmp(internal->d_name, "..") != 0)) {
            pathname operating;
            operating = MakePath(current, "/", internal->d_name);
            if (stat(operating, &info) < 0) {
              printf("Can\'t get information\n");
              _exit(errno);
            } else {
              blocks_summary += info.st_blocks;
              if (S_ISDIR(info.st_mode)) {
                standartFormat(&info, "d");
                QUEUEenter(&folders, operating);
                printf("%s", internal->d_name);
              }
              IF_NOT_DIR(internal->d_name);
            }
          }
        }
        printf("summary %lu\n", blocks_summary / 2);
        printf("\n");
        while(!QUEUEempty(&folders)) {
          Operate(QUEUEquit(&folders));
        }
        QUEUEdestroy(&folders);
        closedir(dir);
        return;
      } else {
        printf("Can\'t open the directory %s\n", current);
        _exit(errno);
      }
    }
    IF_NOT_DIR(current);
  }
}

int main (int argc, char **argv) {
  int i;
  if (argc > 1) {
    for (i = 1; i < argc; i++) {
      Operate(argv[i]);
    }
  } else { //without arguments work with current directory
    Operate(".");
  }
  return 0;
}
