#include <iostream>
#include <assert.h>

/*
 * TODO
 * 1) Что конкретно делает reserve? В документации говорится, что
 * он устанавливает минимально возможное количество элементов
 * в векторе. А что если вызвать reserve к вектору в котором больше
 * элеменетов, чем устанавливает reserve?
 * 2) Написать reserve, capacity, clear, erase, push_back, pop_back, resize, swap
 * 3) swap меняет СОДЕРЖИМОЕ двух векторов! Удобно если был вектор размером 10000,
 * а в данный момент в нем 10 элементов, то-есть это нужно, чтобы не таскать за собой
 * большой объем памяти.
 */

template <typename Data_T>
class CVector {
  public:
	CVector();
	CVector(unsigned int size);
  CVector(unsigned size, int value);
  ~CVector();
  CVector& operator=(const CVector& second_vector);
  int operator==(const CVector& second_vector) const;
  Data_T& at(int number);
  Data_T& operator[](int number);
  void print() const;
  Data_T& front();
  Data_T& back();
  bool empty();
  void resize(size_t new_size);

	Data_T* data_;
	int size_;
  private:
};

template <typename Data_T>
CVector<Data_T>::CVector():
  size_(0),
  data_(new Data_T[0])
{}

template <typename Data_T>
CVector<Data_T>::CVector(unsigned int size):
  size_(size),
  data_(new Data_T[size_])
{
  for (int i = 0; i < size; i++)
    data_[i] = 0;
}
template <typename Data_T>
CVector<Data_T>::CVector(unsigned int size, int value):
  size_(size),
  data_(new Data_T[size_])
{
  for (int i = 0; i < size; i++)
    data_[i] = value;
}

template <typename Data_T>
CVector<Data_T>::~CVector() {
  size_ = -1;
  delete [] data_;
  data_ = 0;
}

template <typename Data_T>
CVector<Data_T>& CVector<Data_T>::operator =(const CVector& second_vector) {
  delete [] data_;
  size_ = second_vector.size_;
  data_ = new Data_T[size_];
  for (int i = 0; i < size_; i++)
    data_[i] = second_vector.data_[i];
  return *this;
}

template <typename Data_T>
int CVector<Data_T>::operator ==(const CVector& second_vector) const {
  if (size_ == second_vector.size_) {
    for (int i = 0; i < size_; i++)
      if (data_[i] == second_vector.data_[i]) {
        continue;
      } else {
        return -1;
      }
  } else {
    return -1;
  }
  return 1;
}

template <typename Data_T>
void CVector<Data_T>::print() const {
  for (int i = 0; i < size_; i++)
    std::cout << "a["<<i<<"] = "<<data_[i]<<"\n";
  std::cout <<"\n";
}

template <typename Data_T>
Data_T& CVector<Data_T>::at (int number) {
    /*assert-ы*/
    if (number >= size_)
      assert(!"Out of range!");
    if (number < 0 )
      assert(!"Number is less than 0!");
    return data_[number];
}

/*Safe access too*/
template <typename Data_T>
Data_T& CVector<Data_T>::operator[](int number) {
  return at(number);
}

template <typename Data_T>
Data_T& CVector<Data_T>::front() {
  return at(0);
}

template <typename Data_T>
Data_T& CVector<Data_T>::back() {
  return at(size_-1);
}

template <typename Data_T>
bool CVector<Data_T>::empty() {
  if (data_ == NULL)
    return true;
  return false;
}

template <typename Data_T>
void CVector<Data_T>::resize(size_t new_size) {
  if (size_ == new_size)
    return;
  if (size_ < new_size) {
    Data_T buf[size_];
    for (int i = 0; i < size_; i++)
      buf[i] = data_[i];
    delete [] data_;
    data_ = new Data_T[new_size];
    for (int i = 0; i < size_; i++) {
      if (i < size_)
        data_[i] = buf[i];
    }
    size_ = new_size;
  }
    if (size_ > new_size) {

    }
}
int main() {
  CVector<int> a(2,0);
  CVector<int> b(5,1);
  a.print();
  a = b;
  a.print();
  a[3] = 5;
  a.print();
    /*
	 *int i = *a.at(0);
	 **a.at(0) = 5;
     */
}
