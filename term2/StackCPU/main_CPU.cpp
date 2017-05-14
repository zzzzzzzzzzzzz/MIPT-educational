#include "CStack.hpp"
#include <cstdlib>
#include <vector>

class CPU {
  public:
    CPU();
    ~CPU();
    int Compile(const char *name_of_file);
  private:
    int Compile();
    int Run();
    int PUSH(int arg);
    int POP();
    int ADD();
    int MUL();
    int SUB(int arg);
    int DUMP();
    int END();

    CStack <int, 10> *Stack_;
    std::vector<int> machine_code_;

    int run_flag_;
    int ax_;
    int bx_;
    int cx_;
    int dx_;
    enum CPU_constants {
      SUCCESS = 777,
      READY = 666
    };
};

CPU::CPU() :
  run_flag_(READY),
  ax_(0),
  bx_(0),
  cx_(0),
  dx_(0)
{
  Stack_ = new CStack<int, 10>();
  std::vector<int> machine_code_(10, 0);
}

CPU::~CPU() {
  Stack_->~CStack();
  Stack_ = NULL;
  machine_code_.~vector();
  ax_ = -1;
  bx_ = -1;
  cx_ = -1;
  dx_ = -1;
  run_flag_ = -1;
}

int CPU::Compile(const char *name_of_file) {
  std::ifstream input_file(name_of_file);
  std::string line;
  std::vector<std::string> lexems;
  while (getline(input_file,line,'\n')) {
    char *cstr_line = new char [line.length()+1];
    strcpy (cstr_line, line.c_str());
    printf("%s\n", cstr_line);
    char * pch = strtok (cstr_line," ,"); // разделители пробел и запятая
    while (pch != NULL) { // пока есть лексемы
      lexems.push_back(pch);
      pch = strtok (NULL, " ,");
    }
    #define INSTRUCTION(NAME, CODE, ARG_NUM, CALLING, DESC)                     \
      if (lexems[0].compare(NAME) == 0) {                                       \
        machine_code_.push_back(CODE);                                          \
        lexems[0] = "\0";                                                       \
        if (ARG_NUM) {                                                          \
          int i = 1;                                                            \
          while (i <= ARG_NUM && i < lexems.size()) {                           \
            machine_code_.push_back(strtol(lexems[i].c_str(), NULL, 10));       \
            lexems[i] = "\0";                                                   \
            i++;                                                                \
          }                                                                     \
          if (i < lexems.size())                                                \
            assert(!"one or more elements seem to be not checked");             \
          if (i <= ARG_NUM)                                                     \
            assert(!"not enough arguments");                                    \
        }                                                                       \
      }
    #include "commands.hpp"
    #undef INSTRUCTION
    lexems.clear();
  }
  lexems.~vector();
  Run();
 }

int CPU::Run() {
  int i = 0;
  int instr_code = 0;
  int arguments[3];
  while (run_flag_ != SUCCESS) {
    instr_code = machine_code_[i];
    switch (instr_code) {
    #define INSTRUCTION(NAME, CODE, ARG_NUM, CALLING, DESC)                     \
      case CODE:                                                                \
                for (int j = 0; j < ARG_NUM; j++)                               \
                  arguments[j] = machine_code_[i+1+j];                          \
                CPU::CALLING;                                                   \
                i+=ARG_NUM;                                                     \
                break;                                                          \

    #include "commands.hpp"
    #undef INSTRUCTION
    default:
           assert(!"No such instruction!");
    }
    i++;
  }
}

int CPU::PUSH(int arg) {
  Stack_->push(arg);
  return 0;
}

int CPU::POP() {
  ax_ = Stack_->pop();
  return 0;
}

int CPU::ADD() {
  int first = Stack_->pop();
  int second = Stack_->pop();
  Stack_->push(first+second);
  return 0;
}

int CPU::MUL() {
  int first = Stack_->pop();
  int second = Stack_->pop();
  Stack_->push(first*second);
  return 0;
}

int CPU::SUB(int arg) {
  Stack_->push(Stack_->pop() - arg);
  return 0;
}

int CPU::DUMP() {
  Stack_->print();
}

int CPU::END() {
  run_flag_ = SUCCESS;
}

int main() {
  CPU *sample = new CPU();
  sample->Compile("codeTest.txt");
  return 0;
}
