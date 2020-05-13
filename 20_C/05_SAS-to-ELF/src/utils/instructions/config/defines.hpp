#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <cstdio>

#define SET_BUF( size )     \
    buf_size = size;        \
    buffer = new char[size];

#define CHECK_REG( argument )                                   \
    if (argument->reg >= 8) {                                    \
        throw std::runtime_error("unsupported register used");  \
    }

struct Operand {
    volatile char r_m : 3;
    volatile char reg : 3;
    volatile char mod : 2;

    Operand(const char& _mod, const char& _reg, const char& _r_m) {
        mod = _mod;
        reg = _reg;
        r_m = _r_m;
    };

    operator unsigned char() {
        return *reinterpret_cast<char*>(this);
    }
};

enum REGISTERS {
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7
};

enum SYSTEM {
    SYSCALL = 0x05
};

enum EXTENTIONS {
    QWORD_OP = 0x48,
    EXT_REG  = 0x41,
    WIDE_OP  = 0x0F,
    REX_WRB  = 0x4D
};

enum JUMPS {
    CALL_NEAR = 0xE8,
    JMP_NEAR  = 0xE9,
    RET_NEAR  = 0xC3,

    JA_NEAR   = 0x87,
    JAE_NEAR  = 0x83,
    JB_NEAR   = 0x82,
    JBE_NEAR  = 0x86,
    JE_NEAR   = 0x84,
    JNE_NEAR  = 0x85
};

enum UTILITY {
    PUSH_REG = 0x50,
    PUSH_NUM = 0x68,
    POP_REG  = 0x58,
    MOV_NUM  = 0xB8,
    MOV_REG  = 0x89,
    CMP_REG  = 0x3B
};

enum MATH {
    ADD  = 0x01,
    SUB  = 0x29,
    IMUL = 0xAF,
    XOR  = 0x33,
};

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
    NUM_OF_COMMANDS = 23
};

static const size_t NUM_ARGS[NUM_OF_COMMANDS] = {
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0  
};//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22

static const char IS_JMP[NUM_OF_COMMANDS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0
};

typedef unsigned char byte_t;

#endif //DEFINES_HPP
