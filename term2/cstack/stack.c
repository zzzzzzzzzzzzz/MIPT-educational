#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/*
 * TODO
 * 1) Доделай до конца. Stack_ok чзнх, что должно делать?
 * 2) Разбери краевые случаи. Чем больше, тем лучше.
 * 3) Разберись с assert. (статья на хабре)
 * 4) Проконсультируйся с народом
 * 5) Переписывай на плюсы
 */

typedef int info_t;
typedef struct Stack Stack_t;

struct Stack {
  info_t * data;
  int dimension;
  int count;
  int error;
};

enum stack_errors {
  OK = 0,
  BAD_POINTER = 1,
  BAD_POP = 2
};

int Stack_create(Stack_t * stackVariable,size_t size);
void Stack_destroy(Stack_t * stackVariable);
void Stack_push(Stack_t * stackVariable, info_t value);
info_t Stack_pop(Stack_t * stackVariable);
info_t Stack_get(Stack_t* stackVariable);
void Stack_dump(Stack_t * stackVariable);
void Stack_print(Stack_t * stackVariable);
void Stack_ok(Stack_t * stackVariable);

int main() {
  Stack_t test;
  Stack_create(&test, 10);
  Stack_push(&test,2);
  Stack_push(&test,8);
  Stack_push(&test,9);
  Stack_push(&test,10);
  Stack_push(&test,0);
  Stack_push(&test,7);
  Stack_push(&test,8);
  Stack_push(&test,101);
  Stack_push(&test,115);
  Stack_push(&test,94);
  Stack_push(&test,20);
  Stack_print(&test);
  Stack_pop(&test);
  Stack_pop(&test);
  printf("%d\n", Stack_get(&test));
  printf("%d\n", Stack_get(&test));
  printf("удалил %d\n", Stack_pop(&test));
  Stack_print(&test);
  printf("удалил %d\n", Stack_pop(&test));
  printf("удалил %d\n", Stack_pop(&test));
  printf("удалил %d\n", Stack_pop(&test));
  printf("удалил %d\n", Stack_pop(&test));
  Stack_print(&test);
  Stack_destroy(&test);
  return 0;
}

int Stack_create(Stack_t * stackVariable,size_t size) {
  stackVariable->count = 0;
  stackVariable->error = 0;//ok
  stackVariable->data = (info_t*)calloc(size, sizeof(info_t));
  stackVariable->dimension = size;
  return 0;
}

void Stack_ok(Stack_t * stackVariable) {
  if (!stackVariable) { stackVariable->error = BAD_POINTER; }
  if (stackVariable->count < 0) { stackVariable->error = BAD_POP; }
  if (stackVariable->error != OK) {
    Stack_dump(stackVariable);
    assert(!"Program crashed. Error file was created, please check stack_error_log.txt");
  }
}

void Stack_destroy(Stack_t * stackVariable) {
  Stack_ok(stackVariable);
  while (stackVariable->count != 0)
    stackVariable->data[--stackVariable->count] = 0;
  free(stackVariable->data);
  stackVariable->count = -1;
  stackVariable->dimension = -1;
  stackVariable->error = -1;
  stackVariable->data = NULL;
}

void Stack_push(Stack_t * stackVariable, info_t value) {
  Stack_ok(stackVariable);
  while (stackVariable->count >= stackVariable->dimension) { //если переполнение
    stackVariable->data = (info_t*)realloc(stackVariable->data, sizeof(info_t) * (stackVariable->dimension *= 2));
    Stack_ok(stackVariable);
    int i = stackVariable->count;
    for (; i < stackVariable->dimension; i++)
      stackVariable->data[i] = 0;
  }
  stackVariable->data[stackVariable->count++] = value;
}

info_t Stack_pop(Stack_t * stackVariable) {
  Stack_ok(stackVariable);
  info_t result = stackVariable->data[--stackVariable->count];
  stackVariable->data[stackVariable->count] = 0;
  Stack_ok(stackVariable);
  return result;
}

info_t Stack_get(Stack_t * stackVariable) {
  Stack_ok(stackVariable);
  if (stackVariable->count) {
    return stackVariable->data[stackVariable->count-1];
  } else {
    printf("Stack is empty, returned 0");
    return 0;
  }
}

void Stack_print(Stack_t * stackVariable){
  Stack_ok(stackVariable);
  int i = 0;
  if (stackVariable->count) {
    for (; i < stackVariable->count; i++)
      printf("[%d] ", stackVariable->data[i]);
  } else
    printf("Stack is empty, really, absolutely");
  printf("\n");
}

void Stack_dump(Stack_t * stackVariable){
  FILE *err_log = fopen("stack_error_log.txt", "w");
  fprintf(err_log,"\n\n\n========================================\n");
  fprintf(err_log,"   Program crashed with ERROR CODE %d\n", stackVariable->error);
  fprintf(err_log,"========================================\n\n\n");
  fprintf(err_log,"Stack dimension: %d\n", stackVariable->dimension);
  fprintf(err_log,"Stack number of elements (count): %d\n", stackVariable->count);
  int i = 0;
  for (; i < stackVariable->count; i++)
    fprintf(err_log, "[0x%X]   data[%d] = %d\n", stackVariable->data[i], i, stackVariable->data[i]);
  for (; i < stackVariable->dimension; i++)
    fprintf(err_log, "[0x%X]  *data[%d] = %d\n", stackVariable->data[i], i, stackVariable->data[i]);
  fprintf(err_log,"\n\n\n========================================\n\n\n");
  fclose(err_log);
}
