#include <time.h>
#include <vector>
#include <assert.h>
#include <stdio.h>

/*
 * TODO
 * 1) Переделать с фиксированной точностью!
 * 2) Разобраться работает ли здесь метод перемножения Карацюбы!
 * 3) Реализовать знак (в принципе необязательно)
 */

/*
 * Program calculates n signes of PI after coma
 * based on 2sqrt(3) {summation} (-1)^k/(3^k(2k+1)) series
 */

/*
 * BigFloat x -> {mantissa, base, order}
 * x = (z0*(base)^(0) + m1*(base)^(-1) + mn*(base)^(-n)) * base^(order)
 * where z0 is the integer part of number
 */

template < size_t base_T >
class BigFloat {
  public:
    BigFloat(unsigned int order);
    ~BigFloat();
    void printTo(FILE * to_file) const;
    void operator +=(const BigFloat *summand);
    void operator -=(const BigFloat *subtrahend);
    void operator *=(const BigFloat *second_factor);
    void operator =(const BigFloat *to_copy);
    BigFloat* fractionToBigFloat(int numerator, int denominator, int max_numbers_after_dot);
    void insertTo(int index, int what);
    unsigned int order_;
    unsigned int number_end_;
  private:
    BigFloat();
    std::vector<unsigned int> mantissa_;
};

template < size_t base_T >
BigFloat< base_T >::BigFloat(unsigned int order) :
  number_end_(0),
  order_(order),
  mantissa_(std::vector<unsigned int>(order+1))
{}

template < size_t base_T >
BigFloat< base_T >::~BigFloat() {
  order_ = 0;
  mantissa_.~vector();
}

template < size_t base_T >
void BigFloat< base_T >::printTo(FILE * to_file = stdout) const {
  fprintf(to_file, "\n%d.", mantissa_.at(0));
  for (int i = 1; i <= number_end_; i++) {
    fprintf(to_file, "%d", mantissa_[i]);
  }
  fprintf(to_file, "\n");
}

template < size_t base_T >
void BigFloat< base_T >::operator +=(const BigFloat *summand) {
  if (number_end_ < summand->number_end_) {
    number_end_ = summand->number_end_;
    if (order_ < summand->number_end_) {
      order_ = summand->number_end_;
      mantissa_.resize(order_+1, 0);
    }
  }
  unsigned int buf = 0;
  for (int i = summand->number_end_; i >= 0; i--) {
    mantissa_[i] += (summand->mantissa_[i] + buf);
    buf = mantissa_[i] / base_T;
    if (i != 0)
      mantissa_[i] %= base_T;
  }
}

template < size_t base_T >
void BigFloat< base_T >::operator -=(const BigFloat *subtrahend) {
  if (number_end_ < subtrahend->number_end_) {
    number_end_ = subtrahend->number_end_;
    if (order_ < subtrahend->number_end_) {
      order_ = subtrahend->number_end_;
      mantissa_.resize(order_+1, 0);
    }
  }
  unsigned int buf = 0;
  int difference = 0;
  for (int i = subtrahend->number_end_; i > 0; i--) {
    difference = mantissa_[i] - subtrahend->mantissa_[i];
    if (difference < 0) {
      int j = i-1;
      for (; j >= 0 && mantissa_[j] == 0; j--) {
        mantissa_[j] = base_T-1;
      }
      mantissa_[j]--;
      difference += base_T;
    }
    mantissa_[i] = difference;
  }
  mantissa_.at(0) -= subtrahend->mantissa_.at(0);
}

template < size_t base_T >
void BigFloat<base_T>::operator*=(const BigFloat<base_T> *second_factor) {
  if (order_ < second_factor->number_end_ + number_end_) {
    order_ += second_factor->number_end_;
    mantissa_.resize(order_ + 1, 0);
  }
  int old_number_end = number_end_;
  number_end_ += second_factor->number_end_;
  std::vector <unsigned int> temp_mant(number_end_+1,0);
  int buf = 0;
  for (int j = second_factor->number_end_; j >= 0; j--) {
    if (second_factor->mantissa_[j] == 0)
      continue;
    for (int i = old_number_end; i >= 0; i--) {
      buf += mantissa_[i]*second_factor->mantissa_[j];
      temp_mant[i+j] += buf % base_T;
      buf /= base_T;
      buf += temp_mant[i+j] / base_T;
      temp_mant[i+j] %= base_T;
    }
  }
  mantissa_ = temp_mant;
}

template < size_t base_T >
void BigFloat<base_T>::operator =(const BigFloat<base_T> *to_copy) {
  order_ = to_copy->number_end_;
  mantissa_.resize(order_ + 1, 0);
  number_end_ = to_copy->number_end_;
  for (int i = 0; i <= number_end_; i++)
    mantissa_[i] = to_copy->mantissa_[i];
}

template < size_t base_T >
void BigFloat< base_T >::insertTo(int index, int what) {
  number_end_ = index;
  if (number_end_ > order_) {
    mantissa_.resize(number_end_ + 1, 0);
  }
  mantissa_.at(index) = what;
}

template < size_t base_T >
BigFloat< base_T >* BigFloat< base_T >::fractionToBigFloat(int numerator, int denominator, int max_numbers_after_dot) {
  int i = 0;
  int in_base = 0;
  while (i < max_numbers_after_dot && numerator != 0) {
    while (numerator < denominator) {
      numerator *= base_T;
      i++;
    }
    in_base = numerator / denominator;
    while (in_base != 0) {
      insertTo(i, in_base % base_T);
      in_base /= base_T;
    }
    numerator %= denominator;
  }
  return this;
}

int main() {
  //unsigned int amount = 0;
  //scanf("%d", &amount);
  BigFloat<10> *d = new BigFloat<10>(100);
  d->insertTo(0,0);
  d->insertTo(1,2);
  d->insertTo(2,5);
  BigFloat<10> *c = new BigFloat<10>(100);
  c->insertTo(0,0);
  c->insertTo(1,2);
  c->insertTo(2,5);
  BigFloat<10> *last_mul = new BigFloat<10>(100);
  BigFloat<10> *first = new BigFloat<10>(100);
  BigFloat<10> *second = new BigFloat<10>(100);
  BigFloat<10> *third = new BigFloat<10>(100);
  BigFloat<10> *sum = new BigFloat<10>(100);
  BigFloat<10> *nil = new BigFloat<10>(100);
  BigFloat<10> *res = new BigFloat<10>(100);
  FILE * output = fopen("output.txt", "w");
  clock_t start_time = clock();
  for (int i = 0; i < 100; i++) {
    //clock_t check_time = clock();
    *sum += first->fractionToBigFloat(40*i*i+42*i+10, (2*i+1)*(4*i+1)*(4*i+3),100);
    //clock_t now_time = clock();
    //fprintf(output ,"\n\n fraction to BigFloat Time %g s", (float)(now_time-check_time) / CLOCKS_PER_SEC);
    //check_time = clock();
    if (i == 0) {
      *last_mul = sum;
    }
    if (i == 1) {
      *last_mul = d;
      *last_mul *= sum;
    }
    if (i > 1) {
      *d *= c;
      *last_mul = d;
      *last_mul *= sum;
    }
    //now_time = clock();
    //fprintf(output ,"\n\n mul time %g s", (float)(now_time-check_time) / CLOCKS_PER_SEC);
    //check_time = clock();
    if (i % 2 == 0)
      *res += last_mul;
    else
      *res -= last_mul;
    *sum = nil;
    *first = nil;
    //now_time = clock();
    //fprintf(output ,"\n\n sum and sub time %g s", (float)(now_time-check_time) / CLOCKS_PER_SEC);
  }
  clock_t end_time = clock();
  clock_t result_time = (end_time - start_time);
  fprintf(output ,"\n\nTime %g s", (float)result_time / CLOCKS_PER_SEC);
  res->printTo(output);
  /*salt*/
  fclose(output);
}

