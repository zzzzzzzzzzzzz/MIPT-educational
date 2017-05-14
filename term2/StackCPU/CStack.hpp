#include <assert.h>
#include <iostream>
#include <cstring>
#include <fstream>

#define ERRORCHECK                                                                        \
  if (not_ok()) {                                                                         \
    dump();                                                                               \
    assert(!"Program crashed. Error file was created, please check stack_error_log.txt"); \
  };

template < typename info_T, size_t size >
class CStack {
  public:
    CStack();
    ~CStack();
    void push(info_T value);
    info_T pop();
    info_T get();
    void print() const;
    void is_empty() const;

  private:
    void dump() const;
    int not_ok();
    info_T * data_;
    int dimension_;
    int count_;
    int error_;
    enum stack_errors_ {
      BAD_INDEX = 1,
      BAD_ARRAY = 2
    };
};

template < typename info_T, size_t size >
CStack< info_T, size >::CStack() :
  data_(new info_T[size]),
  count_(0),
  error_(0),
  dimension_(size)
  {
      memset(data_, 0, dimension_ * sizeof (info_T));
  }

template < typename info_T, size_t size >
CStack< info_T, size >::~CStack() {
  delete [] data_;
  data_ = NULL;
  count_ = -1;
  dimension_ = -1;
  error_ = -1;
}

template < typename info_T, size_t size >
int CStack< info_T, size >::not_ok() {
  if (count_ >= dimension_ || count_ < 0) { error_ = BAD_INDEX; }
  if (!data_) { error_ = BAD_ARRAY; }
  return error_;
}

template < typename info_T, size_t size >
void CStack< info_T, size >::push(info_T value) {
  ERRORCHECK;
  data_[count_++] = value;
}

template < typename info_T, size_t size >
info_T CStack< info_T, size >::pop() {
  ERRORCHECK;
  info_T result = data_[--count_];
  data_[count_] = 0;
  ERRORCHECK;
  return result;
}

template < typename info_T, size_t size >
info_T CStack< info_T, size >::get() {
  ERRORCHECK;
  if (count_) {
    return data_[count_-1];
  } else {
    printf("Stack is empty, returned 0");
    return 0;
  }
}

template < typename info_T, size_t size >
void CStack< info_T, size >::print() const {
  int i = 0;
  if (count_) {
    for (; i < count_; i++)
      std::cout <<"["<< data_[i] <<"] ";
  } else
    std::cout <<"Stack is empty, really, absolutely";
  printf("\n");
}

template < typename info_T, size_t size >
void CStack< info_T, size >::dump() const {
  std::ofstream err_log ("stack_error_log.txt");
  err_log << "\n\n\n========================================\n";
  err_log << "   Program crashed with ERROR CODE "<< error_ <<"\n";
  err_log << "========================================\n\n\n";
  err_log << "Stack dimension: "<< dimension_ <<"\n";
  err_log << "Stack number of elements (count): "<< count_ <<"\n";
  int i = 0;
  for (; i < count_; i++)
    err_log << "["<< &data_[i] <<"]   data["<< i <<"] = "<< data_[i] <<"\n";
  for (; i < dimension_; i++)
    err_log << "[" << &data_[i] <<"]  *data["<< i <<"] = "<< data_[i] <<"\n";
  err_log << "\n\n\n========================================\n\n\n";
  err_log.close();
}

template < typename info_T, size_t size >
void CStack< info_T, size >::is_empty() const {
  if (count_)
    return 0;
  else
    return 1;
}

#undef ERRORCHECK

