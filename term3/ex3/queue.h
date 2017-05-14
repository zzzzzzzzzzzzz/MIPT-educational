#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef char* pathname;

struct QUEUE_node {
  struct QUEUE_node *next;
  pathname name;
};

struct QUEUE {
  struct QUEUE_node *front;
  struct QUEUE_node *back;
};

void QUEUEcreate(struct QUEUE *f) {
  f->front = NULL;
  f->back = NULL;
}

void QUEUEdestroy(struct QUEUE *f) {
    struct QUEUE_node *tmp;
    while (f->front != NULL) {
      tmp = f->front->next;
      free(f->front);
      f->front = tmp;
    }
}

void QUEUEenter(struct QUEUE *f, pathname node) {
  struct QUEUE_node *tmp;
  if (node == NULL)
    assert(!"empty pathname!");
  tmp = (struct QUEUE_node*)malloc(sizeof(struct QUEUE_node));
  tmp->name = node;
  tmp->next = NULL;
  if (f->front == f->back && f->back != NULL) {
    f->front->next = tmp;
  }
  if (f->back != NULL) {
    f->back->next = tmp;
  }
  f->back = tmp;
  if (f->front == NULL){
    f->front = f->back;
  }
}

pathname QUEUEquit(struct QUEUE *f) {
  pathname node;
  struct QUEUE_node *tmp;
  if (f->front != NULL) {
    node = (f->front)->name;
    tmp = f->front->next;
    free(f->front);
    f->front = tmp;
  } else {
    node = NULL;
  }
   return node;
}

int QUEUEempty(struct QUEUE *f) {
  return (f->front == NULL);
}

size_t QUEUEtopElemSize(struct QUEUE *f) {
  if (!QUEUEempty) {
    int i = 0;
    while (f->front->name[i]) {
      i++;
    }
    return i;
  } else {
    assert(!"Queue is empty");
  }
}

void QUEUEdump(struct QUEUE *f) {
  struct QUEUE_node *tmp;
  tmp = f->front;
  if (!QUEUEempty(f)) {
  while (1) {
      fprintf(stdout, "%s<-", tmp->name);
      if (tmp->next == NULL)
        break;
      tmp = tmp->next;
    }
  }
  fprintf(stdout, "-|\n");
}

