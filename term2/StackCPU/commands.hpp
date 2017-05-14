/*Stack instructions*/
/*---<NAME>---<INSTRUCTION CODE>---<NUMBER OF ARGUMENTS>---<CALLING>---------------<DESCRIPTION>---*/
INSTRUCTION("PUSH",   1,                    1,        PUSH(arguments[0]),          "Places the element x to stack");
INSTRUCTION("POP",    2,                    0,        POP(),                       "Delete top element from stack and inserts it to the register");
INSTRUCTION("ADD",    3,                    0,        ADD(),                       "Summarize two last elements, delete terms and place the result of summation to stack");
INSTRUCTION("MUL",    4,                    0,        MUL(),                       "Multiplies two last elements of stack, delete multipliers and place the result to stack");
INSTRUCTION("SUB",    5,                    1,        SUB(arguments[0]),           "Substract x from the last member of stack");
INSTRUCTION("DUMP",   6,                    0,        DUMP(),                      "Special debug instruction");

/*Other instructions*/
/*---<NAME>---<INSTRUCTION CODE>---<NUMBER OF ARGUMENTS>---<CALLING>---------------<DESCRIPTION>---*/
INSTRUCTION("END",    10,                   0,        END(),                       "Special sygnal. Means the end of program");
