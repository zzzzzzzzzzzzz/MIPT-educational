#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef int info_t;

#define ERRORCHECK                                                                        \
  if (ok()) {                                                                             \
    dump();                                                                               \
    assert(!"Program crashed. Error file was created, please check stack_error_log.txt"); \
  };

enum stack_errors {
  WRONG_INDEX = 1,
  BAD_ARRAY = 2
};

class CStack {
  public:
    CStack(size_t size);
    ~CStack();
    void push(info_t value);
    info_t pop();
    info_t get();
    void print() const;

  private:
    CStack(); //запрещаем инициализацию без параметров
    void dump() const;
    int ok();
    info_t * data_;
    int dimension_;
    int count_;
    int error_;
};

CStack::CStack(size_t size) :
  data_(new info_t[size]),
  count_(0),
  error_(0),
  dimension_(size)
  {
      memset(data_, 0, dimension_ * sizeof (info_t));
  }

CStack::~CStack() {
  delete [] data_;
  data_ = NULL;
  count_ = -1;
  dimension_ = -1;
  error_ = -1;
}

int CStack::ok() {
  if (count_ > dimension_ || count_ < 0) { error_ = WRONG_INDEX; }
  if (!data_) { error_ = BAD_ARRAY; }
  return error_;
}

void CStack::push(info_t value) {
  ERRORCHECK;
  if (count_ >= dimension_) { //если переполнение
    data_ = (info_t*)realloc(data_, sizeof(info_t) * (dimension_ *= 2));
    ERRORCHECK;
    for (int i = count_; i < dimension_; i++)
      data_[i] = 0;
  }
  data_[count_++] = value;
}

info_t CStack::pop() {
  ERRORCHECK;
  info_t result = data_[--count_];
  data_[count_] = 0;
  ERRORCHECK;
  return result;
}

info_t CStack::get() {
  ERRORCHECK;
  if (count_) {
    return data_[count_-1];
  } else {
    printf("Stack is empty, returned 0");
    return 0;
  }
}

void CStack::print() const {
  int i = 0;
  if (count_) {
    for (; i < count_; i++)
      printf("[%d] ", data_[i]);
  } else
    printf("Stack is empty, really, absolutely");
  printf("\n");
}

void CStack::dump() const {
  FILE *err_log = fopen("stack_error_log.txt", "w");
  fprintf(err_log,"\n\n\n========================================\n");
  fprintf(err_log,"   Program crashed with ERROR CODE %d\n", error_);
  fprintf(err_log,"========================================\n\n\n");
  fprintf(err_log,"Stack dimension: %d\n", dimension_);
  fprintf(err_log,"Stack number of elements (count): %d\n", count_);
  int i = 0;
  for (; i < count_; i++)
    fprintf(err_log, "[0x%X]   data[%d] = %d\n", data_[i], i, data_[i]);
  for (; i < dimension_; i++)
    fprintf(err_log, "[0x%X]  *data[%d] = %d\n", data_[i], i, data_[i]);
  fprintf(err_log,"\n\n\n========================================\n\n\n");
  fclose(err_log);
}

int main () {
  CStack* test = new CStack(10);
  test->push(5);
  test->push(3);
  test->push(10);
  test->push(15);
  test->push(5);
  test->push(3);
  test->push(10);
  test->push(15);
  test->push(5);
  test->push(3);
  test->push(10);
  test->push(15);
  test->pop();
  test->print();
  return 0;
}
