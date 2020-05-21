#ifndef SAS_INSTRUCTIONS_HPP
#define SAS_INSTRUCTIONS_HPP

enum COMMANDS {
    _END  = 0,
    _ADD  = 1,
    _SUB  = 2,
    _MUL  = 3,
    _DIV  = 4,
    _MOD  = 5,
    _DUMP = 6,
    _POP  = 7,
    _IN   = 8,
    _OUT  = 9,
    _PUSH = 10,
    _MOV  = 11,
    _OUTC = 12,
    _POW  = 13,
    _JMP  = 14,
    _JA   = 15,
    _JAE  = 16,
    _JB   = 17,
    _JBE  = 18,
    _JE   = 19,
    _JNE  = 20,
    _CALL = 21,
    _RET  = 22,
    _SQRT = 23,
    _OUTF = 24,
    NUM_OF_COMMANDS = 25
};

static const size_t NUM_ARGS[NUM_OF_COMMANDS] = {
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1
};//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24

static const char IS_JMP[NUM_OF_COMMANDS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0
};

#endif //SAS_INSTRUCTIONS_HPP
