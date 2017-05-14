#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <math.h>

template < size_t precision_T, size_t base_T >
class BigFloat {
  public:
    BigFloat();
    BigFloat(const BigFloat& what);
    BigFloat(const char *source);
    BigFloat(int * source, size_t count);
    ~BigFloat();
    void operator +=(const BigFloat& summand);
    BigFloat operator +(const BigFloat& summand);
    void operator -=(const BigFloat& subtrahend);
    BigFloat operator -(const BigFloat& subtrahend);
    void operator *=(const BigFloat& second_factor);
    BigFloat operator *(const BigFloat& second_factor);
    BigFloat operator /(const BigFloat& divisor);
    BigFloat& operator =(const BigFloat& what);
    bool operator <= (const BigFloat& b);
    BigFloat MulOnNumber(const int num);
    BigFloat DivOnNumber(const int divisor);
    BigFloat sqrtBinarySearch();
    BigFloat pow(int n);
    void printNTo(int how_many, FILE *to_file) const;
    int end_of_number_;
  private:
    int *buf_;
    int *exp_;
    int findNum(const char symbol);
};

template < size_t precision_T, size_t base_T >
BigFloat< precision_T,base_T >::BigFloat() :
  end_of_number_(0),
  buf_(new int[precision_T]),
  exp_(new int[precision_T])
{
  for (int i = 0; i < precision_T; i++) {
    exp_[i] = 0;
    buf_[i] = 0;
  }
  end_of_number_ = 0;
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T >::BigFloat(const BigFloat< precision_T, base_T >& what) :
  end_of_number_(what.end_of_number_),
  buf_(new int[precision_T]),
  exp_(new int[precision_T])
{
  for (int i = 0; i < precision_T; i++) {
    if  (i <= end_of_number_)
      exp_[i] = what.exp_[i];
    else
      exp_[i] = 0;
    buf_[i] = 0;
  }
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T ,base_T >::BigFloat(int *source, size_t count) :
  end_of_number_(0),
  buf_(new int[precision_T]),
  exp_(new int[precision_T])
{
  for (int i = 0; i < precision_T; i++) {
    if (i < count) {
      exp_[i] = source[i];
      end_of_number_ = i;
    } else {
    exp_[i] = 0;
    }
    buf_[i] = 0;
  }
}
template < size_t precision_T, size_t base_T >
int BigFloat< precision_T, base_T >::findNum(const char symbol) {
  char reg[] = {'0','1','2','3','4','5','6','7','8','9'};
  int j = 0;
  while (symbol != reg[j] && j < 10) {
    j++;
  }
  if (j == 10)
    assert(!"Unknown character!");
  return j;
}
template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T >::BigFloat(const char *source) :
  end_of_number_(0),
  buf_(new int[precision_T]),
  exp_(new int[precision_T])
{
  for (int i = 0; i < precision_T; i++) {
    exp_[i] = 0;
    buf_[i] = 0;
  }
  int length = strlen(source);
  int k = 0;
  while (source[k] != '.' && k < length) {
    exp_[0] *= base_T;
    exp_[0] += findNum(source[k]);
    k++;
  }
  length--;
  for (int i = k; i < length; i++) {
    k++;
    if (i != 0) 
      exp_[i] = findNum(source[k]) * (base_T / 10);
    end_of_number_ = i;
  }
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T ,base_T >::~BigFloat() {
  delete [] exp_;
  delete [] buf_;
  exp_ = NULL;
  buf_ = NULL;
  end_of_number_ = 0;
}

template < size_t precision_T, size_t base_T >
void BigFloat< precision_T ,base_T >::printNTo(int how_many = precision_T, FILE * to_file = stdout) const {
  fprintf(to_file, "\n%d.", exp_[0]);
  for (int i = 1; i < how_many; i++) {
    fprintf(to_file, "%d" , exp_[i]);
  }
  fprintf(to_file, "\n");
}

template < size_t precision_T, size_t base_T >
void BigFloat< precision_T ,base_T >::operator +=(const BigFloat& summand) {
  int temp = 0;
  for (int i = summand.end_of_number_; i >= 0; i--) {
    exp_[i] += (summand.exp_[i] + temp);
    if (exp_[i] < base_T) temp = 0;
    else {
      temp = 1;
      if (i != 0)
        exp_[i] -= base_T;
    }
  }
  if (end_of_number_ < summand.end_of_number_) {
    end_of_number_ = summand.end_of_number_;
  }
  while (exp_[end_of_number_] == 0 && end_of_number_ > 0)
    end_of_number_--;
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T> BigFloat< precision_T, base_T >::operator+(const BigFloat& summand) {
  BigFloat< precision_T, base_T> res(*this);
  res += summand;
  return res;
}

//только из большего меньшее
template < size_t precision_T, size_t base_T >
void BigFloat< precision_T ,base_T >::operator -=(const BigFloat& subtrahend) {
  int difference = 0;
  for (int i = subtrahend.end_of_number_; i >= 0; i--) {
    exp_[i] -= subtrahend.exp_[i];
    if (exp_[i] < 0 && i) {
      exp_[i] += base_T;
      exp_[i-1]--;
    }
  }
  if (end_of_number_ < subtrahend.end_of_number_) {
    end_of_number_ = subtrahend.end_of_number_;
  }
  while (exp_[end_of_number_] == 0 && end_of_number_ > 0)
    end_of_number_--;
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T> BigFloat< precision_T, base_T >::operator-(const BigFloat& subtrahend) {
  BigFloat< precision_T, base_T> res(*this);
  res -= subtrahend;
  return res;
}

template < size_t precision_T, size_t base_T >
void BigFloat< precision_T, base_T >::operator*=(const BigFloat<precision_T, base_T>& second_factor) {
  int temp = 0;
  for (int j = second_factor.end_of_number_; j >= 0; j--) {
    for (int i = end_of_number_; i >= 0; i--) {
      temp = 0;
      if (i+j < precision_T) {
        buf_[i+j] += second_factor.exp_[j]*exp_[i];
        if (i+j != 0) {
          temp = buf_[i+j] / base_T;
          buf_[i+j-1] += temp;
          buf_[i+j] -= temp*base_T;
        }
      } else {
        if (i+j == precision_T) {
          int out_of_range = second_factor.exp_[j]*exp_[i] + temp; 
          temp += out_of_range / base_T;
          if (out_of_range % base_T >= base_T/2)
            temp++;
          buf_[i+j-1] += temp;
        }
        continue;
      }
    }
  }
  end_of_number_ += second_factor.end_of_number_;
  if (end_of_number_ >= precision_T)
    end_of_number_ = precision_T - 1;
  while (buf_[end_of_number_] == 0 && end_of_number_ > 0)
    end_of_number_--;
  for (int i = 0; i <= end_of_number_; i++) {
    exp_[i] = buf_[i];
    buf_[i] = 0;
  }
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T > BigFloat< precision_T, base_T >::operator *(const BigFloat< precision_T, base_T >& second_factor) {
  BigFloat< precision_T, base_T> res(*this);
  res *= second_factor;
  return res;
}

template < size_t precision_T, size_t base_T >
bool BigFloat< precision_T, base_T >::operator <=(const BigFloat<precision_T, base_T>& b)
{
    for (int i = 0; i <= end_of_number_; i++) {
        if (exp_[i] != b.exp_[i])
            return exp_[i] <= b.exp_[i];
    }
    return true;
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T > BigFloat< precision_T, base_T >::operator /(const BigFloat< precision_T, base_T >& divisor) {
  BigFloat< precision_T, base_T > res;
  for (int i = 0; i < precision_T; i++)
  {
    res.exp_[i] = exp_[i];
    res.end_of_number_ = i;
    int x = 0;
    int left = 0, right = base_T;
    while (left <= right)
    {
      int num = (left + right) >> 1;
      res.exp_[i] = num;
      if (res * divisor <= *this)
      {
        x = num;
        left = num+1;
      }
      else
        right = num-1;
    }
    res.exp_[i] = x;
  }
 
  return res;   
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T >& BigFloat< precision_T, base_T >::operator =(const BigFloat< precision_T, base_T >& what) {
  if (this != &what) {
    if (end_of_number_ > what.end_of_number_)
      for (int i = what.end_of_number_; i <= end_of_number_; i++)
        exp_[i] = 0;
    end_of_number_ = what.end_of_number_;
    for (int i = 0; i <= end_of_number_; i++) {
      exp_[i] = what.exp_[i];
    }
  }
  return *this;
}

//Searching of sqrt by dichotomy
template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T > BigFloat< precision_T, base_T >::sqrtBinarySearch() {
  BigFloat< precision_T, base_T > res;
  int i = 0;
  while (i < precision_T) {
    int left = 0, right = base_T-1;
    int curDigit = 0;
    res.end_of_number_ = i;
    while (left <= right) { // подбираем текущую цифру
      int num = (left+right)>>1;
      res.exp_[i] = num;
      if (res * res <= *this) {
        curDigit = num;
        left = num + 1;
      }
      else 
        right = num - 1;
    }
    res.exp_[i] = curDigit;
    i++;
  }
  return res;
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T> BigFloat< precision_T, base_T >::pow(int n) {
  if (n == 0 || n == 1)
    assert(!"Type the number more than 1");
  for (int i = 1; i < n; i++)
    *this *= *this;
  return *this;
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T > BigFloat< precision_T, base_T >::MulOnNumber(const int num) {
  int temp = 0;
  int expi = 0;
  for (int i = end_of_number_; i >= 0; i--) {
    expi = exp_[i]*num;
    expi += temp;
    if (expi >= base_T && i) {
      temp = expi/base_T;
      expi -= temp*base_T;
    } else
      temp = 0;
    exp_[i] = expi;
  }
  while (exp_[end_of_number_] == 0 && end_of_number_)
    end_of_number_--;
  return *this;
}

template < size_t precision_T, size_t base_T >
BigFloat< precision_T, base_T > BigFloat< precision_T, base_T >::DivOnNumber(const int divisor) {
  int temp = 0;
  int expi = 0;
  int num = 0;
  int i = 0;
  BigFloat< precision_T, base_T> answ;
  for (; i < precision_T; i++) {
    expi = exp_[i] + temp*base_T;
    num = expi / divisor;
    temp = expi - num * divisor;
    answ.exp_[i] = num;
  }
  i--;
  while (answ.exp_[i] == 0 && i)
    i--;
  answ.end_of_number_ = i;
  *this = answ;
  return *this;
}

int main() {
  //int amount = 0;
  //scanf("%d", &amount);
  BigFloat<1000,10> sqrt2("2");
  sqrt2 = sqrt2.sqrtBinarySearch();
  BigFloat<1000,10> an("1");
  BigFloat<1000,10> an_1;
  sqrt2.DivOnNumber(2);
  BigFloat<1000,10> bn(sqrt2);
  int x[] = {0, 2,5};
  BigFloat<1000,10> tn(x,3);
  BigFloat<1000,10> pn("1");
  BigFloat<1000,10> res;
  FILE * output = fopen("output.txt", "w");
  clock_t start_time = clock();
  for (int i = 0; i < 9; i++) {
    an_1 = an + bn;
    an_1.DivOnNumber(2);
    bn *= an;
    bn = bn.sqrtBinarySearch();
    tn -= pn * (an - an_1).pow(2);
    //tn.printNTo();
    pn += pn;
    an = an_1;
  }
  tn.MulOnNumber(4);
  an += bn;
  an *= an;
  res = an / tn;
  clock_t end_time = clock();
  clock_t result_time = (end_time - start_time);
  res.printNTo(res.end_of_number_+1, output);
  fprintf(output ,"\n\nTime %g s", (float)result_time / CLOCKS_PER_SEC);
  fclose(output);
}
